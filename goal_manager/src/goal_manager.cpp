#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <geometry_msgs/Vector3.h>
#include <std_msgs/Char.h>
#include <actionlib/client/simple_action_client.h>
#include <geometry_msgs/Twist.h>
#include <math.h>
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


//float posx,posy;
geometry_msgs::Twist sp;
geometry_msgs::Twist sp2;
geometry_msgs::PoseStamped cu_pos;
void current_pos(const geometry_msgs::PoseStamped::ConstPtr& pos){
	cu_pos.pose.position.x = pos->pose.position.x;
	cu_pos.pose.position.y = pos->pose.position.y;
}
void vec2speed(const geometry_msgs::Vector3::ConstPtr& vec){	
	sp.angular.z = vec->z;	
}
int main(int argc, char** argv){
	ros::init(argc, argv, "goal_manager");
	ros::NodeHandle n;
	
	ros::Subscriber position_sub = n.subscribe("/slam_out_pose",10,current_pos);
	ros::Subscriber vec_sub      = n.subscribe("/sks_vision",1,vec2speed);
	ros::Publisher  arm_command  = n.advertise<std_msgs::Char>("/cmd_pos",1);
	ros::Publisher  speed_com    = n.advertise<geometry_msgs::Twist>("/cmd_vel",10);

	std_msgs::Char  com;
	
	actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> Client("move_base", true);
	//Client.waitForServer(); //will wait for infinite time
	move_base_msgs::MoveBaseGoal goal;
	goal.target_pose.header.frame_id = "map";
	bool goal_one_send = false,goal_two_send = false,goal_one_cancel = false,goal_two_cancel = false,swap_detect = false;
	
	ros::Rate rate(1000);
	
	while(ros::ok()){
		//if(posx>target_one_x+0.1&&posx<target_one_x-0.1&&
	   	//   posy>target_one_y+0.1&&posy<target_one_y-0.1&&
		//   posx>target_two_x+0.1&&posx<target_two_x-0.1&&
	   	//   posy>target_two_y+0.1&&posy<target_two_y-0.1  )
		
		if(sp.angular.z<0.5&&sp.angular.z>-0.5){	
			sp2.linear.x  = 40;
			sp2.angular.z =	sp.angular.z;				
			speed_com.publish(sp2);
		}else{
			if(sp.angular.z==999){		 
				sp2.linear.x  = 10;
				sp2.angular.z = 0;
				speed_com.publish(sp2);
			}else{
				sp2.linear.x  = 0;
				sp2.angular.z =	sp.angular.z;				
				speed_com.publish(sp2);	
			}
		}

		if(cu_pos.pose.position.x<target_one_x+0.2&&cu_pos.pose.position.x>target_one_x-0.2&&
	   	   cu_pos.pose.position.y<target_one_y+0.2&&cu_pos.pose.position.y>target_one_y-0.2&& !goal_one_send){
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
		if(cu_pos.pose.position.x<target_two_x+0.2&&cu_pos.pose.position.x>target_two_x-0.2&&
	   	   cu_pos.pose.position.y<target_two_y+0.2&&cu_pos.pose.position.y>target_two_y-0.2&& !goal_two_send){
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
		if(cu_pos.pose.position.x<goal_one_x+0.2&&cu_pos.pose.position.x>goal_one_x-0.2&&
	   	   cu_pos.pose.position.y<goal_one_y+0.2&&cu_pos.pose.position.y>goal_one_y-0.2&& !goal_one_cancel){
			ROS_INFO("arrived first goal!");
			Client.cancelAllGoals();
			goal_one_cancel = true;
		}
		if(cu_pos.pose.position.x<goal_two_x+0.1&&cu_pos.pose.position.x>goal_two_x-0.1&&
	   	   cu_pos.pose.position.y<goal_two_y+0.1&&cu_pos.pose.position.y>goal_two_y-0.1&& !goal_two_cancel){
			ROS_INFO("arrived second goal!");
			Client.cancelAllGoals();
			goal_two_cancel = true;		
		}
		if(cu_pos.pose.position.x<target_one_x+0.1&&cu_pos.pose.position.x>target_one_x-0.1&&
	   	   cu_pos.pose.position.y<target_one_y+0.1&&cu_pos.pose.position.y>target_one_y-0.1&& !swap_detect){
			com.data = 'g';
			arm_command.publish(com);
		}
		ros::spinOnce();
		rate.sleep();
	}
	return 0;
}
