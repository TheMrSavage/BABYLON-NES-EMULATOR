add_library(Cpu ${PROJECT_ROOT}/src/libs/nes/rp2a03/cpu/Cpu_6502.hpp ${PROJECT_ROOT}/src/libs/nes/rp2a03/cpu/Cpu_6502.cpp)
target_include_directories(Cpu PUBLIC ${PROJECT_ROOT}/src/libs/bus)
target_link_libraries(Cpu PUBLIC Bus)
