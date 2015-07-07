#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <geometry_msgs/Vector3.h>
#include <std_msgs/Char.h>
#include <actionlib/client/simple_action_client.h>
#include <geometry_msgs/Twist.h>

#define target_one_x  100.0 //1.0
#define target_one_y  100.0 //-5.48
#define target_two_x -100.940
#define target_two_y -103.0
#define goal_one_x    100.0
#define goal_one_y   -102.5
#define goal_two_x   -101.5
#define goal_two_y   -105.48
#define swap_area_x   100.0
#define swap_area_y   100.0
#define forward_speed 20


float posx,posy;
float ang;


void current_pos(const geometry_msgs::PoseStamped& pos){
	posx = pos.pose.position.x;
	posy = pos.pose.position.y;
}
void vec2speed(const geometry_msgs::Vector3& vec){
	ang = vec.z;
}
int main(int argc, char** argv){
	ros::init(argc, argv, "goal_manager");
	ros::NodeHandle n;
	
	ros::Subscriber position_sub = n.subscribe("/slam_out_pose",10,current_pos);
	ros::Subscriber vec_sub      = n.subscribe("/sks_vision",1,vec2speed);
	ros::Publisher  arm_command  = n.advertise<std_msgs::Char>("/cmd_pos",1);
	ros::Publisher  speed_com    = n.advertise<geometry_msgs::Twist>("/cmd_vel",1);
	std_msgs::Char  com;
	geometry_msgs::Twist sp;
	actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> Client("move_base", true);
	Client.waitForServer(); //will wait for infinite time
	move_base_msgs::MoveBaseGoal goal;
	goal.target_pose.header.frame_id = "map";
	bool goal_one_send = false,goal_two_send = false,goal_one_cancel = false,goal_two_cancel = false;
	
	
	
	while(ros::ok()){
		sp.linear.x  = forward_speed;
		sp.angular.z = ang/3.14;
		speed_com.publish(sp);	
		if(posx<target_one_x+0.1&&posx>target_one_x-0.1&&
	   	   posy<target_one_y+0.1&&posy>target_one_y-0.1&& !goal_one_send){
			ROS_INFO("arrived first target, sending first goal...");
			goal.target_pose.pose.position.x = goal_one_x;
			goal.target_pose.pose.position.y = goal_one_y;	
			goal.target_pose.pose.position.z = 0.0;
			goal.target_pose.pose.orientation.x = 0.0;
			goal.target_pose.pose.orientation.y = 0.0;
			goal.target_pose.pose.orientation.z = 0.0;
			goal.target_pose.pose.orientation.w = 2.57;
			ros::Duration(1.0).sleep(); //sleep for one sec
			Client.sendGoal(goal);
			goal_one_send = true;
		}
		if(posx<target_two_x+0.1&&posx>target_two_x-0.1&&
	   	   posy<target_two_y+0.1&&posy>target_two_y-0.1&& !goal_two_send){
			ROS_INFO("arrived second target, sending second goal...");
			goal.target_pose.pose.position.x = goal_two_x;
			goal.target_pose.pose.position.y = goal_two_y;	
			goal.target_pose.pose.position.z = 0.0;
			goal.target_pose.pose.orientation.x = 0.0;
			goal.target_pose.pose.orientation.y = 0.0;
			goal.target_pose.pose.orientation.z = 0.0;
			goal.target_pose.pose.orientation.w = 4.14;
			ros::Duration(1.0).sleep(); //sleep for one sec
			Client.sendGoal(goal);
			goal_two_send = true;
		}
		if(posx<goal_one_x+0.1&&posx>goal_one_x-0.1&&
	   	   posy<goal_one_y+0.1&&posy>goal_one_y-0.1&& !goal_one_cancel){
			ROS_INFO("arrived first goal!");
			Client.cancelAllGoals();
			goal_one_cancel = true;
		}
		if(posx<goal_two_x+0.1&&posx>goal_two_x-0.1&&
	   	   posy<goal_two_y+0.1&&posy>goal_two_y-0.1&& !goal_two_cancel){
			ROS_INFO("arrived second goal!");
			Client.cancelAllGoals();
			goal_two_cancel = true;		
		}
		if(posx<target_one_x+0.1&&posx>target_one_x-0.1&&
	   	   posy<target_one_y+0.1&&posy>target_one_y-0.1  ){
			com.data = 'g';
			arm_command.publish(com);
		} 
	}
	return 0;
}
