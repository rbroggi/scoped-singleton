set(BINARY ${CMAKE_PROJECT_NAME})

add_library(${BINARY}_lib STATIC ScopedSingleton.hpp ScopedSingleton.cpp HTTP.h HTTP.cpp)