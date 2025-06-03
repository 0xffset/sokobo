# Ensure CMAKE_MODULE_PATH is initialized before use
if(NOT DEFINED CMAKE_MODULE_PATH)
  set(CMAKE_MODULE_PATH "")
endif()

cmake_minimum_required(VERSION 3.14)

project(sokoboTests LANGUAGES CXX)

# ---- Dependencies ----
include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.4  
)

FetchContent_MakeAvailable(Catch2)

include(Catch)

# ---- Tests ----
add_executable(sokobo_test source/sokobo_test.cpp)
target_link_libraries(
    sokobo_test PRIVATE
    sokobo_lib
    Catch2::Catch2WithMain
)
target_compile_features(sokobo_test PRIVATE cxx_std_17)

# catch_discover_tests(sokobo_test)

# ---- End-of-file commands ----
add_folders(Test)
