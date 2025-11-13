cmake_minimum_required(VERSION 4.1)

# Project metadata
project(AnnoTool
    VERSION 1.0
    DESCRIPTION "A tool for parsing and editing the files of Anno 1602"
    LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Declare source files
set(SRC_FILES
    src/main/main.cpp
)

# Declare header files
set(HDR_FILES
)

# Add the executable target
add_executable(
    ${PROJECT_NAME}
    ${SRC_FILES}
    ${HDR_FILES}
)

# Organise files based on directories
source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}"
    PREFIX "Source Files"
    FILES ${SRC_FILES}
)
source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}"
    PREFIX "Header Files"
    FILES ${HDR_FILES}
)

# Add include directories
target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# Set compiler warnings
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()
