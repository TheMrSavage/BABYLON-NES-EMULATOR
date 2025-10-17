#ifndef CPU_6502
    #define CPU_6502

    #include <cstdint>
#include <vector>
    class CPU{
        private:
            uint16_t pc;
            uint8_t sp;
            uint8_t acc;
            uint8_t idX;
            uint8_t idY;
            uint8_t p;

            const std::vector<uint8_t>& memory; // This SHOULD BE replaced by a properly BUS.
            
        public:
            // This must be used by debugger only!
            uint16_t& returnPc();
            uint8_t& returnSp();
            uint8_t& returnAcc();
            uint8_t& returnIdX();
            uint8_t& returnIdY();
            uint8_t& returnP();

            CPU(std::vector<uint8_t>& memory) : memory(memory){};
    };
#endif
