##############################################################################
# Sections
##############################################################################

set(QT_FORMS ${CMAKE_CURRENT_SOURCE_DIR}/ui/main_window.ui)
set(QT_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/resources/images.qrc)
file(GLOB_RECURSE QT_MOC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS *.hpp)

QT4_ADD_RESOURCES(QT_RESOURCES_CPP ${QT_RESOURCES})
QT4_WRAP_UI(QT_FORMS_HPP ${QT_FORMS})
QT4_WRAP_CPP(QT_MOC_HPP ${QT_MOC})

##############################################################################
# Sources
##############################################################################

file(GLOB_RECURSE QT_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS *.cpp)

##############################################################################
# Binaries
##############################################################################

add_executable(sks_ui ${QT_FORMS_HPP} ${QT_SOURCES} ${QT_RESOURCES_CPP}  ${QT_MOC_HPP})
#add_library(sks_ui_lib ${QT_SOURCES} ${QT_RESOURCES_CPP} ${QT_FORMS_HPP} ${QT_MOC_HPP})
target_link_libraries(sks_ui ${QT_LIBRARIES})
target_link_libraries(sks_ui ${catkin_LIBRARIES})
target_link_libraries(sks_ui qnode)

install(TARGETS sks_ui
  RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
)
