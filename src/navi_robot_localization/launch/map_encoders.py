#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import TransformStamped, Twist
from tf2_ros import TransformBroadcaster
import math

class Odom(Node):
    def __init__(self):
        super().__init__('odom_publisher')
        self.tf_broadcaster = TransformBroadcaster(self)

        # Suscriptor
        self.sub_cmd_vel = self.create_subscription(
            Twist,
            '/encoder_data',
            self.cmd_vel_callback,
            10
        )

        # Variables de posición/orientación
        self.x = 0.0
        self.y = 0.0
        self.th = 0.0 

        # Timer para publicar odometría (10 Hz)
        self.timer = self.create_timer(0.1, self.update_odom)
    
    def cmd_vel_callback(self, msg):
        # Actualiza posición/orientación basado en /cmd_vel
        self.x += msg.linear.x * 0.1  
        self.y += msg.linear.y * 0.1`
        self.th += msg.angular.z * 0.1
    
    def update_odom(self):
        t = TransformStamped()
        t.header.stamp = self.get_clock().now().to_msg()
        t.header.frame_id = 'odom'
        t.child_frame_id = 'base_link'

        # Posición
        t.transform.translation.x = self.x
        t.transform.translation.y = self.y
        t.transform.translation.z = 0.0

        # Orientación (convertir ángulo a quaternion)
        t.transform.rotation.x = 0.0
        t.transform.rotation.y = 0.0
        t.transform.rotation.z = math.sin(self.th / 2)
        t.transform.rotation.w = math.cos(self.th / 2)

        self.tf_broadcaster.sendTransform(t)
        self.get_logger().info(f'Odometría: x={self.x:.2f}, y={self.y:.2f}, th={math.degrees(self.th):.2f}°')

def main():
    rclpy.init()
    node = Odom()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == '__main__':
    main()
