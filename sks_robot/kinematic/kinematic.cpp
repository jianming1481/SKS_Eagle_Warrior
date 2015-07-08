/*
 * csll is for serial port (RS232)
 */

// %Tag(FULLTEXT)%
#include "ros/ros.h"
//#include "std_msgs/String.h"
#include "std_msgs/Char.h"
#include "geometry_msgs/Twist.h"

#include "ArmControl.h"

//====================//
//ROS motion callback //
//====================//
void ArmCallback(const std_msgs::Char::ConstPtr& cmd)
{
    ROS_INFO("cmd_pos : %c", cmd->data);
    double tmp_char = cmd->data;

    mcssl_send2arm(tmp_char);
}


int main(int argc, char **argv)
{

    //RS232 init
    if(mcssl_init()<=0){
        return 0;
    }

    //ros init
    ros::init(argc, argv, "sksArm");

   //node handle
    ros::NodeHandle n;

    //motion subscriber
    ros::Subscriber sub = n.subscribe(arm_topic_name, 1000, ArmCallback);

    //spin
    ros::spin();


    //RS232 finish
    mcssl_finish();

    return 0;
}
