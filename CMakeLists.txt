cmake_minimum_required(VERSION 3.5)

project(multiSort LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(sources
  src/async_task_runner.cpp
  src/async_task_runner.h
  src/command_line_args_handler.cpp
  src/command_line_args_handler.h
  src/main.cpp
)

add_executable(multiSort
  ${sources}
)

target_link_libraries(multiSort
  pthread
)
