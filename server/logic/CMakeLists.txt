cmake_minimum_required(VERSION 3.25)
project(logic)

set(CMAKE_CXX_STANDARD 17)
FIND_PACKAGE( OpenCV REQUIRED )

add_executable(logic nonogram_example.cpp)

include_directories( ${OpenCV_INCLUDE_DIRS} )
link_directories( ${OpenCV_LIB_DIR} )
target_link_libraries( logic ${OpenCV_LIBS} )

