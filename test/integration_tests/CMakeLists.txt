# Add each .cpp file as a separate executable
file(GLOB_RECURSE TEST_SOURCES "*.cpp")

# Add include directory for NanoLog
include_directories(${CMAKE_SOURCE_DIR}/third_party/NanoLog/runtime
                    ${CMAKE_BINARY_DIR})

foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    
    # Link against NanoLog library, pthreads, and POSIX AIO
    target_link_libraries(${TEST_NAME} GTest::GTest GTest::Main 
                          ${CMAKE_SOURCE_DIR}/third_party/NanoLog/runtime/libNanoLog.a
                          pthread rt)
    
    # Set required compile flags, including format check
    target_compile_options(${TEST_NAME} PRIVATE -Werror=format)
endforeach()

# Set optimization and debug flags
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
