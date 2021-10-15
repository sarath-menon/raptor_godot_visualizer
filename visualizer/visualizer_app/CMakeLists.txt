project(MyLibrary CXX)

add_library(MyLibrary SHARED
src/godot_library.cpp
src/controller.cpp
src/visualizer_publisher.cpp
src/visualizer_subscriber.cpp
)


target_link_libraries(MyLibrary PUBLIC 
godot-cpp
fastcdr 
fastrtps
# msg_lib
# mocap_fastdds_lib
comm_lib
)    

target_include_directories(MyLibrary PUBLIC include)                           