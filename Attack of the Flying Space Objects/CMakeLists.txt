cmake_minimum_required(VERSION 3.10)
project(Asteroids)

# Set the C++ standard to C++11
set(CMAKE_CXX_STANDARD 11)

# Add your source files
set(SOURCES
    main.cpp
    olcConsoleGameEngine.h
)

# Include directory containing your game engine header file
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Add the executable target
add_executable(${PROJECT_NAME} ${SOURCES})
