
cmake_minimum_required(VERSION 2.8)

project(detectaMoeda)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

add_executable(detectaMoeda_completo detectaMoeda_completo.cxx)

target_link_libraries(detectaMoeda_completo ${ITK_LIBRARIES})
