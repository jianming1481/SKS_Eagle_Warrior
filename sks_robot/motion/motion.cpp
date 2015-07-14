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
    double speed_const = 25*exp(fabs(msg->angular.z)/3.141516);
    ROS_INFO("x[%lf], z[%lf]", msg->linear.x, msg->angular.z);

    double motor_left  = msg->linear.x+speed_const*msg->angular.z;
    double motor_right = msg->linear.x-speed_const*msg->angular.z;
    if(motor_left >= 150 || motor_right >= 150){
	motor_left = msg->linear.x-40+speed_const*msg->angular.z;
        motor_right = msg->linear.x-40-speed_const*msg->angular.z;
    }else if((motor_left >= 127 && motor_left < 150) || (motor_right >= 127 && motor_right < 150)){
	motor_left = msg->linear.x-20+speed_const*msg->angular.z;
        motor_right = msg->linear.x-20-speed_const*msg->angular.z;
    }
    ROS_INFO("motor_left[%lf], motor_right[%lf], motor_speed[%lf]", motor_left,motor_right,speed_const);
    
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
