#ifndef BUS
    #define BUS

    #include <bits/std_thread.h>
    #include <cstdint>
    #include <ostream>
    #include <vector>
    #include "../nes/ppu/registers/PPURegisters.hpp"

    enum class BUS_OPERATION_TYPES_ENUM {
        BUS_READ = 0,
        BUS_WRITE = 1
    };

    typedef struct BUS_OPERATION {
        uint16_t address; 
        uint8_t value;
        BUS_OPERATION_TYPES_ENUM type;
    }BUS_OPERATION;
    
    std::ostream& operator<<(std::ostream& os, const BUS_OPERATION& operation);

    class Bus {
        private: 
            std::vector<uint8_t> memory;
            std::vector<BUS_OPERATION> operations;
            bool debbuging = false;
            static const int MEMORY_SIZE = 0x8000;  // Yeah, only 2 KB for RAM

        public:
            void writeByteAt(uint16_t address, uint8_t data);
            uint8_t fetchByteAt(uint16_t address);
            
            bool loadMemory(const std::vector<uint8_t>& room);
            void setDebugMode();
            
            const std::vector<BUS_OPERATION>& returnBusOperations();

            Bus();
    };
#endif
