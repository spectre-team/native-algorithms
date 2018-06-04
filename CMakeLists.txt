cmake_minimum_required(VERSION 3.0)
project(spectre_native_algorithms CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/finders")
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/configs")

find_package(OpenMP REQUIRED)
find_package(OpenCV REQUIRED core ml)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if (result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif ()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if (result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif ()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
include_directories(BEFORE SYSTEM
  "${gtest_SOURCE_DIR}/include" "${gmock_SOURCE_DIR}/include")

include(CheckIncludeFileCXX)
check_include_file_cxx(/usr/include/gsl/gsl HAVE_GSL)
if (NOT HAVE_GSL)
    include_directories(dep/gsl/include)
    message(STATUS "Using GSL includes from dep/")
endif ()

include_directories(src)
enable_testing()

add_subdirectory(src/Spectre.libClassifier)
add_subdirectory(src/Spectre.libClassifier.Tests)
add_subdirectory(src/Spectre.libClustering)
add_subdirectory(src/Spectre.libClustering.Tests)
add_subdirectory(src/Spectre.libDataset)
add_subdirectory(src/Spectre.libDataset.Tests)
add_subdirectory(src/Spectre.libDivik)
add_subdirectory(src/Spectre.libException)
add_subdirectory(src/Spectre.libFunctional)
add_subdirectory(src/Spectre.libFunctional.Tests)
add_subdirectory(src/Spectre.libGaClassifier)
add_subdirectory(src/Spectre.libGaClassifier.Tests)
add_subdirectory(src/Spectre.libGaussianMixtureModelling)
add_subdirectory(src/Spectre.libGaussianMixtureModelling.Tests)
add_subdirectory(src/Spectre.libGenetic)
add_subdirectory(src/Spectre.libGenetic.Tests)
add_subdirectory(src/Spectre.libHeatmapDataScaling)
add_subdirectory(src/Spectre.libPeakFinder)
add_subdirectory(src/Spectre.libPeakFinder.Tests)
add_subdirectory(src/Spectre.libStatistics)
add_subdirectory(src/Spectre.libStatistics.Tests)
add_subdirectory(src/Spectre.libWavelet)
add_subdirectory(src/Spectre.libWavelet.Tests)
