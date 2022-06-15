cmake_minimum_required(VERSION 3.1)

project(perception)
set(TARGET perception)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR x86_64)
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-std=c++14 ${CMAKE_CXX_FLAGS}")

set(OpenCV_DIR /home/chenlai/apps/opencv4/lib/cmake/opencv4)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIR})
link_directories(${OpenCV_LIBRARIES})

find_package(Protobuf REQUIRED)
link_directories(${Protobuf_INCLUDE_DIRS})

include_directories(${CMAKE_SOURCE_DIR}/common)
include_directories(${CMAKE_SOURCE_DIR}/ego)
include_directories(${CMAKE_SOURCE_DIR}/files)
include_directories(${CMAKE_SOURCE_DIR}/interface)
include_directories(${CMAKE_SOURCE_DIR}/lanes)
include_directories(${CMAKE_SOURCE_DIR}/obstacles)
include_directories(${CMAKE_SOURCE_DIR}/process)
include_directories(${CMAKE_SOURCE_DIR}/sensors)

# aux_source_directory(${CMAKE_SOURCE_DIR}/common PERCEPTION_SRC)
aux_source_directory(${CMAKE_SOURCE_DIR}/ego PERCEPTION_SRC)
aux_source_directory(${CMAKE_SOURCE_DIR}/files PERCEPTION_SRC)
aux_source_directory(${CMAKE_SOURCE_DIR}/interface PERCEPTION_SRC)
aux_source_directory(${CMAKE_SOURCE_DIR}/sensors PERCEPTION_SRC)
aux_source_directory(${CMAKE_SOURCE_DIR}/obstacles PERCEPTION_SRC)
aux_source_directory(${CMAKE_SOURCE_DIR}/process PERCEPTION_SRC)
aux_source_directory(${CMAKE_SOURCE_DIR}/lanes PERCEPTION_SRC)

add_executable(${TARGET} ${PERCEPTION_SRC} main.cpp)

target_link_libraries(${TARGET} PRIVATE ${OpenCV_LIBRARIES} ${Protobuf_LIBRARIES})
