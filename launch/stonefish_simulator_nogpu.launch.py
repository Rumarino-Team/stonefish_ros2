from launch_ros.actions import Node
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration

def generate_launch_description():
    simulation_data = LaunchConfiguration('simulation_data')
    scenario_desc = LaunchConfiguration('scenario_desc')
    simulation_rate = LaunchConfiguration('simulation_rate')
    fast_fixed_step = LaunchConfiguration('fast_fixed_step')
    fixed_time_step = LaunchConfiguration('fixed_time_step')
    use_sim_time_stamps = LaunchConfiguration('use_sim_time_stamps')
    
    simulation_data_arg = DeclareLaunchArgument(
        'simulation_data',
        default_value = ''
    )

    scenario_desc_arg = DeclareLaunchArgument(
        'scenario_desc',
        default_value = ''
    )

    simulation_rate_arg = DeclareLaunchArgument(
        'simulation_rate',
        default_value = '100.0'
    )

    fast_fixed_step_arg = DeclareLaunchArgument(
        'fast_fixed_step',
        default_value = 'false'
    )

    fixed_time_step_arg = DeclareLaunchArgument(
        'fixed_time_step',
        default_value = '0.0'
    )

    use_sim_time_stamps_arg = DeclareLaunchArgument(
        'use_sim_time_stamps',
        default_value = 'false'
    )

    stonefish_simulator_nogpu_node = Node(
            package='stonefish_ros2',
            executable='stonefish_simulator_nogpu',
            namespace='stonefish_ros2',
            name='stonefish_simulator_nogpu',
            arguments=[simulation_data, scenario_desc, simulation_rate, fast_fixed_step, fixed_time_step, use_sim_time_stamps],
            output='screen',
    )

    return LaunchDescription([
        simulation_data_arg,
        scenario_desc_arg,
        simulation_rate_arg,
        fast_fixed_step_arg,
        fixed_time_step_arg,
        use_sim_time_stamps_arg,
        stonefish_simulator_nogpu_node
    ])
