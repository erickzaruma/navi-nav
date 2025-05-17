#!/usr/bin/env python3
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import ExecuteProcess, RegisterEventHandler, TimerAction
from launch.event_handlers import OnProcessExit

def generate_launch_description():
    # 1. Driver del LIDAR
    lidar_node = Node(
        package='rplidar_ros',
        executable='rplidar_c1_launch.py',
        parameters=[{'serial_port': '/dev/lidar'}],
        output='screen'
    )

    # 2. Transformación estática del LIDAR
    static_tf = Node(
        package='tf2_ros',
        executable='static_transform_publisher',
        arguments=['0', '0', '0', '0', '0', '0', 'base_link', 'laser'],
        output='screen'
    )

    # 3. Odometría con encoders
    encoder_odom = Node(
        package='navi_robot_localization',
        executable='map_encoders.py',
        output='screen'
    )

    # 4. SLAM Toolbox
    slam_toolbox = Node(
        package='slam_toolbox',
        executable='online_async_launch.py',
        parameters=['/home/jetson/odom.yaml'],
        output='screen'
    )

    # 5. RViz2
    rviz2 = ExecuteProcess(
        cmd=['rviz2'],
        output='screen'
    )

    # Secuenciación con delays
    delayed_slam = TimerAction(
        period=2.0,
        actions=[slam_toolbox]
    )

    delayed_rviz = TimerAction(
        period=4.0,  # 2s después del SLAM
        actions=[rviz2]
    )

    return LaunchDescription([
        lidar_node,
        static_tf,
        encoder_odom,
        delayed_slam,
        delayed_rviz,
        
        # Manejo de shutdown
        RegisterEventHandler(
            event_handler=OnProcessExit(
                target_action=rviz2,
                on_exit=[lidar_node, static_tf, encoder_odom, slam_toolbox]
            )
        )
    ])