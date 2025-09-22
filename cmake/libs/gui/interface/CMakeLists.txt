add_library(InterfaceModule ${PROJECT_ROOT}/src/libs/gui/Interface.hpp ${PROJECT_ROOT}/src/libs/gui/Interface.cpp)

target_include_directories(InterfaceModule PRIVATE ${imgui_SOURCE_DIR})

target_link_libraries(InterfaceModule PRIVATE MainScreen)
target_link_libraries(InterfaceModule PUBLIC SDL3::SDL3)
target_link_libraries(InterfaceModule PUBLIC ImGui)
