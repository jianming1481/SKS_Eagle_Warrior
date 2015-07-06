##############################################################################
# CMake
##############################################################################

cmake_minimum_required(VERSION 2.8.0)
project(sks_vision)

##############################################################################
# Catkin
##############################################################################

# qt_build provides the qt cmake glue, roscpp the comms for a default talker
find_package(catkin REQUIRED COMPONENTS qt_build roscpp
message_generation std_msgs sensor_msgs cv_bridge image_transport)
include_directories(${catkin_INCLUDE_DIRS})
# Use this to define what the package will export (e.g. libs, headers).
# Since the default here is to produce only a binary, we don't worry about
# exporting anything. 
catkin_package()

##############################################################################
# Qt Environment
##############################################################################

# this comes from qt_build's qt-ros.cmake which is automatically 
# included via the dependency call in package.xml
rosbuild_prepare_qt4(QtCore QtGui) # Add the appropriate components to the component list here

##############################################################################
# Sections
##############################################################################
#test_node
file(GLOB QT_FORMS_test_node RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test_node/testnode_mainwindow.ui)
file(GLOB_RECURSE QT_MOC_test_node RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS test_node/testnode_mainwindow.h)
#sks_vision
file(GLOB QT_FORMS_sks_vision RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} sks_vision/sksvision_mainwindow.ui)
file(GLOB_RECURSE QT_MOC_sks_vision RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS sks_vision/sksvision_mainwindow.h sks_vision/func.h
#sks_vision/qInterface.hpp sks_vision/qnode.hpp
)
#test_node
QT4_WRAP_UI(QT_FORMS_HPP_test_node ${QT_FORMS_test_node})
QT4_WRAP_CPP(QT_MOC_HPP_test_node ${QT_MOC_test_node})
#sks_vision
QT4_WRAP_UI(QT_FORMS_HPP_sks_vision ${QT_FORMS_sks_vision})
QT4_WRAP_CPP(QT_MOC_HPP_sks_vision ${QT_MOC_sks_vision})


##############################################################################
# Sources
##############################################################################
#test_node
file(GLOB_RECURSE QT_SOURCES_test_node RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS test_node/main.cpp test_node/testnode_mainwindow.cpp)
#sks_vision
file(GLOB_RECURSE QT_SOURCES_sks_vision RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS sks_vision/main.cpp sks_vision/sksvision_mainwindow.cpp sks_vision/func.cpp
#sks_vision/qInterface.cpp sks_vision/qnode.cpp
)

##############################################################################
# Binaries
##############################################################################
#test_node
add_executable(test_node ${QT_SOURCES_test_node} ${QT_FORMS_HPP_test_node} ${QT_MOC_HPP_test_node})
target_link_libraries(test_node ${QT_LIBRARIES} ${catkin_LIBRARIES})
#sks_vision
add_executable(sks_vision ${QT_SOURCES_sks_vision} ${QT_FORMS_HPP_sks_vision} ${QT_MOC_HPP_sks_vision})
target_link_libraries(sks_vision ${QT_LIBRARIES} ${catkin_LIBRARIES})


