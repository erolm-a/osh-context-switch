cmake_minimum_required(VERSION 3.7)

project(Jit VERSION 0.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Jit WIN32 main.cc schedulers/roundrobin.cc schedulers/task.cc)

target_include_directories(Jit PRIVATE schedulers/)

find_package(Threads REQUIRED)
target_link_libraries(Jit Threads::Threads)

# target_compile_definitions(Jit PRIVATE -DVERBOSE)
