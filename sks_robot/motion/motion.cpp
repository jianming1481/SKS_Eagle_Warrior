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
    double linear_x = msg->linear.x * 110;
    //double speed_const = 25*exp(fabs(msg->angular.z)/3.141516);	//Round 1
    double speed_const = 11*exp(fabs(msg->angular.z));    //Round 2
    ROS_INFO("x[%lf], z[%lf]", linear_x, msg->angular.z);
    double motor_left;							
    double motor_right;


    /*motor_left  = linear_x+speed_const*msg->angular.z;		//Round 1
    motor_right = linear_x-speed_const*msg->angular.z;
    if(motor_left >= 150 || motor_right >= 150){
	motor_left = msg->linear.x-40+speed_const*msg->angular.z;
        motor_right = msg->linear.x-40-speed_const*msg->angular.z;
    }else if((motor_left >= 127 && motor_left < 150) || (motor_right >= 127 && motor_right < 150)){
	motor_left = msg->linear.x-20+speed_const*msg->angular.z;
        motor_right = msg->linear.x-20-speed_const*msg->angular.z;
    }*/

    if((msg->linear.x > 0 && msg->angular.z >= 0) || (msg->linear.x < 0 && msg->angular.z < 0)){	//Round 2
	motor_left = linear_x - speed_const * msg->angular.z;
	motor_right = linear_x - 2 * speed_const * msg->angular.z;
    }else if((msg->linear.x > 0 && msg->angular.z < 0) || (msg->linear.x < 0 && msg->angular.z >= 0)){
	motor_left = linear_x + 2 * speed_const * msg->angular.z;
	motor_right = linear_x + speed_const * msg->angular.z;
    }else if(msg->linear.x == 0){
	if(msg->angular.z == 1){
		motor_left = 50;
		motor_right = -50;
	}else if (msg->angular.z == -1){
		motor_left = -50;
		motor_right = 50;
	}
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
