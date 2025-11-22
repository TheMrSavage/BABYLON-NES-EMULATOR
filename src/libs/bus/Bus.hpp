#ifndef BUS
    #define BUS

    #include <cstdint>
    #include <vector>
    #include <nes/ppu/registers/PPURegisters.hpp>
    
    class Bus {
        private: 
            std::vector<uint8_t> memory;
            static const int MEMORY_SIZE = 0x8000;  // Yeah, only 2 KB for RAM

        public:
            void writeMemoryAt(uint16_t address, uint8_t data);
            uint8_t readByteAt(uint16_t address);
            
            bool loadMemory(const std::vector<uint8_t>& room);

            Bus();
    };
#endif
