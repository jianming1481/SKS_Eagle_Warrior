/**
 * @file /QServer_server/QServer.cpp
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
#include "qsks_vision.hpp"
#include <std_msgs/String.h>

/*****************************************************************************
** Implementation
*****************************************************************************/
//#define QT_NO_KEYWORDS

QSKS::QSKS(int argc, char** argv ) :
    QNode(argc,argv,"QSKS")
    {}

void QSKS::ros_comms_init() {
}


void QSKS::run() {
    ros::spin();
    Q_EMIT rosShutdown();
}
