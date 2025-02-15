cmake_minimum_required(VERSION 3.22)
project(datetimelib VERSION 1.0 LANGUAGES CXX)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Source files for the library
set(SOURCES
    src/datetimelib.cpp
)

# Header files (for installation and IDEs)
set(HEADERS
    include/datetimelib/types.hpp
    include/datetimelib/datetimelib.hpp
)

# Create static and shared libraries
add_library(datetimelib_static STATIC ${SOURCES})
add_library(datetimelib_shared SHARED ${SOURCES})

# Set include directories
# Set include directories properly for build and install
target_include_directories(datetimelib_static PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_include_directories(datetimelib_shared PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)


# Define output names
set_target_properties(datetimelib_static PROPERTIES OUTPUT_NAME datetimelib)
set_target_properties(datetimelib_shared PROPERTIES OUTPUT_NAME datetimelib)

# Installation rules
install(TARGETS datetimelib_static datetimelib_shared
    EXPORT datetimelibTargets  # Ensure targets are registered for export
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

# Install header files
install(FILES ${HEADERS} DESTINATION include/datetimelib)

# Export package for other CMake projects
include(CMakePackageConfigHelpers)

set(CONFIG_INSTALL_DIR lib/cmake/datetimelib)

install(EXPORT datetimelibTargets
    FILE datetimelibTargets.cmake
    NAMESPACE datetimelib::
    DESTINATION ${CONFIG_INSTALL_DIR}
)

configure_package_config_file(
    cmake/datetimelibConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/datetimelibConfig.cmake
    INSTALL_DESTINATION ${CONFIG_INSTALL_DIR}
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/datetimelibConfig.cmake
    DESTINATION ${CONFIG_INSTALL_DIR}
)

