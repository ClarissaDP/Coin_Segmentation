
cmake_minimum_required(VERSION 2.8)

project(2-trabalho)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

add_executable(segmentaMoeda segmentaMoeda.cxx)

target_link_libraries(segmentaMoeda ${ITK_LIBRARIES})