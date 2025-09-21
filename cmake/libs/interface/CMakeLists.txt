add_library(InterfaceModule ${PROJECT_ROOT}/src/libs/gui/Interface.cpp)

target_include_directories(InterfaceModule PRIVATE ${imgui_SOURCE_DIR})

target_link_libraries(InterfaceModule PRIVATE ImGui)
