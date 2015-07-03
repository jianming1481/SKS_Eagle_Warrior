#ifndef QVISION_NODE_HPP_
#define QVISION_NODE_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#ifndef Q_MOC_RUN
#include <ros/ros.h>
//#include "../include/testQT/qnode.hpp"
#include "qnode.hpp"
#include <QObject>
#endif

#include <ros/ros.h>
#include <string>
#include <std_msgs/String.h>

#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

/*****************************************************************************
** Class
*****************************************************************************/
namespace enc = sensor_msgs::image_encodings;

Q_DECLARE_METATYPE(std::string);

class QVISION : public QNode {
 Q_OBJECT


public:
    QVISION(int argc, char** argv);
    virtual ~QVISION() {}
    void run();
    void ros_comms_init();

    //----------------distance
    void setPara_space(int space){
        nh->setParam("/FIRA/Distance_Model/Block" ,space);
    }
    void setPara_num(int num){
        nh->setParam("/FIRA/Distance_Model/Number" ,num);
    }
    void setPara(std::vector<int> ivector){
        nh->setParam("/FIRA/Distance_Model/Pixel" ,ivector);
    }
    void setPara_discheck(int discheck){
        nh->setParam("/FIRA/Distance_Model/Check" ,discheck);
    }

Q_SIGNALS:

private:
    ros::NodeHandle *nh;
    ros::ServiceClient setexposure;
    ros::ServiceClient setwhitebalance;
};

#endif /* QServer_NODE_HPP_ */
