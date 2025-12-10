cmake_minimum_required(VERSION 3.10)
project(lab06_oop)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.0
  TLS_VERIFY false
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(${CMAKE_PROJECT_NAME}_lib
    src/npc.cpp
    src/dragon.cpp
    src/frog.cpp
    src/knight.cpp
    src/npc_factory.cpp
    src/file_npc_factory.cpp
    src/battle_visitor.cpp
    src/console_logger.cpp
    src/file_logger.cpp
    src/dungeon.cpp
    src/game.cpp
)

target_include_directories(${CMAKE_PROJECT_NAME}_lib PUBLIC include)

add_executable(${CMAKE_PROJECT_NAME}_exe src/main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME}_exe PRIVATE ${CMAKE_PROJECT_NAME}_lib)

add_executable(lab07 src/main_lab7.cpp)
target_link_libraries(lab07 PRIVATE ${CMAKE_PROJECT_NAME}_lib)
target_include_directories(lab07 PRIVATE include)

enable_testing()

add_executable(tests tests/tests.cpp)
target_link_libraries(tests PRIVATE ${CMAKE_PROJECT_NAME}_lib gtest_main)
target_include_directories(tests PUBLIC include)

add_test(NAME LabOOPTests COMMAND tests)
