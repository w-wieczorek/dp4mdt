cmake_minimum_required(VERSION 3.15)
project(dp4mdt CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(immer REQUIRED CONFIG)
find_package(CLI11 REQUIRED)
find_package(scippp REQUIRED CONFIG)
find_package(spdlog REQUIRED)

add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/read_data.cpp
    src/mts_ilp.cpp
    src/dtree.cpp
    src/mdt_dp.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE immer::immer)
target_link_libraries(${PROJECT_NAME} PRIVATE CLI11::CLI11)
target_link_libraries(${PROJECT_NAME} PRIVATE scippp::scippp)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)
