cmake_minimum_required(VERSION 3.31.7)

project(NES_emulator)

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

add_subdirectory(cmake/dependencies/sdl3)
add_subdirectory(cmake/dependencies/ImGui)
add_subdirectory(cmake/dependencies/nativefiledialog)
add_subdirectory(cmake/libs/gui)
add_subdirectory(cmake/libs/nes)

add_executable(Nes_Emulator src/main.cpp)

target_include_directories(Nes_Emulator PRIVATE src/libs)

target_link_libraries(Nes_Emulator PRIVATE InterfaceModule)
target_link_libraries(Nes_Emulator PRIVATE Nes)
