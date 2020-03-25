cmake_minimum_required(VERSION 3.15)
project(iz2 LANGUAGES C CXX)

# Enable all compiler checks
if ( CMAKE_COMPILER_IS_GNUCC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Werror -Wpedantic")
endif()
if ( MSVC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4")
endif()

add_subdirectory(third_party)
add_subdirectory(emailfilter)
add_subdirectory(src)

option(BUILD_TESTS "Build test programs" ON)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTS)
    include(CTest)
#    add_subdirectory(tests)
endif()
