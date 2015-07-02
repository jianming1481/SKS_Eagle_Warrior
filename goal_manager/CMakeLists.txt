cmake_minimum_required(VERSION 2.8.3)
project(goal_manager)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp rospy 
                             genmsg
			     actionlib_msgs
			     actionlib
)
generate_messages(DEPENDENCIES actionlib_msgs) 
## Generate messages in the 'msg' folder
# add_message_files(
#   FILES
#   Message1.msg
#   Message2.msg
# )

## Generate services in the 'srv' folder
# add_service_files(
#   FILES
#   Service1.srv
#   Service2.srv
# )

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
#generate_messages(DEPENDENCIES geometry_msgs)


catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES goal_manager
#  CATKIN_DEPENDS other_catkin_pkg
#  DEPENDS system_lib
)

###########
## Build ##
###########
include_directories(include ${catkin_INCLUDE_DIRS})


add_executable(goal_manager src/goal_manager.cpp)
target_link_libraries(goal_manager ${catkin_LIBRARIES})
add_dependencies(goal_manager goal_manager_generate_messages_cpp)
 


