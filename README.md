# navi-nav

Repo for the use of ROS2 Humble and SLAM for control and navigation of the navi robotic platform. To be optimized for the Jetson Orin NX embedded platform

Run natively on Ubuntu 22.04, if not available then Docker must be used.

## Quick start
[ bash ]
git clone https://github.com/erickzaruma/navi-nav navi_nav && cd navi_nav
colcon build --symlink-install
source install/setup.bash
ros2 launch navi_robot_bringup sim_house.launch.py
