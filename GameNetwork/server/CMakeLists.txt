cmake_minimum_required(VERSION 3.10) 
project(BoostAsioExample)
set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIRS})
add_executable(server server.cpp GameServer.cpp) 
target_link_libraries(server ${Boost_LIBRARIES} pthread)
