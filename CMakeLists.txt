cmake_minimum_required(VERSION 3.10) # Specify the minimum version for CMake

project(img) # Define the project name

# Add all the source files needed to build the executable
add_executable(${PROJECT_NAME} input_handle.cc file_parser.cc image_handle.cc main.cc round_to_int.cc util.cc)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# If you're using SDL2, you might need to find the package and link it
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

# If you're using other libraries, you can find and link them in a similar way
