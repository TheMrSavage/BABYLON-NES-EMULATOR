add_subdirectory(cpu)
add_subdirectory(apu)

add_library(Rp2a03 ${PROJECT_ROOT}/src/libs/nes/rp2a03/Rp2a03.hpp ${PROJECT_ROOT}/src/libs/nes/rp2a03/Rp2a03.cpp)

target_link_libraries(Rp2a03 PUBLIC Cpu)
target_link_libraries(Rp2a03 PUBLIC Apu)
