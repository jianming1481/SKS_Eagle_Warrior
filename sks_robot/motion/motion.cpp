/*
 * csll is for serial port (RS232)
 */

// %Tag(FULLTEXT)%
#include "ros/ros.h"
//#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"

#include "MotorControl.h"


//====================//
//ROS motion callback //
//====================//
void motionCallback(const geometry_msgs::Twist::ConstPtr& msg)
{
    ROS_INFO("x[%lf], z[%lf]", msg->linear.x, msg->angular.z);
    double motor_left  = 100*(msg->linear.x)+30*msg->angular.z;
    double motor_right = 100*(msg->linear.x)-30*msg->angular.z;
    if(abs(motor_left)>=127)
	motor_left = 127;
    if(abs(motor_right)>=127)
	motor_right = 127;
    ROS_INFO("motor_left[%lf], motor_right[%lf]", motor_left,motor_right);

    mcssl_send2motor(motor_left, motor_right);
}


int main(int argc, char **argv)
{

    //RS232 init
    if(mcssl_init()<=0){
        return 0;
    }

    //ros init
    ros::init(argc, argv, "sksMotion");

   //node handle
    ros::NodeHandle n;

    //motion subscriber
    ros::Subscriber sub = n.subscribe(motion_topic_name, 1000, motionCallback);

    //spin
    ros::spin();


    //RS232 finish
    mcssl_finish();

    return 0;
}
