cmake_minimum_required(VERSION 4.1)

MESSAGE(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")

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
    src/files/file_utils.cpp
    src/files/game_dat_file.cpp
    src/files/scenario_file.cpp
    src/files/text_cod_file.cpp
    src/tool/tool.cpp
    src/util/buffer_utils.cpp
    src/main.cpp
)

# Declare header files
set(HDR_FILES
    include/files/file_utils.h
    include/files/game_dat_file.h
    include/files/scenario_file.h
    include/files/text_cod_file.h
    include/tool/config.h
    include/tool/tool.h
    include/util/buffer_utils.h
)

# Add the executable target
add_executable(
    ${PROJECT_NAME}
    ${SRC_FILES}
    ${HDR_FILES}
)

# Find dependencies
#find_package(Boost CONFIG REQUIRED program_options)
find_package(boost_program_options CONFIG REQUIRED)

# Link dependencies
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::program_options)

# Organise files based on directories
source_group(
  TREE "${CMAKE_CURRENT_SOURCE_DIR}/src"
  PREFIX "Source Files"
  FILES ${SRC_FILES}
)
source_group(
  TREE "${CMAKE_CURRENT_SOURCE_DIR}/include"
  PREFIX "Source Files"
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
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive- /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

# Add Windows preprocessor definitions
if (MSVC)
    add_compile_definitions(
        WIN32_LEAN_AND_MEAN
        NOMINMAX
        STRICT
    )
endif()
