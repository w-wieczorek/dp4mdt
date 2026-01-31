cmake_minimum_required(VERSION 3.15)
project(dp4mdt CXX)

find_package(immer REQUIRED CONFIG)
find_package(CLI11 REQUIRED)

add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/read_data.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE immer::immer)
target_link_libraries(${PROJECT_NAME} PRIVATE CLI11::CLI11)
