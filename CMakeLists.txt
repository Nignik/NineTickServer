cmake_minimum_required(VERSION 3.29 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project("NineTickServer")

add_subdirectory("proto_messages")
add_subdirectory("src")