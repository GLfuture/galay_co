cmake_minimum_required(VERSION 3.25)
project(test)
add_compile_options(-fcoroutines)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)

set(SRC_LIST test.cc)
set(LIB_LIST galay_co)

include_directories(include)
add_subdirectory(src)

add_executable(test ${SRC_LIST})
target_link_libraries(test ${LIB_LIST})

