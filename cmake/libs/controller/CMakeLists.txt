add_library(Controller ${PROJECT_ROOT}/src/libs/controller/EmulatorController.hpp ${PROJECT_ROOT}/src/libs/controller/EmulatorController.cpp)

target_include_directories(Controller PUBLIC ${PROJECT_ROOT}/src/libs)

target_link_libraries(Controller PUBLIC Nes)
target_link_libraries(Controller PUBLIC InterfaceModule)
target_link_libraries(Controller PUBLIC SDL3::SDL3)
target_link_libraries(Controller PUBLIC ImGui)
