##############################################################################
# CMake
##############################################################################

cmake_minimum_required(VERSION 2.8.0)
project(sks_vision)
find_package(catkin)

##############################################################################
# Catkin
##############################################################################

# qt_build provides the qt cmake glue, roscpp the comms for a default talker
find_package(catkin REQUIRED COMPONENTS qt_build message_generation std_msgs roscpp
	     sensor_msgs cv_bridge image_transport)
include_directories(${catkin_INCLUDE_DIRS})

##############################################################################
# Qt Environment
##############################################################################

# this comes from qt_build's qt-ros.cmake which is automatically 
# included via the dependency call in manifest.xml
rosbuild_prepare_qt4(QtCore QtGui) # Add the appropriate components to the component list here

##############################################################################
# Services
##############################################################################

#add_service_files(DIRECTORY srv FILES TwoInts.srv)
#add_service_files(
#  FILES
#)
#add_message_files(
#  FILES
#)
generate_messages(DEPENDENCIES std_msgs)
catkin_package(CATKIN_DEPENDS message_runtime std_msgs roscpp 
		sensor_msgs cv_bridge image_transport)

##############################################################################
# Subprojects
##############################################################################

foreach(subdir
    common
    sks_vision
    sks_vision_nui#
    )
  #install(DIRECTORY ${subdir} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/qt_tutorials)
  add_subdirectory(${subdir})
endforeach()
