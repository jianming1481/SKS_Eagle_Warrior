##############################################################################
# Sections
##############################################################################
file(GLOB_RECURSE QT_MOC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS *.hpp)
QT4_WRAP_CPP(QT_MOC_HPP ${QT_MOC})
##############################################################################
# Sources
##############################################################################
file(GLOB_RECURSE QT_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS sks_vision_nui.cpp func.cpp)
file(GLOB_RECURSE QT_SOURCES_pub RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS pub_test.cpp func.cpp)
##############################################################################
# Binaries
##############################################################################

add_executable(sks_vision_nui sks_vision_nui.cpp ${QT_SOURCES} ${QT_MOC})
target_link_libraries(sks_vision_nui visionLib)#${catkin_LIBRARIES} ${OpenCV_LIBS}

add_executable(pub_test pub_test.cpp ${QT_SOURCES_pub} ${QT_MOC})
target_link_libraries(pub_test visionLib)

#add_executable(sks_vision_nui sks_vision_nui.cpp)
#target_link_libraries(sks_vision_nui ${catkin_LIBRARIES} ${OpenCV_LIBS})

#add_executable(pub_test pub_test.cpp)
#target_link_libraries(pub_test ${catkin_LIBRARIES} ${OpenCV_LIBS})
