/*****************************************************************************
** Includes
*****************************************************************************/

#include <ros/ros.h>
#include <string>
#include <sstream>
#include "qvision.hpp"
#include <std_msgs/String.h>
#include "geometry_msgs/Twist.h"

/*****************************************************************************
** Implementation
*****************************************************************************/
//#define QT_NO_KEYWORDS

QVISION::QVISION(int argc, char** argv ) :
    QNode(argc,argv,"sks_vision")
    {}

void QVISION::ros_comms_init() {
    nh = new ros::NodeHandle();
    sensor_msgs::ImageConstPtr msg;
}

void QVISION::run() {
    ros::spin();
    Q_EMIT rosShutdown();
}
