cmake_minimum_required(VERSION 3.13)
project(OBDOnBoard)

set(CMAKE_CXX_STANDARD 17)

find_package(Curses REQUIRED)

include_directories(${CURSES_INCLUDE_DIR})
add_executable(OBDOnBoard main.cpp rpm.cpp serialCom.cpp serialCom.h)
target_link_libraries(OBDOnBoard ${CURSES_LIBRARIES})