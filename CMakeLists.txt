cmake_minimum_required(VERSION 2.8)
project(boostTest)

find_package(Boost 1.57)

add_library(simpleLogger simpleLogger.cpp)
target_link_libraries(simpleLogger boost_log boost_log_setup boost_thread boost_system pthread)

add_executable(app app.cpp)
target_link_libraries(app simpleLogger)
