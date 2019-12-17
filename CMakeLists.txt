cmake_minimum_required(VERSION 3.10)
project(SimMOS)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib/pipeline)
include_directories(lib/pipeline/src)

add_executable(SimMOS src/main.cpp src/logic_level.hpp src/merge_node.cpp src/merge_node.hpp src/pmos.cpp src/pmos.hpp src/node_base.cpp src/node_base.hpp src/nmos.cpp src/nmos.hpp src/global_input.cpp src/global_input.hpp)

target_link_libraries(SimMOS pipeline)
