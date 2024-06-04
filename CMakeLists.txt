cmake_minimum_required(VERSION 3.5)

# Project Name
project(rimworld)

# Enable debug symbols
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

# Enable AddressSanitizer
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")

# Optional: Add more sanitizer flags if necessary
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address,undefined -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined -g")

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

# Add compiler options
target_compile_options(${PROJECT_NAME} PRIVATE -Wno-unknown-warning-option)

# Add libraries
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2 SDL2_image SDL2_ttf m)

# Add custom target for 'make run'
add_custom_target(run
    COMMAND ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running ${PROJECT_NAME}"
)
