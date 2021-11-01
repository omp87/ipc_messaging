cmake_minimum_required (VERSION 3.17)
project (multi_process_messaging)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(host_process host_process.cpp)
target_link_libraries(host_process ${OpenCV_LIBS})

add_executable(remote_process remote_process.cpp)
target_link_libraries(remote_process ${OpenCV_LIBS})