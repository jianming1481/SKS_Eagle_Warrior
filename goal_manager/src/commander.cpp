#include "goal_manager.h"
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

#define target_one_x 0.0 //1.0
#define target_one_y 0.0 //-5.48
#define target_two_x -0.940
#define target_two_y -3.0
#define goal_one_x   0.0
#define goal_one_y   -2.5
#define goal_two_x   -1.5
#define goal_two_y   -5.48






int main(int argc, char** argv){
	ros::init(argc,argv,"commander");
	goal_manager::goal_manager_init();
	while(ros::ok()){
		goal_manager::manager();
	}	
	return 0;
	}
