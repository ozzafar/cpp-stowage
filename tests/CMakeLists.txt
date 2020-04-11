
project(tests)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(tests_run ReadInputTest.cpp)
target_link_libraries(tests_run gtest gtest_main)
