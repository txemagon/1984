cmake_minimum_required(VERSION 2.8)
project( reconocimiento_facial )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( reconocimiento_facial reconocimiento_facial.cpp )
target_link_libraries( reconocimiento_facial ${OpenCV_LIBS} )
