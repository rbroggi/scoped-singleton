# 'Google_test' is the subproject name
project(Google_tests)

add_subdirectory(googletest-release-1.11.0)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
