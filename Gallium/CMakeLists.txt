cmake_minimum_required(VERSION 3.10)
project(OgreApp)
set(CMAKE_CXX_STANDARD 20)

find_package(PkgConfig REQUIRED)
pkg_check_modules(OGRE REQUIRED OGRE)

find_package(SDL2 REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
find_package(nlohmann_json REQUIRED)

find_library(OGRE_BITES_LIB NAMES OgreBites)
find_library(OGRE_RTSHADER_LIB NAMES OgreRTShaderSystem)

include_directories(
	epic/entity
	epic/entity/static
	epic/nature
	epic/physics
	epic/event
	epic/reflect
	epic/entity/movable
	/usr/local/include/bullet
	${SDL2_INCLUDE_DIRS}
	${Boost_INCLUDE_DIRS}
)
set(SRCS main.cpp)

add_executable(
	main ${SRCS} 
	epic/nature/Environment.cpp
	epic/physics/PhysicsManager.cpp
	epic/entity/movable/Character.cpp
)

target_include_directories(main PRIVATE ${OGRE_INCLUDE_DIRS})
target_link_libraries(main PRIVATE
	${SDL2_LIBRARIES}
	${OGRE_LIBRARIES}
	${OGRE_BITES_LIB}
	${OGRE_RTSHADER_LIB}
	BulletDynamics BulletCollision LinearMath
	OgreBullet
	yaml-cpp
	nlohmann_json::nlohmann_json
	${Boost_LIBRARIES}
	pthread
)

target_compile_options(main PRIVATE ${OGRE_CFLAGS_OTHER})