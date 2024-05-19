cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(takeoff_and_land)

add_executable(takeoff_and_land
    takeoff_and_land.cpp
)

add_executable(tflite_client
    tflite_client.cpp
)

find_package(MAVSDK REQUIRED)

target_link_libraries(takeoff_and_land
    MAVSDK::mavsdk
    /usr/lib64/libmodal_pipe.so
    /usr/lib64/libmodal_json.so)

target_link_libraries(tflite_client
    MAVSDK::mavsdk
    /usr/lib64/libmodal_pipe.so
    /usr/lib64/libmodal_json.so)

if(NOT MSVC)
    add_compile_options(takeoff_and_land PRIVATE -Wall -Wextra)
else()
    add_compile_options(takeoff_and_land PRIVATE -WX -W2)
endif()

if(NOT MSVC)
    add_compile_options(tflite_client PRIVATE -Wall -Wextra)
else()
    add_compile_options(tflite_client PRIVATE -WX -W2)
endif()
