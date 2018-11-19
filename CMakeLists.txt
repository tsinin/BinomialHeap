cmake_minimum_required(VERSION 3.12)
project(binomialHeap)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(google-tests/googletest-master)
include_directories(google-tests/googletest-master/googletest/include)
include_directories(google-tests/googletest-master/googlemock/include)


add_executable(binomialHeap main.cpp Tests/Vector.h Tests/binomialHeap.h Tests/test.cpp)
target_link_libraries(binomialHeap gtest gtest_main)