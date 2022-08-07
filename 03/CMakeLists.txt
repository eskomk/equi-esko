# cmake_minimum_required (VERSION 2.6)

cmake_minimum_required(VERSION 3.14)
# project(my_project)
project (eiger03)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

FetchContent_MakeAvailable(googletest)

add_library(Eiger_lib Eiger.cpp)

add_executable(eiger03 eiger03.cpp)

target_link_libraries(
  eiger03
  Eiger_lib
  )

enable_testing()

add_executable(
  eiger_test
  eiger_test.cpp
)
target_link_libraries(
  eiger_test
  GTest::gtest_main
  Eiger_lib
)

include(GoogleTest)
gtest_discover_tests(eiger_test)
