cmake_minimum_required(VERSION 3.14)

project(rtweekend VERSION 1.0)

find_package(CImg CONFIG REQUIRED)

add_executable(RayTracing main.cpp)

target_link_libraries(RayTracing PRIVATE CImg::CImg)