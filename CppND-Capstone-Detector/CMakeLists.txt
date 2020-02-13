cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17 -pthread")

find_package(OpenCV 3.4 REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})
add_definitions(${OpenCV_DEFINITIONS})

project(HelloWorld)

# Find all executables
file(GLOB project_SRCS src/*.cpp) #src/*.h

add_executable(HelloWorld ${project_SRCS})
target_link_libraries(HelloWorld ${OpenCV_LIBRARIES})
