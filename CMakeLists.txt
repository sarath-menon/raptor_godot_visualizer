cmake_minimum_required(VERSION 3.0.0)
project(quad_sim VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17) # Feel free to use whatever standard you want

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/${ARCHITECTURE}")
add_subdirectory(godot-cpp)
include_directories(include)

add_library(${PROJECT_NAME} SHARED
  src/controller.cpp
  src/gdlibrary.cpp) 
     
target_link_libraries(${PROJECT_NAME} PUBLIC godot-cpp)

set_target_properties(${PROJECT_NAME}
  PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/quad_sim_godot/bin/osx
  POSITION_INDEPENDENT_CODE ON)
