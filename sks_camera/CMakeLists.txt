##############################################################################
# CMake
##############################################################################

cmake_minimum_required(VERSION 2.8.0)
project(sks_camera)

##############################################################################
# Catkin
##############################################################################

# qt_build provides the qt cmake glue, roscpp the comms for a default talker
find_package(catkin REQUIRED COMPONENTS qt_build roscpp
message_generation std_msgs sensor_msgs cv_bridge image_transport)
find_package(OpenCV)

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
#testQT
#file(GLOB QT_FORMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ui/*.ui)
#file(GLOB QT_RESOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} resources/*.qrc)
#file(GLOB_RECURSE QT_MOC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS include/testQT/*.hpp)
#test_Node
#file(GLOB QT_FORMS_testNode RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test_Node/testnode_mainwindow.ui)
#file(GLOB_RECURSE QT_MOC_testNode RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS test_Node/testnode_mainwindow.h test_Node/func.h)

#testQT
#QT4_ADD_RESOURCES(QT_RESOURCES_CPP ${QT_RESOURCES})
#QT4_WRAP_UI(QT_FORMS_HPP ${QT_FORMS})
#QT4_WRAP_CPP(QT_MOC_HPP ${QT_MOC})
#test_Node
#QT4_WRAP_UI(QT_FORMS_HPP_testNode ${QT_FORMS_testNode})
#QT4_WRAP_CPP(QT_MOC_HPP_testNode ${QT_MOC_testNode})

##############################################################################
# Sources
##############################################################################
#testQT
#file(GLOB_RECURSE QT_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS src/main.cpp src/main_window.cpp src/qnode.cpp)
#test_Node
#file(GLOB_RECURSE QT_SOURCES_testNode RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS test_Node/main.cpp test_Node/#testnode_mainwindow.cpp test_Node/func.cpp)

##############################################################################
# Binaries
##############################################################################
#testQT
#add_executable(testQT ${QT_SOURCES} ${QT_RESOURCES_CPP} ${QT_FORMS_HPP} ${QT_MOC_HPP})
#target_link_libraries(testQT ${QT_LIBRARIES} ${catkin_LIBRARIES})
#install(TARGETS testQT RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

#test_Node
#add_executable(test_Node ${QT_SOURCES_testNode} ${QT_FORMS_HPP_testNode} ${QT_MOC_HPP_testNode})
#target_link_libraries(test_Node ${QT_LIBRARIES} ${catkin_LIBRARIES} ${OpenCV_LIBS})
#install(TARGETS test_Node RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

#simple_image_transport
# add the publisher example
add_executable(my_publisher simple_image_transport/my_publisher.cpp)
target_link_libraries(my_publisher ${catkin_LIBRARIES} ${OpenCV_LIBS})
# add the subscriber example
#add_executable(my_subscriber simple_image_transport/my_subscriber.cpp)
#target_link_libraries(my_subscriber ${QT_LIBRARIES} ${OpenCV_LIBS} ${catkin_LIBRARIES})
