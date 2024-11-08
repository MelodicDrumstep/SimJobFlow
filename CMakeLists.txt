cmake_minimum_required(VERSION 3.10)
project(SimJobFlow)

set(CMAKE_CXX_STANDARD 20)

# Find Google Test
find_package(GTest REQUIRED)

# Add third_party/json directory to include path
include_directories(${CMAKE_SOURCE_DIR}/third_party/json/include)

# Add src directory and its subdirectories to include path
include_directories(${CMAKE_SOURCE_DIR}/src/input_handler)
include_directories(${CMAKE_SOURCE_DIR}/src/machine)
include_directories(${CMAKE_SOURCE_DIR}/src/objective)
include_directories(${CMAKE_SOURCE_DIR}/src/scheduler)
include_directories(${CMAKE_SOURCE_DIR}/src/timer)
include_directories(${CMAKE_SOURCE_DIR}/src/visualization)
include_directories(${CMAKE_SOURCE_DIR}/src/job)
include_directories(${CMAKE_SOURCE_DIR}/src/model)
include_directories(${CMAKE_SOURCE_DIR}/src/output_handler)
include_directories(${CMAKE_SOURCE_DIR}/src/simjobflow)
include_directories(${CMAKE_SOURCE_DIR}/src/util)

# Add subdirectoriesa
add_subdirectory(test)