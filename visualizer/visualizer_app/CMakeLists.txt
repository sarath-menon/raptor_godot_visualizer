project(MyLibrary CXX)

add_library(MyLibrary SHARED
src/godot_library.cpp
src/controller.cpp
)


target_link_libraries(MyLibrary PUBLIC 
godot-cpp
fastcdr 
fastrtps
comm_lib
)    

target_include_directories(MyLibrary PUBLIC include)                           