#ifndef CPU_6502
    #define CPU_6502

    #include <cstdint>
    #include <vector>
    #include "instructions/InstructionsOpcodeEnum.hpp"
    class CPU{
        private:
            uint16_t pc;
            uint8_t sp;
            uint8_t acc;
            uint8_t idX;
            uint8_t idY;
            
            // From https://www.nesdev.org/wiki/Status_flags:
            /*
                7  bit  0
                ---- ----
                NV1B DIZC
                |||| ||||
                |||| |||+- Carry
                |||| ||+-- Zero
                |||| |+--- Interrupt Disable
                |||| +---- Decimal
                |||+------ (No CPU effect; see: the B flag)
                ||+------- (No CPU effect; always pushed as 1)
                |+-------- Overflow
                +--------- Negative
             */
            uint8_t p;            

            std::vector<uint8_t>& memory; // This SHOULD BE replaced by a properly BUS.
            
            uint8_t stackPop();
            void stackPush(uint8_t data);

            uint16_t getNextAddress(enum ADDRESSING_MODE_ENUM);
            uint8_t fetchByteAt(uint16_t address);
            uint8_t fetchNextByte();

            void writeByteAt(uint16_t address, uint8_t data);


            // Instructions below
            void ADC(uint16_t data);
            void AND(uint16_t data);
            void ASL(uint16_t data);
            void BCC(uint16_t data);
            void BCS(uint16_t data);
            void BEQ(uint16_t data);
            void BIT(uint16_t data);
            void BMI(uint16_t data);
            void BNE(uint16_t data);
            void BPL(uint16_t data);
            void BRK(uint16_t data);
            void BVC(uint16_t data);
            void BVS(uint16_t data);
            void CLC();
            void CLD();
            void CLI();
            void CLV();
            void CMP(uint16_t data);
            void CPX(uint16_t data);
            void CPY(uint16_t data);
            void DEC(uint16_t data);
            void DEX();
            void DEY();
            void EOR(uint16_t data);
            void INC(uint16_t data);
            void INX();
            void INY();
            void JMP(uint16_t data);
            void JSR(uint16_t data);
            void LDA(uint16_t data);
            void LDX(uint16_t data);
            void LDY(uint16_t data);
            void LSR(uint16_t data);
            void NOP();
            void ORA(uint16_t data);
            void PHA();
            void PHP();
            void PLA();
            void PLP();
            void ROL(uint16_t data);
            void ROR(uint16_t data);
            void RTI(uint16_t data);
            void RTS(uint16_t data);
            void SBC(uint16_t data);
            void SEC();
            void SED();
            void SEI();
            void STA(uint16_t data);
            void STX(uint16_t data);
            void STY(uint16_t data);
            void TAX();
            void TAY();
            void TSX(uint16_t data);
            void TXA();
            void TXS(uint16_t data);
            void TYA();
        public:
            // This must be used by debugger only!
            uint16_t& returnPc();
            uint8_t& returnSp();
            uint8_t& returnAcc();
            uint8_t& returnIdX();
            uint8_t& returnIdY();
            uint8_t& returnP();
            const std::vector<uint8_t>& returnMemory();
            
            int executeNextInstruction();
            
            // "The processor supports a 256 byte stack located between $0100 and $01FF. The stack pointer is an 8 bit register and holds the low 8 bits of the next free location on the stack. The location of the stack is fixed and cannot be moved."
            // "Pushing bytes to the stack causes the stack pointer to be decremented. Conversely pulling bytes causes it to be incremented."
            CPU(std::vector<uint8_t>& memory) : memory(memory){
                this->sp = 0xFF;
            };
    };
#endif
