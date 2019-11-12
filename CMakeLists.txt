cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(sqlite-cpp)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-O2 -Wall")

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/SQLiteCpp)

include_directories(
  ${CMAKE_CURRENT_LIST_DIR}/thirdparty/SQLiteCpp/include
)

add_executable(a.out main.cpp)

target_link_libraries(a.out
  SQLiteCpp
  sqlite3
  pthread
  dl
  )
