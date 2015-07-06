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
#include "qInterface.hpp"
#include <std_msgs/String.h>

/*****************************************************************************
** Implementation
*****************************************************************************/
//#define QT_NO_KEYWORDS

QInterface::QInterface(int argc, char** argv ) :
    QNode(argc,argv,"QInterface")
    {}

void QInterface::imageCb(const sensor_msgs::ImageConstPtr& msg){
//    try{
        cv_ptr = cv_bridge::toCvCopy(msg, enc::BGR8);
//    }catch (cv_bridge::Exception& e){
//        ROS_ERROR("cv_bridge exception: %s", e.what());
//        return;
//    }
}

void QInterface::ros_comms_init() {
    nh = new ros::NodeHandle();
    it_ = new image_transport::ImageTransport(*nh);
    sensor_msgs::ImageConstPtr msg;
    image_sub_ = it_->subscribe("/camera/image_raw" ,1 ,&QInterface::imageCb ,this);
    //setexposure = nh->serviceClient<prosilica_camera::camsrv>("/camera/set_exposure_info");
    //setwhitebalance = nh->serviceClient<prosilica_camera::camsrv>("/camera/set_whitebalance_info");
}


void QInterface::run() {
    ros::spin();
    Q_EMIT rosShutdown();
}
