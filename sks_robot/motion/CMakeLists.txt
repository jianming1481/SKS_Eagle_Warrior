
#file(GLOB_RECURSE CSSL_H RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/cssl FOLLOW_SYMLINKS *.h)
#file(GLOB_RECURSE CSSL_C RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/cssl FOLLOW_SYMLINKS *.c)

add_library(csslLib cssl/cssl.h cssl/cssl.c cssl/uty.c)



##############################################################################
# Binaries
##############################################################################

#add_executable(motion motion.cpp cssl/cssl.h cssl/cssl.c cssl/uty.c)
add_executable(motion motion.cpp MotorControl.h)
target_link_libraries(motion ${catkin_LIBRARIES})
target_link_libraries(motion qnode)
target_link_libraries(motion csslLib)

