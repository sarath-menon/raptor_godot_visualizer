cmake_minimum_required(VERSION 3.21)
project(MyLibrary)

set(CMAKE_CXX_STANDARD 17) # Feel free to use whatever standard you want

###############################################################################
# Load external packages
###############################################################################

find_package(fastcdr REQUIRED)
find_package(fastrtps REQUIRED)

add_subdirectory(visualizer_app)
add_subdirectory(communication_lib)
add_subdirectory(external)