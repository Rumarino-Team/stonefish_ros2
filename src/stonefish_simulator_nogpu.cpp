/*    
    This file is a part of stonefish_ros2.

    stonefish_ros is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    stonefish_ros is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

//
//  stonefish_simulator_nogpu.cpp
//  stonefish_ros2
//
//  Created by Patryk Cieslak on 05/10/23.
//  Copyright (c) 2023 Patryk Cieslak. All rights reserved.
//

#include "rclcpp/rclcpp.hpp"
#include <Stonefish/utils/SystemUtil.hpp>
#include "stonefish_ros2/ROS2SimulationManager.h"
#include "stonefish_ros2/ROS2ConsoleSimulationApp.h"

using namespace std::chrono_literals;

static bool parse_bool_arg(const char* value)
{
    std::string s(value);
    return s == "true" || s == "1" || s == "yes" || s == "on";
}

class StonefishConsoleNode : public rclcpp::Node
{
public:
    StonefishConsoleNode(const std::string& scenarioPath,
                           const std::string& dataPath,
                           sf::Scalar rate,
                           sf::Scalar fixedTimeStep,
                           bool useSimulationTimeStamps) 
                           : Node("stonefish_simulator_nogpu")
    {   
        sf::ROS2SimulationManager* manager = new sf::ROS2SimulationManager(rate, scenarioPath, std::shared_ptr<rclcpp::Node>(this), useSimulationTimeStamps);
        app_ = std::shared_ptr<sf::ROS2ConsoleSimulationApp>(new sf::ROS2ConsoleSimulationApp("Stonefish Simulator", dataPath, manager));
        app_->Startup(fixedTimeStep);
    };

    void Shutdown()
    {
        app_->Shutdown();
    };

private:
    std::shared_ptr<sf::ROS2ConsoleSimulationApp> app_;
};

int main(int argc, char **argv)
{
	rclcpp::init(argc, argv, rclcpp::InitOptions(), rclcpp::SignalHandlerOptions::None);
    
    //Check number of command line arguments
	if(argc < 4)
	{
		std::cout << "Not enough command-line arguments provided!" << std::endl;
		exit(-1);
	}

    //Parse the arguments
    std::string dataPath = std::string(argv[1]) + "/";
    std::string scenarioPath(argv[2]);
    sf::Scalar rate = atof(argv[3]);
    bool fastFixedStep = argc > 4 ? parse_bool_arg(argv[4]) : false;
    sf::Scalar fixedTimeStep = argc > 5 ? atof(argv[5]) : sf::Scalar(0);
    bool useSimulationTimeStamps = argc > 6 ? parse_bool_arg(argv[6]) : fastFixedStep;

    if(fastFixedStep && fixedTimeStep <= sf::Scalar(0))
    {
        fixedTimeStep = sf::Scalar(1) / rate;
    }
    if(!fastFixedStep)
    {
        fixedTimeStep = sf::Scalar(0);
    }

    // Start simulation
    std::shared_ptr<StonefishConsoleNode> node(new StonefishConsoleNode(scenarioPath, dataPath, rate, fixedTimeStep, useSimulationTimeStamps));
    rclcpp::spin(node);
    node->Shutdown();
    rclcpp::shutdown();
    return 0;
}
