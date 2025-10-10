cmake_minimum_required(VERSION 3.31.7)

project(BABYLON)

include(FetchContent)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    # warning level 4
    add_compile_options(/W4)
else()
    # additional warnings
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_subdirectory(cmake/dependencies)
add_subdirectory(cmake/libs)

add_executable(BABYLON src/main.cpp)

target_include_directories(BABYLON PUBLIC src/libs)

target_link_libraries(BABYLON PRIVATE Controller)
