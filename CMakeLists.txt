cmake_minimum_required(VERSION 3.10)
project(soa VERSION 0.1.0 LANGUAGES CXX)

# Boost header dependencies
find_package(Boost REQUIRED)

# Add library target
add_library(soa INTERFACE)
add_library(soa::soa ALIAS soa)

# Configure library target
target_include_directories(
    soa
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>  
    $<INSTALL_INTERFACE:include/>)
target_compile_features(soa INTERFACE cxx_std_17)
target_link_libraries(soa INTERFACE Boost::boost)

# Add tests
enable_testing()
add_subdirectory(test)

# Install
include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/soa)

install(
    TARGETS soa
    EXPORT soa-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
    EXPORT soa-targets
    FILE soaTargets.cmake
    NAMESPACE soa::
    DESTINATION ${INSTALL_CONFIGDIR})

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/soaConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion)

configure_package_config_file(
    ${CMAKE_CURRENT_LIST_DIR}/cmake/soaConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/soaConfig.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR})

install(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/soaConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/soaConfigVersion.cmake
    DESTINATION ${INSTALL_CONFIGDIR})

export(
    EXPORT soa-targets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/soaTargets.cmake
    NAMESPACE soa::)

export(PACKAGE soa)
