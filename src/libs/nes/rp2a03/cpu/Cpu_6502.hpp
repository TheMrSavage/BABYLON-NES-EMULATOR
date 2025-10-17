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
            uint8_t p;
            const std::vector<uint8_t>& memory; // This SHOULD BE replaced by a properly BUS.

            int executeNextInstruction();
            
            uint16_t getNextAddress(enum ADDRESSING_MODE_ENUM);
            uint8_t fetchByteAt(uint16_t address);
            uint8_t fetchNextByte();
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
            void CLC(uint16_t data);
            void CLD(uint16_t data);
            void CLI(uint16_t data);
            void CLV(uint16_t data);
            void CMP(uint16_t data);
            void CPX(uint16_t data);
            void CPY(uint16_t data);
            void DEC(uint16_t data);
            void DEX(uint16_t data);
            void DEY(uint16_t data);
            void EOR(uint16_t data);
            void INC(uint16_t data);
            void INX(uint16_t data);
            void INY(uint16_t data);
            void JMP(uint16_t data);
            void JSR(uint16_t data);
            void LDA(uint16_t data);
            void LDX(uint16_t data);
            void LDY(uint16_t data);
            void LSR(uint16_t data);
            void NOP(uint16_t data);
            void ORA(uint16_t data);
            void PHA(uint16_t data);
            void PHP(uint16_t data);
            void PLA(uint16_t data);
            void PLP(uint16_t data);
            void ROL(uint16_t data);
            void ROR(uint16_t data);
            void RTI(uint16_t data);
            void RTS(uint16_t data);
            void SBC(uint16_t data);
            void SEC(uint16_t data);
            void SED(uint16_t data);
            void SEI(uint16_t data);
            void STA(uint16_t data);
            void STX(uint16_t data);
            void STY(uint16_t data);
            void TAX(uint16_t data);
            void TAY(uint16_t data);
            void TSX(uint16_t data);
            void TXA(uint16_t data);
            void TXS(uint16_t data);
            void TYA(uint16_t data);
        public:
            // This must be used by debugger only!
            uint16_t& returnPc();
            uint8_t& returnSp();
            uint8_t& returnAcc();
            uint8_t& returnIdX();
            uint8_t& returnIdY();
            uint8_t& returnP();
            const std::vector<uint8_t>& returnMemory();

            CPU(std::vector<uint8_t>& memory) : memory(memory){};
    };
#endif
