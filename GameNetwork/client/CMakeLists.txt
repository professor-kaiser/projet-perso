cmake_minimum_required(VERSION 3.10) 
project(BoostAsioExample)
set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIRS})
add_executable(client client.cpp) 
target_link_libraries(client ${Boost_LIBRARIES} pthread)
