cmake_minimum_required(VERSION 3.8)
project(navi_robot_localization)

# Buscar dependencias requeridas
find_package(ament_cmake REQUIRED)
find_package(rclpy REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(rplidar_ros REQUIRED)
find_package(slam_toolbox REQUIRED)
find_package(navi_robot_bringup REQUIRED)

install(PROGRAMS
  scripts/map_encoders.py
  DESTINATION lib/${PROJECT_NAME}
)

# Archivos de lanzamiento
install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}
)

# Archivos de configuración
install(DIRECTORY
  config
  DESTINATION share/${PROJECT_NAME}
)

# Configuración de RViz
install(DIRECTORY
  rviz
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
