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

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")

add_subdirectory(cmake/options)

# TODO: Find a better way to setup options...
if (DEV_MODE)
    add_compile_options(-g3 -Og -fno-omit-frame-pointer)
endif()

# TODO: Find a better way to separate different builds...
if (BUILD_TESTS)
    add_executable(Tests src/tests/tests.cpp)
    add_subdirectory(cmake/dependencies/json)
    target_include_directories(Tests PUBLIC src/libs)
    target_link_libraries(Tests PRIVATE Cpu)
else ()
    add_executable(BABYLON src/main.cpp)
    target_include_directories(BABYLON PUBLIC src/libs)
    target_link_libraries(BABYLON PRIVATE Controller)
endif()
