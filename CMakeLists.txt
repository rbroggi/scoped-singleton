cmake_minimum_required(VERSION 3.19)
project(ScopedSingleton)

set(CMAKE_CXX_STANDARD 17)

# jsoncons include
include_directories(src)
include_directories("vendor/jsoncons-0.165.0/include")

add_subdirectory(vendor/googletest-release-1.11.0)
add_subdirectory(src)
add_subdirectory(tests)


# adding the Google_Tests_run target
#add_executable(Google_Tests_run tests/ScopedSingletonTest.cpp)
# linking Google_Tests_run with DateConverter_lib which will be tested
#target_link_libraries(Google_Tests_run scoped_singleton)
#target_link_libraries(Google_Tests_run gtest gtest_main)
#target_include_directories(Google_Tests_run include)
