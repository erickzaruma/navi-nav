# navi-nav

Repo for the use of ROS2 Humble and SLAM for control and navigation of the navi robotic platform. To be optimized for the Jetson Orin NX embedded platform

Run natively on Ubuntu 22.04, if not available then Docker must be used.

## Quick start
[ bash ]
git clone https://github.com/erickzaruma/navi-nav navi_nav && cd navi_nav
colcon build --symlink-install
source install/setup.bash
ros2 launch navi_robot_bringup sim_house.launch.py

## STM32 Folder
Folder for STM32 Workspace Development in STM32CubeIDE (HAL)
The file `navibot_v0.zip` contains all project files. To access the project, please unzip the file.

To develop a new STM32 project based on this one, create a new *STM32 Project from an Existing STM32 Configuration File (.ioc)* in STM32CubeIDE, and import the file `navibot_v.0.ioc`.

To flash the firmware to the robot's board, upload the file `navibot_v.0.hex` located in the `Debug` folder.



