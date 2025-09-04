cmake_minimum_required(VERSION 3.16)
project(siphash_cpp VERSION 1.0.0 LANGUAGES CXX)

option(BUILD_TESTING "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" OFF)

add_library(siphash_cpp INTERFACE)
target_include_directories(siphash_cpp INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/siphash_cpp>
    $<INSTALL_INTERFACE:include/siphash_cpp>)
target_compile_features(siphash_cpp INTERFACE cxx_std_11)

include(CMakePackageConfigHelpers)

install(TARGETS siphash_cpp EXPORT siphash_cpp-targets)
install(DIRECTORY include/siphash_cpp/ DESTINATION include/siphash_cpp)

install(EXPORT siphash_cpp-targets
  FILE siphash_cpp-targets.cmake
  NAMESPACE siphash_cpp::
  DESTINATION lib/cmake/siphash_cpp)

configure_package_config_file(
  cmake/siphash_cpp-config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/siphash_cpp-config.cmake
  INSTALL_DESTINATION lib/cmake/siphash_cpp)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/siphash_cpp-config-version.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/siphash_cpp-config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/siphash_cpp-config-version.cmake
  DESTINATION lib/cmake/siphash_cpp)

if(BUILD_EXAMPLES)
  add_executable(siphash_example examples/basic.cpp)
  target_link_libraries(siphash_example PRIVATE siphash_cpp)
endif()

if(BUILD_TESTING)
  enable_testing()
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip)
  FetchContent_MakeAvailable(googletest)

  add_executable(siphash_tests tests/siphash_tests.cpp)
    target_link_libraries(siphash_tests PRIVATE siphash_cpp gtest_main)
  add_test(NAME siphash_tests COMMAND siphash_tests)
endif()
