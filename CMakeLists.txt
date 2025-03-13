cmake_minimum_required(VERSION 3.30)
project(datetimelib VERSION 1.0.0 LANGUAGES CXX)

# Include FetchContent for downloading dependencies
include(FetchContent)

# Specify C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add minimum compiler version check
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 16)
        message(FATAL_ERROR "Clang version must be at least 16!")
    endif()
endif()

# Add library source files
set(SOURCES
    src/datetimelib.cpp
)

# Add library headers
set(HEADERS
    include/datetimelib/datetimelib.hpp
)

# Create static library
add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
target_compile_definitions(${PROJECT_NAME} PRIVATE
    _GLIBCXX_USE_CXX11_ABI=1
)

# Set properties for both libraries
foreach(target ${PROJECT_NAME})
    target_include_directories(${target}
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
    )
endforeach()

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
)

# Add unit test executable
add_executable(unit src/unit.cpp)
target_include_directories(unit PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/test
)
target_link_libraries(unit
    PRIVATE
        ${PROJECT_NAME}
)

# Force CTest to regenerate test files
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS 
    "${CMAKE_CURRENT_BINARY_DIR}/Testing/Temporary/LastTest.log"
)

# Enable testing
enable_testing()
add_test(NAME unit COMMAND unit)

# Installation commands
include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

# Export targets
install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# Create and install config file
include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# Generate version file
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Install config files
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
