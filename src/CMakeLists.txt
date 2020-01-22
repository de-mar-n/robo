cmake_minimum_required( VERSION 2.8 )
project( RoboPosture )
set( OpenCV_FOUND 1 )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
#-----------compilation step
add_executable( robo_posture.exe segment.cpp math_fct.cpp custom.cpp diagram.cpp)
set(CMAKE_BUILD_TYPE Debug)
#-----------linking step
target_link_libraries( robo_posture.exe ${OpenCV_LIBS} )
