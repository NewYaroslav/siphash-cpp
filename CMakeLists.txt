cmake_minimum_required(VERSION 3.16)
project(siphash-hpp VERSION 1.0 LANGUAGES CXX)

option(BUILD_TESTING "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" OFF)

add_library(siphash_hpp INTERFACE)
target_include_directories(siphash_hpp INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/siphash-hpp>
    $<INSTALL_INTERFACE:include/siphash-hpp>)

include(CMakePackageConfigHelpers)

install(TARGETS siphash_hpp EXPORT siphash-hpp-targets)
install(DIRECTORY include/siphash-hpp/ DESTINATION include/siphash-hpp)

install(EXPORT siphash-hpp-targets
  FILE siphash-hpp-targets.cmake
  NAMESPACE siphash_hpp::
  DESTINATION lib/cmake/siphash-hpp)

configure_package_config_file(
  cmake/siphash-hpp-config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/siphash-hpp-config.cmake
  INSTALL_DESTINATION lib/cmake/siphash-hpp)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/siphash-hpp-config-version.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/siphash-hpp-config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/siphash-hpp-config-version.cmake
  DESTINATION lib/cmake/siphash-hpp)

if(BUILD_EXAMPLES)
  add_executable(siphash_example examples/basic.cpp)
  target_link_libraries(siphash_example PRIVATE siphash_hpp)
endif()

if(BUILD_TESTING)
  enable_testing()
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip)
  FetchContent_MakeAvailable(googletest)

  add_executable(siphash_tests tests/siphash_tests.cpp)
  target_link_libraries(siphash_tests PRIVATE siphash_hpp gtest_main)
  add_test(NAME siphash_tests COMMAND siphash_tests)
endif()
