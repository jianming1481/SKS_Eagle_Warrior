#ifndef goal_manager_HPP
#define goal_manager_HPP
#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Twist.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>



class goal_manager {

public:
	goal_manager(int argc, char** argv);
	float posx,posy;
	manager();
	bool goal_one_send = false,
	     goal_two_send = false,
	     goal_one_cancel = false,
 	     goal_two_cancel = false;
protected:
	void goal_manager_init();
private:
	

	ros::Publisher  speed_pub    ;
	ros::Subscriber position_sub ;
	ros::Subscriber vec          ;
	ros::Publisher  swap_pub     ;

	geometry_msgs::Twist speed;
	move_base_msgs::MoveBaseGoal goal;
	void current_pos(const geometry_msgs::PoseStamped& pos);

	void vision_vec(const geometry_msgs::Quaternion& ori);
};
#endif
