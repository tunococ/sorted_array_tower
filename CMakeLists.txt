cmake_minimum_required(VERSION 4.3.2)
project(sorted_array_tower
  VERSION 1.0.0
  DESCRIPTION "A C++ library for efficient sorted array operations"
  LANGUAGES CXX
)

# Set C++ standard
if(NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Option for building with modules (C++20 modules support)
option(SAT_USE_MODULES "Use C++20 modules instead of headers" OFF)

if(SAT_USE_MODULES)
  if(NOT CMAKE_GENERATOR STREQUAL "Ninja" AND NOT CMAKE_GENERATOR STREQUAL "Ninja Multi-Config" AND NOT CMAKE_GENERATOR MATCHES "Visual Studio")
    message(WARNING "C++ modules are not supported by generator ${CMAKE_GENERATOR}; falling back to header-only mode")
    set(SAT_USE_MODULES OFF CACHE BOOL "Use C++20 modules instead of headers" FORCE)
  endif()
endif()

# Include Conan generated files
if(EXISTS "${CMAKE_BINARY_DIR}/conan_toolchain.cmake")
  include("${CMAKE_BINARY_DIR}/conan_toolchain.cmake")
endif()

# Find Conan-provided packages
find_package(doctest QUIET)

# Main library target
if(SAT_USE_MODULES)
  add_library(sorted_array_tower STATIC)
else()
  add_library(sorted_array_tower INTERFACE)
endif()

if(SAT_USE_MODULES)
  # Module-based implementation (C++20)
  target_sources(sorted_array_tower
    PUBLIC
      FILE_SET CXX_MODULES
        FILES src/sorted_array_tower.cppm
      FILE_SET HEADERS
        FILES include/sorted_array_tower/sorted_array_tower.hpp
        BASE_DIRS include
  )
  target_compile_features(sorted_array_tower PUBLIC cxx_std_20)
  set(SAT_TARGET_VISIBILITY PUBLIC)
else()
  # Header-only implementation
  target_sources(sorted_array_tower
    INTERFACE
      FILE_SET headers
        TYPE HEADERS
        FILES include/sorted_array_tower/sorted_array_tower.hpp
  )
  set(SAT_TARGET_VISIBILITY INTERFACE)
endif()

# Set include directories
target_include_directories(sorted_array_tower
  ${SAT_TARGET_VISIBILITY}
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Enable testing
enable_testing()

# Add test executable
if(doctest_FOUND)
  add_executable(test_sorted_array_tower
    tests/test_sorted_array_tower.cpp
  )

  if(SAT_USE_MODULES)
    target_compile_definitions(test_sorted_array_tower
      PRIVATE
        SAT_USE_MODULES
    )
  endif()

  target_link_libraries(test_sorted_array_tower
    PRIVATE
      sorted_array_tower
      doctest::doctest
  )

  target_include_directories(test_sorted_array_tower
    PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

  add_test(
    NAME sorted_array_tests
    COMMAND test_sorted_array_tower
  )
else()
  message(WARNING "doctest not found - tests will not be built")
endif()

# Code coverage support (only when this is the top-level project)
if(PROJECT_IS_TOP_LEVEL)
  option(SAT_ENABLE_COVERAGE "Enable code coverage instrumentation" OFF)

  if(SAT_ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
      if(doctest_FOUND)
        target_compile_options(test_sorted_array_tower PRIVATE --coverage -O0 -g)
        target_link_options(test_sorted_array_tower PRIVATE --coverage)

        find_program(GCOVR gcovr)

        if(GCOVR)
          if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            find_program(SAT_LLVM_COV llvm-cov)
            if(SAT_LLVM_COV)
              set(SAT_GCOV_EXECUTABLE "${SAT_LLVM_COV} gcov")
            else()
              message(WARNING "llvm-cov not found — coverage target will not be created")
              set(SAT_ENABLE_COVERAGE OFF)
            endif()
          else()
            set(SAT_GCOV_EXECUTABLE gcov)
          endif()

          if(SAT_ENABLE_COVERAGE)
            set(SAT_BUILD_DIR "$<TARGET_FILE_DIR:test_sorted_array_tower>")
            add_custom_target(coverage
              COMMAND $<TARGET_FILE:test_sorted_array_tower>
              COMMAND ${CMAKE_COMMAND} -E make_directory "${SAT_BUILD_DIR}/coverage_report"
              COMMAND ${GCOVR} --root ${CMAKE_SOURCE_DIR}
                --txt
                "${SAT_BUILD_DIR}/coverage_report/report.txt"
                --html-details
                "${SAT_BUILD_DIR}/coverage_report/index.html"
                --cobertura
                "${SAT_BUILD_DIR}/coverage_report/cobertura.xml"
                --markdown
                "${SAT_BUILD_DIR}/coverage_report/summary.md"
                --exclude "${CMAKE_SOURCE_DIR}/tests/.*"
                --gcov-executable "${SAT_GCOV_EXECUTABLE}"
                --txt-summary >
                "${SAT_BUILD_DIR}/coverage_report/summary.txt"
              WORKING_DIRECTORY "${SAT_BUILD_DIR}"
              COMMENT "Generating code coverage report in ${SAT_BUILD_DIR}/coverage_report"
            )
          endif()
        else()
          message(WARNING "gcovr not found — coverage target will not be created")
        endif()
      endif()
    elseif(MSVC)
      message(WARNING "Code coverage is not supported with MSVC")
    else()
      message(WARNING "Code coverage is not supported with ${CMAKE_CXX_COMPILER_ID}")
    endif()
  endif()
endif()

# Installation rules
install(TARGETS sorted_array_tower
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY include/sorted_array_tower
  DESTINATION include
)