cmake_minimum_required(VERSION 3.12)
project(concurrent_make)

set(CMAKE_CXX_STANDARD 17)

FIND_PACKAGE(libssh REQUIRED)
FIND_PACKAGE(Boost COMPONENTS system filesystem REQUIRED)
FIND_PACKAGE(Threads REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(SSH_Node)

add_executable(ccmake_node node_daemon/background_commun.cpp node_daemon/background_commun.h node_daemon/requests.cpp node_daemon/requests.h node_daemon/sys_spec.cpp node_daemon/sys_spec.h)
target_link_libraries(ccmake_node ${CMAKE_THREAD_LIBS_INIT} ${Boost_SYSTEM_LIBRARIES} ${Boost_FILESYSTEM_LIBRARIES})

add_executable(concurrent_make main.cpp SSH_Node/Scp.cpp SSH_Node/Node.cpp dependecies_parser.cpp ccmake.cpp node_daemon/sys_spec.cpp node_daemon/requests.cpp)
target_link_libraries(concurrent_make ${CMAKE_THREAD_LIBS_INIT} ${Boost_SYSTEM_LIBRARIES} ${Boost_FILESYSTEM_LIBRARIES})
target_link_libraries(concurrent_make -lssh -lssh_threads stdc++fs)


