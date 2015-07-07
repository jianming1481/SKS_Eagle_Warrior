#include "goal_manager.h"



#define target_one_x 0.0 //1.0
#define target_one_y 0.0 //-5.48
#define target_two_x -0.940
#define target_two_y -3.0
#define goal_one_x   0.0
#define goal_one_y   -2.5
#define goal_two_x   -1.5
#define goal_two_y   -5.48
#define swap_area_x  0.0
#define seap_area_y  0.0

void goal_manager::current_pos(const geometry_msgs::PoseStamped& pos){
	posx = pos.pose.position.x;
	posy = pos.pose.position.y;
}
void goal_manager::vision_vec(const geometry_msgs::Quaternion& ori){
	if(ori.z!=999){
	speed.linear.x  = 20;
	speed.angular.z = ori.z;
	speed_pub.publish(speed);
	}
}
void goal_manager::goal_manager_init(){
	n = new ros::NodeHandle();
	speed_pub     = n.advertise<geometry_msgs::Twist>("/cmd_vel",1);
	position_sub  = n.subscribe("/slam_out_pose",10,goal_manager::current_pos);
	vec           = n.subscribe("/",10,goal_manager::vision_vec);
	swap_pub      = n.advertise<std_msgs::Char>("/cmd_pos",10);
	actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> Client("move_base", true);
	move_base_msgs::MoveBaseGoal goal;
	goal.target_pose.header.frame_id = "map";
	Client.waitForServer();//will wait for infinite time

	
}
void goal_manager::manager(){
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
	if(posx<goal_two_x+0.1&&posx>goal_two_x-0.1&&
   	   posy<goal_two_y+0.1&&posy>goal_two_y-0.1){
		
	}
}
	bool goal_one_send = false,goal_two_send = false,goal_one_cancel = false,goal_two_cancel = false;
	while(ros::ok()){	
		
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
		if()
	}
	return 0;		
}
