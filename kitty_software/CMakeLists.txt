cmake_minimum_required(VERSION 3.0.2)
project(kitty_software)

find_package(Threads REQUIRED)
find_package( Boost 1.7 COMPONENTS system REQUIRED )
if(Boost_FOUND)
    # message ("Boost found")
else()
    message (FATAL_ERROR "Cannot find Boost")
endif()

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  realtime_tools
)

catkin_package(
  CATKIN_DEPENDS message_runtime
)

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIR}
  include
)




