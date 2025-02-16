cmake_minimum_required(VERSION 3.20)
project(datetimelib VERSION 1.0.0 LANGUAGES CXX)

# Include FetchContent for downloading dependencies
include(FetchContent)

# Fetch spdlog
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.12.0  # Specify a version tag
)
FetchContent_MakeAvailable(spdlog)

# Specify C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable position independent code for shared libraries
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Platform-specific compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-D_GLIBCXX_USE_CXX11_ABI=1)
    
    if(APPLE)
        # Add system include paths for macOS
        include_directories(SYSTEM
            /usr/local/include
            /usr/include
            /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include
        )
        
        # Force include cstdio before other system headers
        add_compile_options(-include cstdio)
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

# Create shared library
add_library(${PROJECT_NAME}_shared SHARED ${SOURCES} ${HEADERS})
target_compile_definitions(${PROJECT_NAME}_shared PRIVATE
    _GLIBCXX_USE_CXX11_ABI=1
)

# Create static library
add_library(${PROJECT_NAME}_static STATIC ${SOURCES} ${HEADERS})
target_compile_definitions(${PROJECT_NAME}_static PRIVATE
    _GLIBCXX_USE_CXX11_ABI=1
)

# Set properties for both libraries
foreach(target ${PROJECT_NAME}_shared ${PROJECT_NAME}_static)
    target_include_directories(${target}
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
    )
    
    if(APPLE)
        # Add system include paths for each target
        target_include_directories(${target} SYSTEM PRIVATE
            /usr/local/include
            /usr/include
            /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include
        )
    endif()
endforeach()

# Set output names
set_target_properties(${PROJECT_NAME}_shared PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

set_target_properties(${PROJECT_NAME}_static PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
)

# Add unit test executable
add_executable(unit src/unit.cpp)
target_link_libraries(unit
    PRIVATE
        ${PROJECT_NAME}_static  # Link against the static library
        spdlog::spdlog         # Link against spdlog
)

# Enable testing
enable_testing()
add_test(NAME unit COMMAND unit)

# Installation commands
include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}_shared ${PROJECT_NAME}_static
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
