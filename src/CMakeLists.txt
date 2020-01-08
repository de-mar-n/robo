cmake_minimum_required( VERSION 2.8 )
project( exemplesOpencv )
set( OpenCV_FOUND 1)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
#-----------compilation step
add_executable( projet_voir.exe segment.cpp math_fct.cpp)
set(CMAKE_BUILD_TYPE Debug)
#-----------linking step
target_link_libraries( projet_voir.exe ${OpenCV_LIBS} )
