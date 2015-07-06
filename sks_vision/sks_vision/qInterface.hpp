/**
 * @file /QServer_server/QServer.hpp
 *
 * @brief Ros communication central!
 *
 * @date February 2011
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef QServer_NODE_HPP_
#define QServer_NODE_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#ifndef Q_MOC_RUN
#include <ros/ros.h>
#include "../common/qnode.hpp"

#include <QObject>
#endif

#include <ros/ros.h>
#include "../common/qnode.hpp"
#include <string>
#include <std_msgs/String.h>
#include <vision/MSN.h>
//#include <prosilica_camera/camsrv.h>

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

class QInterface : public QNode {
 Q_OBJECT


public:
    QInterface(int argc, char** argv);
    virtual ~QInterface() {}
    void run();
    void ros_comms_init();

    void imageCb(const sensor_msgs::ImageConstPtr& msg);
    cv_bridge::CvImagePtr cv_ptr;

    void set_exposure(int value){
//        //prosilica_camera::camsrv srv;
//        //srv.request.cam_exposure = value;
//        if(setexposure.call(srv)){

//        }else{
//            ROS_ERROR("Failed");
//            return;
//        }
//        return;
    }
    void set_whitebalance(int value){
//        prosilica_camera::camsrv srv;
//        srv.request.cam_white = value;
//        if(setwhitebalance.call(srv)){

//        }else{
//            ROS_ERROR("Failed");
//            return;
//        }
//        return;
    }
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
    //-------------------------

    void setPara_(char path[20],std::vector<int> vec){
        nh->setParam(path,vec);
    }
    //----------------HSV
    void setPara_mapcheck(int mapcheck){
        nh->setParam("/FIRA/Color_Map/Check" ,mapcheck);
    }
    //-------------------------

    //----------------center
    void sent_center_X(int center_x){
        nh->setParam("/FIRA/Center/X",center_x);
    }
    void sent_center_Y(int center_y){
        nh->setParam("/FIRA/Center/Y",center_y);
    }
    void sent_center_Inner(int center_inner){
        nh->setParam("/FIRA/Center/Inner",center_inner);
    }
    void sent_center_Outer(int center_outer){
        nh->setParam("/FIRA/Center/Outer",center_outer);
    }
    void sent_center_Front(int center_front){
        nh->setParam("/FIRA/Center/Front",center_front);
    }
    //-------------------------

    //---------------white

    void sent_white_gray(int white_gray){
        nh->setParam("/FIRA/White/Gray",white_gray);
    }
    void sent_white_angle(int white_angle){
        nh->setParam("/FIRA/White/Angle",white_angle);
    }



    //-----------------------------
    void setCheck(char path[20],int Check){
        nh->setParam(path,Check);
    }
    void setCheck_scan(int r){
        nh->setParam("/FIRA/Scanning/Check",r);
    }
    void setPara_scan(std::vector<int>scan){
        nh->setParam("/FIRA/Scanning/Scan",scan);
    }
    int get_center(char para[20],int get){
        if(nh->getParam(para,get)){
        return get;
        }
    }
    //---------------Black
    void set_value_StopAngle(std::vector<int>angle){
        nh->setParam("/FIRA/Black/Anglearry",angle);
    }
    void sent_black_angle(int black_angle){
        nh->setParam("/FIRA/Black/Angle",black_angle);
    }
    void setPara_Vec(char para[30],std::vector<int>vec){
        nh->setParam(para,vec);
    }
    std::vector<int> ovector_DisReal;
    void getPara_DisReal(){
        nh->getParam("/FIRA/Distance_Model/Pixel", ovector_DisReal);
    }

//    std::vector<int> ovector_DisPixel;
//    void getPara_DisPixel(){
//        nh->getParam("/FIRA/Center/Stop/Dis", ovector_DisPixel);
//    }

    //-------------------------------------

Q_SIGNALS:

private:
    ros::NodeHandle *nh;
    image_transport::ImageTransport *it_;
    image_transport::Subscriber image_sub_;
    ros::ServiceClient setexposure;
    ros::ServiceClient setwhitebalance;

};

#endif /* QServer_NODE_HPP_ */
