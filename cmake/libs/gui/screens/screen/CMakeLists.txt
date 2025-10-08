add_library(Screen ${PROJECT_ROOT}/src/libs/gui/Screens/Screen.hpp ${PROJECT_ROOT}/src/libs/gui/Screens/Screen.cpp)

target_include_directories(Screen PRIVATE ${imgui_SOURCE_DIR})
target_include_directories(Screen PUBLIC ${PROJECT_ROOT}/src/libs)
target_link_libraries(Screen PUBLIC SDL3::SDL3)
