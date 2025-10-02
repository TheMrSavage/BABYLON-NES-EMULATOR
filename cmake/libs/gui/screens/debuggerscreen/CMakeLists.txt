add_library(DebuggerScreen ${PROJECT_ROOT}/src/libs/gui/Screens/DebuggerScreen/DebuggerScreen.hpp ${PROJECT_ROOT}/src/libs/gui/Screens/DebuggerScreen/DebuggerScreen.cpp)

target_include_directories(DebuggerScreen PRIVATE ${imgui_SOURCE_DIR})

target_link_libraries(DebuggerScreen PUBLIC Screen)
target_link_libraries(DebuggerScreen PUBLIC SDL3::SDL3)
target_link_libraries(DebuggerScreen PUBLIC ImGui)
