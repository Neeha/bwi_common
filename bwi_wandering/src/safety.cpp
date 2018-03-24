#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Bool.h"
#include <geometry_msgs/Vector3Stamped.h>
#include <cmath>

sensor_msgs::LaserScan laser_msg;

void laser_cb(const sensor_msgs::LaserScan::ConstPtr& msg) {
  ROS_INFO("Laser Data Received\n");
  laser_msg = *msg;
}


int main(int argc, char **argv) {
    ROS_INFO("Started");
    ros::init(argc, argv, "safety");
    ros::NodeHandle nh_;

    ros::Subscriber laser_sub = nh_.subscribe("/scan_filtered", 1000, laser_cb);
    ros::Publisher twist_pub = nh_.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);
    ros::Publisher safe_pub = nh_.advertise<std_msgs::Bool>("/cmd_safe",1000);    

    ros::Time start_time = ros::Time::now();
    ros::Duration timeout(0.5);
    ROS_INFO("Spinning for 0.5 seconds");
    while(ros::Time::now()-start_time < timeout)
      ros::spinOnce();

    geometry_msgs::Twist twist_msg; 
    std_msgs::Bool is_poss_msg;
    // arbitrary value for now
    twist_msg.linear.x = 0.50;
    twist_msg.angular.z = 0;

    // assuming that this value is not arbritary, checking if it is okay to proceed
    while(ros::ok()){
      ros::spinOnce();

      float z = twist_msg.angular.z;
      float rad_index = 0;
      if (z < laser_msg.range_min && z > laser_msg.range_max) {
        ROS_INFO ("Range out of bound");
        is_poss_msg.data = false;
        safe_pub.publish(is_poss_msg);
      }
      else {
        rad_index = std::abs(laser_msg.angle_min - z)/laser_msg.angle_increment;

        float dist = laser_msg.ranges[rad_index];
        ROS_INFO("Distance from laser is %f", dist);

        if (dist > twist_msg.linear.x && !std::isinf(dist)) {
          ROS_INFO ("Valid Twist Message. No obstacle detected by laser.");
          twist_pub.publish(twist_msg);
          is_poss_msg.data = true;
          safe_pub.publish(is_poss_msg);
        }
        else {
          ROS_INFO ("Obstacle detected. Can't proceed");
          is_poss_msg.data = false;
          safe_pub.publish(is_poss_msg);
        }
      }
    }

    return 0;
}
