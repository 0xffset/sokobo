# Ensure CMAKE_MODULE_PATH is initialized before use
set(CMAKE_ERROR_DEPRECATED OFF)
if(NOT DEFINED CMAKE_MODULE_PATH)
  set(CMAKE_MODULE_PATH "")
endif()

cmake_minimum_required(VERSION 3.14)

# include(cmake/prelude.cmake)

project(
    sokobo
    VERSION 0.1.0
    DESCRIPTION "C++ Computational Algebra CLI"
    HOMEPAGE_URL ""
    LANGUAGES CXX
)

# include(cmake/project-is-top-level.cmake)
# include(cmake/variables.cmake)

# ---- Declare library ----

#add_library(
#    sokobo_lib OBJECT
#    source/lib.cpp
#)
#
#target_include_directories(
#    sokobo_lib ${warning_guard}
#    PUBLIC
#    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
#)
#
#target_compile_features(sokobo_lib PUBLIC cxx_std_17)

# include(FetchContent)

# FetchContent_Declare(
#   fmt
#   GIT_REPOSITORY https://github.com/fmtlib/fmt.git
#   GIT_TAG 10.2.1 
# )

# FetchContent_MakeAvailable(fmt)

#target_link_libraries(sokobo_lib PRIVATE fmt::fmt)


# ---- Declare executable ----

add_executable(sokobo_exe source/main.cpp source/cli.cpp source/differential_equations.cpp source/numerical_methods.cpp source/fourier.cpp  source/calculus.cpp source/polynomial.cpp source/complex_number.cpp source/expression.cpp  source/laplace.cpp  source/matrix.cpp)
add_executable(sokobo::exe ALIAS sokobo_exe)
add_executable(calculus "source/calculus.cpp" "source/include/calculus.h")
add_executable(complex_number "source/complex_number.cpp" "source/include/complex_number.h")
add_executable(differential_equations "source/differential_equations.cpp" "source/include/differential_equations.h")
add_executable(expression "source/expression.cpp" "source/include/expression.h")
add_executable(fourier "source/fourier.cpp" "source/include/fourier.h")
add_executable(laplace "source/laplace.cpp" "source/include/laplace.h" )
add_executable(numerical_methods "source/numerical_methods.cpp" "source/include/numerical_methods.h")
add_executable(polynomial "source/polynomial.cpp" "source/include/polynomial.h")
add_executable(matrix "source/matrix.cpp" "source/include/matrix.h"
        test/matrix_test.cpp)
add_executable(cli "source/cli.cpp" "source/include/cli.h")

set_property(TARGET sokobo_exe PROPERTY OUTPUT_NAME sokobo)

target_compile_features(sokobo_exe PRIVATE cxx_std_17)
#target_include_directories(sokobo_lib INTERFACE ${CMAKE_SOURCE_DIR}/source)

#target_link_libraries(sokobo_exe PRIVATE sokobo_lib)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  # include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT sokobo_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of sokobo"
  )
endif()

# include(cmake/dev-mode.cmake)
