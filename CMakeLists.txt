cmake_minimum_required(VERSION 3.14)
project(RobotMovement VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler warning flags
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Core library targets
add_library(robot_movement_lib
    src/Grid.cpp
    src/Bresenham.cpp
    src/RobotController.cpp
    src/Command.cpp
    src/CommandParser.cpp
)

target_include_directories(robot_movement_lib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Main executable
add_executable(robot_movement src/main.cpp)
target_link_libraries(robot_movement PRIVATE robot_movement_lib)

# Enable testing
enable_testing()
add_executable(robot_movement_tests tests/test_main.cpp)
target_link_libraries(robot_movement_tests PRIVATE robot_movement_lib)

add_test(NAME RobotMovementTests COMMAND robot_movement_tests)
