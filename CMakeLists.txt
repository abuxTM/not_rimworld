cmake_minimum_required(VERSION 3.0)

# Project Name
project(rimworld)

# Set commands
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add common include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Collect all source files from subdirectories
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.c)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Add libraries
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2 SDL2_image SDL2_ttf m)

# Add custom target for 'make run'
add_custom_target(run
    COMMAND ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running ${PROJECT_NAME}"
)
