cmake_minimum_required(VERSION 3.5)

project(TankServer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Network REQUIRED)

add_executable(TankServer
    main.cpp
    Server.cpp
    Server.h
)

target_link_libraries(ServerApp Qt6::Core Qt6::Network)
