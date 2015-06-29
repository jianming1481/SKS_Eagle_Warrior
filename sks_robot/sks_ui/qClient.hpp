/**
 * @file /QClient_server/QClient.hpp
 *
 * @brief Ros communication central!
 *
 * @date February 2011
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef QClient_HPP_
#define QClient_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#ifndef Q_MOC_RUN
#include <ros/ros.h>
#include "../common/qnode.hpp"
#endif
#include <string>
#include <std_msgs/String.h>
#include "geometry_msgs/Twist.h"


/*****************************************************************************
** Class
*****************************************************************************/

class QClient : public QNode {

public:
    QClient(int argc, char** argv);
    virtual ~QClient() {}
    void run();
    void ros_comms_init();

    void pub_sks_vel(geometry_msgs::Twist vel);
    void set_robot_speed(double v_x,double v_y,double yaw);

private:
    const char *motion_topic_name = "/sks_speed_topic";
    ros::NodeHandle *n;
    ros::Publisher sks_vel_pub;
    geometry_msgs::Twist msg;
};

#endif /* QClient_NODE_HPP_ */
