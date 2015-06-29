/**
 * @file /QClient_server/QClient.cpp
 *
 * @brief Ros communication central!
 *
 * @date February 2011
 **/

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ros/ros.h>
#include <string>
#include <sstream>
#include "qClient.hpp"
#include <std_msgs/String.h>



/*****************************************************************************
** Implementation
*****************************************************************************/

QClient::QClient(int argc, char** argv) :
    QNode(argc,argv,"SKS_UI")
    {

    }
void QClient::ros_comms_init() {
    n = new ros::NodeHandle();
    sks_vel_pub = n->advertise<geometry_msgs::Twist>(motion_topic_name,1000);
}

void QClient::run() {
//    ros::Rate loop_rate(100);

//    while(ros::ok())
//    {
//        ros::spinOnce();
//        loop_rate.sleep();
//    }
}

void QClient::pub_sks_vel(geometry_msgs::Twist vel)
{
    sks_vel_pub.publish(vel);
}

void QClient::set_robot_speed(double v_x, double v_y, double yaw)
{
    msg.linear.x = v_x;
    msg.linear.y = v_y;
    msg.angular.z = yaw;

    pub_sks_vel(msg);
}
