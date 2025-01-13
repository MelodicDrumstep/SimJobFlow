cmake_minimum_required(VERSION 3.10)
project(SimJobFlow)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

set(GTEST_ROOT "/dssg/home/acct-hpc/asc/wltu/googletest")
set(GTEST_INCLUDE_DIR "${GTEST_ROOT}/googletest/include")
set(GTEST_LIBRARY "${GTEST_ROOT}/build/lib/libgtest.a")
set(GTEST_MAIN_LIBRARY "${GTEST_ROOT}/build/lib/libgtest_main.a")

include_directories(${GTEST_INCLUDE_DIR})
link_directories("${GTEST_ROOT}/build/lib")

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

# Add third_party/json directory to include path
include_directories(${CMAKE_SOURCE_DIR}/third_party/json/include)

# Add src directory and its subdirectories to include path
include_directories(${CMAKE_SOURCE_DIR}/src/input_handler)
include_directories(${CMAKE_SOURCE_DIR}/src/scheduler)
include_directories(${CMAKE_SOURCE_DIR}/src/OPT_solver)
include_directories(${CMAKE_SOURCE_DIR}/src/scheduler/greedy)
include_directories(${CMAKE_SOURCE_DIR}/src/timer)
include_directories(${CMAKE_SOURCE_DIR}/src/visualization)
include_directories(${CMAKE_SOURCE_DIR}/src/output_handler)
include_directories(${CMAKE_SOURCE_DIR}/src/simjobflow)
include_directories(${CMAKE_SOURCE_DIR}/src/util)
include_directories(${CMAKE_SOURCE_DIR}/src/util/machine)
include_directories(${CMAKE_SOURCE_DIR}/src/util/job)
include_directories(${CMAKE_SOURCE_DIR}/src/util/model)
include_directories(${CMAKE_SOURCE_DIR}/src/util/objective)
include_directories(${CMAKE_SOURCE_DIR}/src/util/exception)

# Add subdirectories
add_subdirectory(test)
add_subdirectory(app)
