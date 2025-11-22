#ifndef BUS
    #define BUS

    #include <cstdint>
    #include <vector>
    #include <nes/ppu/registers/PPURegisters.hpp>
    
    enum class BUS_OPERATION_TYPES_ENUM {
        BUS_READ,
        BUS_WRITE
    };

    typedef struct BUS_OPERATION {
        uint16_t address; 
        uint8_t value;
        BUS_OPERATION_TYPES_ENUM type;
    }BUS_OPERATION;
    
    class Bus {
        private: 
            std::vector<uint8_t> memory;
            std::vector<BUS_OPERATION> operations;
            bool debbuging = false;
            static const int MEMORY_SIZE = 0x8000;  // Yeah, only 2 KB for RAM

        public:
            void writeMemoryAt(uint16_t address, uint8_t data);
            uint8_t readByteAt(uint16_t address);
            
            bool loadMemory(const std::vector<uint8_t>& room);
            void setDebugMode();
            
            Bus();
    };
#endif
