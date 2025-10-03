add_subdirectory(ppu)
add_subdirectory(rp2a03)

add_library(Nes ${PROJECT_ROOT}/src/libs/nes/Nes.hpp ${PROJECT_ROOT}/src/libs/nes/Nes.cpp)

target_link_libraries(Nes PUBLIC Rp2a03)
target_link_libraries(Nes PUBLIC Ppu)
