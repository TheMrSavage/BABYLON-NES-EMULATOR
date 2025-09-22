add_library(MainScreen ${PROJECT_ROOT}/src/libs/gui/Screens/MainScreen/MainScreen.hpp ${PROJECT_ROOT}/src/libs/gui/Screens/MainScreen/MainScreen.cpp)

target_include_directories(MainScreen PRIVATE ${imgui_SOURCE_DIR})

target_link_libraries(MainScreen PUBLIC SDL3::SDL3)
target_link_libraries(MainScreen PUBLIC ImGui)
