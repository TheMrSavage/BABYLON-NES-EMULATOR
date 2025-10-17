#include "Cpu_6502.hpp"
#include <cstdint>
#include <stdexcept>
#include <vector>
#include "instructions/InstructionsOpcodeEnum.hpp"

uint16_t& CPU::returnPc() {
    return this->pc;
}

uint8_t& CPU::returnSp() {
    return this->sp;
}

uint8_t& CPU::returnAcc() {
    return this->acc;
}

uint8_t& CPU::returnIdX() {
    return this->idX;
}

uint8_t& CPU::returnIdY() {
    return this->idY;
}

uint8_t& CPU::returnP() {
    return this->p;
}

const std::vector<uint8_t>& CPU::returnMemory() {
    return this->memory;
}

uint8_t CPU::fetchNextByte() {
    uint8_t data = this->fetchByteAt(this->pc++);
    return data;
}

// TODO: Implement addressing modes and return the properly address specified by it
uint16_t CPU::getNextAddress(enum ADDRESSING_MODE_ENUM) {

}

// TODO: Again, this should be replaced with a proper bus
uint8_t CPU::fetchByteAt(uint16_t address) {
    return this->memory[address];
}

// TODO: As i said below, there's some instructions that recive data and others that recive adresses. For now i'll leave like this and, with time, i'll adjust
// This function returns the clocks that every instruction costs
int CPU::executeNextInstruction() {
    uint8_t opcode = this->fetchNextByte();
    uint8_t data;
    uint16_t addressToFetchData;

    switch (opcode) {
        case ADC_IMMEDIATE:
            addressToFetchData = this->getNextAddress(IMMEDIATE);
            this->ADC(addressToFetchData);
            return 2; 
              
        case ADC_ZERO_PAGE:
            addressToFetchData = this->getNextAddress(ZERO_PAGE);
            this->ADC(addressToFetchData);
            return 3; 
                      
        case ADC_ZERO_PAGE_X:
            addressToFetchData = this->getNextAddress(ZERO_PAGE_X);
            this->ADC(addressToFetchData);
            return 4; 
                      
        case ADC_ABSOLUTE:
            addressToFetchData = this->getNextAddress(ABSOLUTE);
            this->ADC(addressToFetchData);
            return 4; 
                      
        case ADC_ABSOLUTE_X:
            addressToFetchData = this->getNextAddress(ABSOLUTE_X);
            this->ADC(addressToFetchData);
            return 4; // (+1 if page crossed)
                      
        case ADC_ABSOLUTE_Y:
            addressToFetchData = this->getNextAddress(ABSOLUTE_Y);
            this->ADC(addressToFetchData);
            return 4; // (+1 if page crossed)
                      
        case ADC_INDIRECT_X:
            addressToFetchData = this->getNextAddress(INDIRECT_X);
            this->ADC(addressToFetchData);
            return 6; 
                      
        case ADC_INDIRECT_Y:
            addressToFetchData = this->getNextAddress(INDIRECT_Y);
            this->ADC(addressToFetchData);
            return 5; // (+1 if page crossed)
                      
        case AND_IMMEDIATE:
            addressToFetchData = this->getNextAddress(IMMEDIATE);
            this->AND(addressToFetchData);
            return 2; 
                      
        case AND_ZERO_PAGE:
            addressToFetchData = this->getNextAddress(ZERO_PAGE);
            this->AND(addressToFetchData);
            return 3; 
                      
        case AND_ZERO_PAGE_X:
            addressToFetchData = this->getNextAddress(ZERO_PAGE_X);
            this->AND(addressToFetchData);
            return 4; 
                      
        case AND_ABSOLUTE:
            addressToFetchData = this->getNextAddress(ABSOLUTE);
            this->AND(addressToFetchData);
            return 4; 
                      
        case AND_ABSOLUTE_X:
            addressToFetchData = this->getNextAddress(ABSOLUTE_X);
            this->AND(addressToFetchData);
            return 4; // (+1 if page crossed)
                      
        case AND_ABSOLUTE_Y:
            addressToFetchData = this->getNextAddress(ABSOLUTE_Y);
            this->AND(addressToFetchData);
            return 4; // (+1 if page crossed)
                      
        case AND_INDIRECT_X:
            addressToFetchData = this->getNextAddress(INDIRECT_X);
            this->AND(addressToFetchData);
            return 6; 
                      
        case AND_INDIRECT_Y:
            addressToFetchData = this->getNextAddress(INDIRECT_Y);
            this->AND(addressToFetchData);
            return 5; // (+1 if page crossed)
                      
        case ASL_ACCUMULATOR:
            addressToFetchData = this->getNextAddress(ACCUMULATOR);
            this->ASL(addressToFetchData);
            return 2; 
                      
        case ASL_ZERO_PAGE:
            addressToFetchData = this->getNextAddress(ZERO_PAGE);
            this->ASL(addressToFetchData);
            return 5; 
                      
        case ASL_ZERO_PAGE_X:
            addressToFetchData = this->getNextAddress(ZERO_PAGE_X);
            this->ASL(addressToFetchData);
            return 6; 
                      
        case ASL_ABSOLUTE:
            addressToFetchData = this->getNextAddress(ABSOLUTE);
            this->ASL(addressToFetchData);
            return 6; 
                      
        case ASL_ABSOLUTE_X:
            addressToFetchData = this->getNextAddress(ABSOLUTE_X);
            this->ASL(addressToFetchData);
            return 7; 
                      
        case BCC_RELATIVE:
            addressToFetchData = this->getNextAddress(RELATIVE);
            this->BCC(addressToFetchData);
            return 2; // (+1 if branch succeeds+2 if to a new page)
                      
        case BCS_RELATIVE:
            addressToFetchData = this->getNextAddress(RELATIVE);
            this->BCS(addressToFetchData);
            return 2; // (+1 if branch succeeds +2 if to a new page)
                      
        case BEQ_RELATIVE:
            addressToFetchData = this->getNextAddress(RELATIVE);
            this->BEQ(addressToFetchData);
            return 2; // (+1 if branch succeeds +2 if to a new page)
                      
        case BIT_ZERO_PAGE:
            addressToFetchData = this->getNextAddress(ZERO_PAGE);
            this->BIT(addressToFetchData);
            return 3; 
                      
        case BIT_ABSOLUTE:
            addressToFetchData = this->getNextAddress(ABSOLUTE);
            this->BIT(addressToFetchData);
            return 4; 
                      
        case BMI_RELATIVE:
            addressToFetchData = this->getNextAddress(RELATIVE);
            this->BMI(addressToFetchData);
            return 2; // (+1 if branch succeeds +2 if to a new page)
                      
        case BNE_RELATIVE:
            addressToFetchData = this->getNextAddress(RELATIVE);
            this->BNE(addressToFetchData);
            return 2; // (+1 if branch succeeds +2 if to a new page)
                      
        case BPL_RELATIVE:
            addressToFetchData = this->getNextAddress(RELATIVE);
            this->BPL(addressToFetchData);
            return 2; // (+1 if branch succeeds +2 if to a new page)
                      
        case BRK_IMPLIED:
            addressToFetchData = this->getNextAddress(IMPLIED);
            this->BRK(addressToFetchData);
            return 7; 
                      
        case BVC_RELATIVE:
            addressToFetchData = this->getNextAddress(RELATIVE);
            this->BVC(addressToFetchData);
            return 2; // (+1 if branch succeeds +2 if to a new page)
                      
        case BVS_RELATIVE:
            addressToFetchData = this->getNextAddress(RELATIVE);
            this->BVS(addressToFetchData);
            return 2; // (+1 if branch succeeds +2 if to a new page)
                      
        case CLC_IMPLIED:
            addressToFetchData = this->getNextAddress(IMPLIED);
            this->CLC(addressToFetchData);
            return 2; 
                      
        case CLD_IMPLIED:
            addressToFetchData = this->getNextAddress(IMPLIED);
            this->CLD(addressToFetchData);
            return 2; 
                      
        case CLI_IMPLIED:
            addressToFetchData = this->getNextAddress(IMPLIED);
            this->CLI(addressToFetchData);
            return 2; 
                      
        case CLV_IMPLIED:
            addressToFetchData = this->getNextAddress(IMPLIED);
            this->CLV(addressToFetchData);
            return 2; 
                      
        case CMP_IMMEDIATE:
            addressToFetchData = this->getNextAddress(IMMEDIATE);
            this->CMP(addressToFetchData);
            return 2; 
                      
        case CMP_ZERO_PAGE:
            addressToFetchData = this->getNextAddress(ZERO_PAGE);
            this->CMP(addressToFetchData);
            return 3; 
                      
        case CMP_ZERO_PAGE_X:
            addressToFetchData = this->getNextAddress(ZERO_PAGE_X);
            this->CMP(addressToFetchData);
            return 4; 
                      
        case CMP_ABSOLUTE:
            addressToFetchData = this->getNextAddress(ABSOLUTE);
            this->CMP(addressToFetchData);
            return 4; 
                      
        case CMP_ABSOLUTE_X:
            addressToFetchData = this->getNextAddress(ABSOLUTE_X);
            this->CMP(addressToFetchData);
            return 4; // (+1 if page crossed)
                      
        case CMP_ABSOLUTE_Y:
            addressToFetchData = this->getNextAddress(ABSOLUTE_Y);
            this->CMP(addressToFetchData);
            return 4; // (+1 if page crossed)
                      
        case CMP_INDIRECT_X:
            addressToFetchData = this->getNextAddress(INDIRECT_X);
            this->CMP(addressToFetchData);
            return 6; 
                      
        case CMP_INDIRECT_Y:
            addressToFetchData = this->getNextAddress(INDIRECT_Y);
            this->CMP(addressToFetchData);
            return 5; // (+1 if page crossed)
                      
        case CPX_IMMEDIATE:
            addressToFetchData = this->getNextAddress(IMMEDIATE);
            this->CPX(addressToFetchData);
            return 2; 
                      
        case CPX_ZERO_PAGE:
            addressToFetchData = this->getNextAddress(ZERO_PAGE);
            this->CPX(addressToFetchData);
            return 3; 
                      
        case CPX_ABSOLUTE:
            addressToFetchData = this->getNextAddress(ABSOLUTE);
            this->CPX(addressToFetchData);
            return 4; 
                      
        case CPY_IMMEDIATE:
            addressToFetchData = this->getNextAddress(IMMEDIATE);
            this->CPY(addressToFetchData);
            return 2; 
                      
        case CPY_ZERO_PAGE:
            addressToFetchData = this->getNextAddress(ZERO_PAGE);
            this->CPY(addressToFetchData);
            return 3; 
                      
        case CPY_ABSOLUTE:
            addressToFetchData = this->getNextAddress(ABSOLUTE);
            this->CPY(addressToFetchData);
            return 4; 
                      
        case DEC_ZERO_PAGE:
            addressToFetchData = this->getNextAddress(ZERO_PAGE);
            this->DEC(addressToFetchData);
            return 5; 
                      
        case DEC_ZERO_PAGE_X:
            addressToFetchData = this->getNextAddress(ZERO_PAGE_X);
            this->DEC(addressToFetchData);
            return 6; 
                      
        case DEC_ABSOLUTE:
            addressToFetchData = this->getNextAddress(ABSOLUTE);
            this->DEC(addressToFetchData);
            return 6; 
                      
        case DEC_ABSOLUTE_X:
            addressToFetchData = this->getNextAddress(ABSOLUTE_X);
            this->DEC(addressToFetchData);
            return 7; 
                      
        case DEX_IMPLIED:
            addressToFetchData = this->getNextAddress(IMPLIED);
            this->DEX(addressToFetchData);
            return 2; 
                      
        case DEY_IMPLIED:
            addressToFetchData = this->getNextAddress(IMPLIED);
            this->DEY(addressToFetchData);
            return 2; 
                      
        case EOR_IMMEDIATE:
            addressToFetchData = this->getNextAddress(IMMEDIATE);
            this->EOR(addressToFetchData);
            return 2; 
                      
        case EOR_ZERO_PAGE:
            addressToFetchData = this->getNextAddress(ZERO_PAGE);
            this->EOR(addressToFetchData);
            return 3; 
                      
        case EOR_ZERO_PAGE_X:
            addressToFetchData = this->getNextAddress(ZERO_PAGE_X);
            this->EOR(addressToFetchData);
            return 4; 
                      
        case EOR_ABSOLUTE:
            addressToFetchData = this->getNextAddress(ABSOLUTE);
            this->EOR(addressToFetchData);
            return 4; 
                      
        case EOR_ABSOLUTE_X:
            addressToFetchData = this->getNextAddress(ABSOLUTE_X);
            this->EOR(addressToFetchData);
            return 4; // (+1 if page crossed)
                      
        case EOR_ABSOLUTE_Y:
            addressToFetchData = this->getNextAddress(ABSOLUTE_Y);
            this->EOR(addressToFetchData);
            return 4; // (+1 if page crossed)
                      
        case EOR_INDIRECT_X:
            addressToFetchData = this->getNextAddress(INDIRECT_X);
            this->EOR(addressToFetchData);
            return 6; 
                      
        case EOR_INDIRECT_Y:
            addressToFetchData = this->getNextAddress(INDIRECT_Y);
            this->EOR(addressToFetchData);
            return 5; // (+1 if page crossed)
                      
        case INC_ZERO_PAGE:
            addressToFetchData = this->getNextAddress(ZERO_PAGE);
            this->INC(addressToFetchData);
            return 5; 
                      
        case INC_ZERO_PAGE_X:
            addressToFetchData = this->getNextAddress(ZERO_PAGE_X);
            this->INC(addressToFetchData);
            return 6; 
                      
        case INC_ABSOLUTE:
            addressToFetchData = this->getNextAddress(ABSOLUTE);
            this->INC(addressToFetchData);
            return 6; 
                      
        case INC_ABSOLUTE_X:
            addressToFetchData = this->getNextAddress(ABSOLUTE_X);
            this->INC(addressToFetchData);
            return 7; 
                      
        case INX_IMPLIED:
            addressToFetchData = this->getNextAddress(IMPLIED);
            this->INX(addressToFetchData);
            return 2; 
                      
        case INY_IMPLIED:
            addressToFetchData = this->getNextAddress(IMPLIED);
            this->INY(addressToFetchData);
            return 2; 
                      
        case JMP_ABSOLUTE:
            addressToFetchData = this->getNextAddress(ABSOLUTE);
            this->JMP(addressToFetchData);
            return 3; 
                      
        case JMP_INDIRECT:
            addressToFetchData = this->getNextAddress(INDIRECT);
            this->JMP(addressToFetchData);
            return 5; 
                      
        case JSR_ABSOLUTE:
            addressToFetchData = this->getNextAddress(ABSOLUTE);
            this->JSR(addressToFetchData);
            return 6; 
                      
        case LDA_IMMEDIATE:
            addressToFetchData = this->getNextAddress(IMMEDIATE);
            this->LDA(addressToFetchData);
            return 2; 
                      
        case LDA_ZERO_PAGE:
            addressToFetchData = this->getNextAddress(ZERO_PAGE);
            this->LDA(addressToFetchData);
            return 3; 
                      
        case LDA_ZERO_PAGE_X:
            addressToFetchData = this->getNextAddress(ZERO_PAGE_X);
            this->LDA(addressToFetchData);
            return 4; 
                      
        case LDA_ABSOLUTE:
            addressToFetchData = this->getNextAddress(ABSOLUTE);
            this->LDA(addressToFetchData);
            return 4; 
                      
        case LDA_ABSOLUTE_X:
            addressToFetchData = this->getNextAddress(ABSOLUTE_X);
            this->LDA(addressToFetchData);
            return 4; // (+1 if page crossed)
                      
        case LDA_ABSOLUTE_Y:
            addressToFetchData = this->getNextAddress(ABSOLUTE_Y);
            this->LDA(addressToFetchData);
            return 4; // (+1 if page crossed)
                      
        case LDA_INDIRECT_X:
            addressToFetchData = this->getNextAddress(INDIRECT_X);
            this->LDA(addressToFetchData);
            return 6; 
                      
        case LDA_INDIRECT_Y:
            addressToFetchData = this->getNextAddress(INDIRECT_Y);
            this->LDA(addressToFetchData);
            return 5; // (+1 if page crossed)
                      
        case LDX_IMMEDIATE:
            addressToFetchData = this->getNextAddress(IMMEDIATE);
            this->LDX(addressToFetchData);
            return 2; 
                      
        case LDX_ZERO_PAGE:
            addressToFetchData = this->getNextAddress(ZERO_PAGE);
            this->LDX(addressToFetchData);
            return 3; 
                      
        case LDX_ZERO_PAGE_Y:
            addressToFetchData = this->getNextAddress(ZERO_PAGE_Y);
            this->LDX(addressToFetchData);
            return 4; 
                      
        case LDX_ABSOLUTE:
            addressToFetchData = this->getNextAddress(ABSOLUTE);
            this->LDX(addressToFetchData);
            return 4; 
                      
        case LDX_ABSOLUTE_Y:
            addressToFetchData = this->getNextAddress(ABSOLUTE_Y);
            this->LDX(addressToFetchData);
            return 4; // (+1 if page crossed)
                      
        case LDY_IMMEDIATE:
            addressToFetchData = this->getNextAddress(IMMEDIATE);
            this->LDY(addressToFetchData);
            return 2; 
                      
        case LDY_ZERO_PAGE:
            addressToFetchData = this->getNextAddress(ZERO_PAGE);
            this->LDY(addressToFetchData);
            return 3; 
                      
        case LDY_ZERO_PAGE_X:
            addressToFetchData = this->getNextAddress(ZERO_PAGE_X);
            this->LDY(addressToFetchData);
            return 4; 
                      
        case LDY_ABSOLUTE:
            addressToFetchData = this->getNextAddress(ABSOLUTE);
            this->LDY(addressToFetchData);
            return 4; 
                      
        case LDY_ABSOLUTE_X:
            addressToFetchData = this->getNextAddress(ABSOLUTE_X);
            this->LDY(addressToFetchData);
            return 4; // (+1 if page crossed)
                      
        case LSR_ACCUMULATOR:
            addressToFetchData = this->getNextAddress(ACCUMULATOR);
            this->LSR(addressToFetchData);
            return 2; 
                      
        case LSR_ZERO_PAGE:
            addressToFetchData = this->getNextAddress(ZERO_PAGE);
            this->LSR(addressToFetchData);
            return 5; 
                      
        case LSR_ZERO_PAGE_X:
            addressToFetchData = this->getNextAddress(ZERO_PAGE_X);
            this->LSR(addressToFetchData);
            return 6; 
                      
        case LSR_ABSOLUTE:
            addressToFetchData = this->getNextAddress(ABSOLUTE);
            this->LSR(addressToFetchData);
            return 6; 
                      
        case LSR_ABSOLUTE_X:
            addressToFetchData = this->getNextAddress(ABSOLUTE_X);
            this->LSR(addressToFetchData);
            return 7; 
                      
        case NOP_IMPLIED:
            addressToFetchData = this->getNextAddress(IMPLIED);
            this->NOP(addressToFetchData);
            return 2; 
                      
        case ORA_IMMEDIATE:
            addressToFetchData = this->getNextAddress(IMMEDIATE);
            this->ORA(addressToFetchData);
            return 2; 
                      
        case ORA_ZERO_PAGE:
            addressToFetchData = this->getNextAddress(ZERO_PAGE);
            this->ORA(addressToFetchData);
            return 3; 
                      
        case ORA_ZERO_PAGE_X:
            addressToFetchData = this->getNextAddress(ZERO_PAGE_X);
            this->ORA(addressToFetchData);
            return 4; 
                      
        case ORA_ABSOLUTE:
            addressToFetchData = this->getNextAddress(ABSOLUTE);
            this->ORA(addressToFetchData);
            return 4; 
                      
        case ORA_ABSOLUTE_X:
            addressToFetchData = this->getNextAddress(ABSOLUTE_X);
            this->ORA(addressToFetchData);
            return 4; // (+1 if page crossed)
                      
        case ORA_ABSOLUTE_Y:
            addressToFetchData = this->getNextAddress(ABSOLUTE_Y);
            this->ORA(addressToFetchData);
            return 4; // (+1 if page crossed)
                      
        case ORA_INDIRECT_X:
            addressToFetchData = this->getNextAddress(INDIRECT_X);
            this->ORA(addressToFetchData);
            return 6; 
                      
        case ORA_INDIRECT_Y:
            addressToFetchData = this->getNextAddress(INDIRECT_Y);
            this->ORA(addressToFetchData);
            return 5; // (+1 if page crossed)
                      
        case PHA_IMPLIED:
            addressToFetchData = this->getNextAddress(IMPLIED);
            this->PHA(addressToFetchData);
            return 3; 
                      
        case PHP_IMPLIED:
            addressToFetchData = this->getNextAddress(IMPLIED);
            this->PHP(addressToFetchData);
            return 3; 
                      
        case PLA_IMPLIED:
            addressToFetchData = this->getNextAddress(IMPLIED);
            this->PLA(addressToFetchData);
            return 4; 
                      
        case PLP_IMPLIED:
            addressToFetchData = this->getNextAddress(IMPLIED);
            this->PLP(addressToFetchData);
            return 4; 
                      
        case ROL_ACCUMULATOR:
            addressToFetchData = this->getNextAddress(ACCUMULATOR);
            this->ROL(addressToFetchData);
            return 2; 
                      
        case ROL_ZERO_PAGE:
            addressToFetchData = this->getNextAddress(ZERO_PAGE);
            this->ROL(addressToFetchData);
            return 5; 
                      
        case ROL_ZERO_PAGE_X:
            addressToFetchData = this->getNextAddress(ZERO_PAGE_X);
            this->ROL(addressToFetchData);
            return 6; 
                      
        case ROL_ABSOLUTE:
            addressToFetchData = this->getNextAddress(ABSOLUTE);
            this->ROL(addressToFetchData);
            return 6; 
                      
        case ROL_ABSOLUTE_X:
            addressToFetchData = this->getNextAddress(ABSOLUTE_X);
            this->ROL(addressToFetchData);
            return 7; 
                      
        case ROR_ACCUMULATOR:
            addressToFetchData = this->getNextAddress(ACCUMULATOR);
            this->ROR(addressToFetchData);
            return 2; 
                      
        case ROR_ZERO_PAGE:
            addressToFetchData = this->getNextAddress(ZERO_PAGE);
            this->ROR(addressToFetchData);
            return 5; 
                      
        case ROR_ZERO_PAGE_X:
            addressToFetchData = this->getNextAddress(ZERO_PAGE_X);
            this->ROR(addressToFetchData);
            return 6; 
                      
        case ROR_ABSOLUTE:
            addressToFetchData = this->getNextAddress(ABSOLUTE);
            this->ROR(addressToFetchData);
            return 6; 
                      
        case ROR_ABSOLUTE_X:
            addressToFetchData = this->getNextAddress(ABSOLUTE_X);
            this->ROR(addressToFetchData);
            return 7; 
                      
        case RTI_IMPLIED:
            addressToFetchData = this->getNextAddress(IMPLIED);
            this->RTI(addressToFetchData);
            return 6; 
                      
        case RTS_IMPLIED:
            addressToFetchData = this->getNextAddress(IMPLIED);
            this->RTS(addressToFetchData);
            return 6; 
                      
        case SBC_IMMEDIATE:
            addressToFetchData = this->getNextAddress(IMMEDIATE);
            this->SBC(addressToFetchData);
            return 2; 
                      
        case SBC_ZERO_PAGE:
            addressToFetchData = this->getNextAddress(ZERO_PAGE);
            this->SBC(addressToFetchData);
            return 3; 
                      
        case SBC_ZERO_PAGE_X:
            addressToFetchData = this->getNextAddress(ZERO_PAGE_X);
            this->SBC(addressToFetchData);
            return 4; 
                      
        case SBC_ABSOLUTE:
            addressToFetchData = this->getNextAddress(ABSOLUTE);
            this->SBC(addressToFetchData);
            return 4; 
                      
        case SBC_ABSOLUTE_X:
            addressToFetchData = this->getNextAddress(ABSOLUTE_X);
            this->SBC(addressToFetchData);
            return 4; // (+1 if page crossed)
                      
        case SBC_ABSOLUTE_Y:
            addressToFetchData = this->getNextAddress(ABSOLUTE_Y);
            this->SBC(addressToFetchData);
            return 4; // (+1 if page crossed)
                      
        case SBC_INDIRECT_X:
            addressToFetchData = this->getNextAddress(INDIRECT_X);
            this->SBC(addressToFetchData);
            return 6; 
                      
        case SBC_INDIRECT_Y:
            addressToFetchData = this->getNextAddress(INDIRECT_Y);
            this->SBC(addressToFetchData);
            return 5; // (+1 if page crossed)
                      
        case SEC_IMPLIED:
            addressToFetchData = this->getNextAddress(IMPLIED);
            this->SEC(addressToFetchData);
            return 2; 
                      
        case SED_IMPLIED:
            addressToFetchData = this->getNextAddress(IMPLIED);
            this->SED(addressToFetchData);
            return 2; 
                      
        case SEI_IMPLIED:
            addressToFetchData = this->getNextAddress(IMPLIED);
            this->SEI(addressToFetchData);
            return 2; 
                      
        case STA_ZERO_PAGE:
            addressToFetchData = this->getNextAddress(ZERO_PAGE);
            this->STA(addressToFetchData);
            return 3; 
                      
        case STA_ZERO_PAGE_X:
            addressToFetchData = this->getNextAddress(ZERO_PAGE_X);
            this->STA(addressToFetchData);
            return 4; 
                      
        case STA_ABSOLUTE:
            addressToFetchData = this->getNextAddress(ABSOLUTE);
            this->STA(addressToFetchData);
            return 4; 
                      
        case STA_ABSOLUTE_X:
            addressToFetchData = this->getNextAddress(ABSOLUTE_X);
            this->STA(addressToFetchData);
            return 5; 
                      
        case STA_ABSOLUTE_Y:
            addressToFetchData = this->getNextAddress(ABSOLUTE_Y);
            this->STA(addressToFetchData);
            return 5; 
                      
        case STA_INDIRECT_X:
            addressToFetchData = this->getNextAddress(INDIRECT_X);
            this->STA(addressToFetchData);
            return 6; 
                      
        case STA_INDIRECT_Y:
            addressToFetchData = this->getNextAddress(INDIRECT_Y);
            this->STA(addressToFetchData);
            return 6; 
                      
        case STX_ZERO_PAGE:
            addressToFetchData = this->getNextAddress(ZERO_PAGE);
            this->STX(addressToFetchData);
            return 3; 
                      
        case STX_ZERO_PAGE_Y:
            addressToFetchData = this->getNextAddress(ZERO_PAGE_Y);
            this->STX(addressToFetchData);
            return 4; 
                      
        case STX_ABSOLUTE:
            addressToFetchData = this->getNextAddress(ABSOLUTE);
            this->STX(addressToFetchData);
            return 4; 
                      
        case STY_ZERO_PAGE:
            addressToFetchData = this->getNextAddress(ZERO_PAGE);
            this->STY(addressToFetchData);
            return 3; 
                      
        case STY_ZERO_PAGE_X:
            addressToFetchData = this->getNextAddress(ZERO_PAGE_X);
            this->STY(addressToFetchData);
            return 4; 
                      
        case STY_ABSOLUTE:
            addressToFetchData = this->getNextAddress(ABSOLUTE);
            this->STY(addressToFetchData);
            return 4; 
                      
        case TAX_IMPLIED:
            addressToFetchData = this->getNextAddress(IMPLIED);
            this->TAX(addressToFetchData);
            return 2; 
                      
        case TAY_IMPLIED:
            addressToFetchData = this->getNextAddress(IMPLIED);
            this->TAY(addressToFetchData);
            return 2; 
                      
        case TSX_IMPLIED:
            addressToFetchData = this->getNextAddress(IMPLIED);
            this->TSX(addressToFetchData);
            return 2; 
                      
        case TXA_IMPLIED:
            addressToFetchData = this->getNextAddress(IMPLIED);
            this->TXA(addressToFetchData);
            return 2; 
                      
        case TXS_IMPLIED:
            addressToFetchData = this->getNextAddress(IMPLIED);
            this->TXS(addressToFetchData);
            return 2; 
                      
        case TYA_IMPLIED:
            addressToFetchData = this->getNextAddress(IMPLIED);
            this->TYA(addressToFetchData);
            return 2; 

        default:
            throw std::runtime_error("Invalid opcode. Exiting...");
    }
}

// All instructions below
// TODO: There's some instructions that recive an address and other then recive a data... With time i change the signature of
//    functions to match each case, but i want to have the proper skeleton for now

//TODO: Implement
void CPU::ADC(uint16_t data){}

//TODO: Implement
void CPU::AND(uint16_t data){}

//TODO: Implement
void CPU::ASL(uint16_t data){}

//TODO: Implement
void CPU::BCC(uint16_t data){}

//TODO: Implement
void CPU::BCS(uint16_t data){}

//TODO: Implement
void CPU::BEQ(uint16_t data){}

//TODO: Implement
void CPU::BIT(uint16_t data){}

//TODO: Implement
void CPU::BMI(uint16_t data){}

//TODO: Implement
void CPU::BNE(uint16_t data){}

//TODO: Implement
void CPU::BPL(uint16_t data){}

//TODO: Implement
void CPU::BRK(uint16_t data){}

//TODO: Implement
void CPU::BVC(uint16_t data){}

//TODO: Implement
void CPU::BVS(uint16_t data){}

//TODO: Implement
void CPU::CLC(uint16_t data){}

//TODO: Implement
void CPU::CLD(uint16_t data){}

//TODO: Implement
void CPU::CLI(uint16_t data){}

//TODO: Implement
void CPU::CLV(uint16_t data){}

//TODO: Implement
void CPU::CMP(uint16_t data){}

//TODO: Implement
void CPU::CPX(uint16_t data){}

//TODO: Implement
void CPU::CPY(uint16_t data){}

//TODO: Implement
void CPU::DEC(uint16_t data){}

//TODO: Implement
void CPU::DEX(uint16_t data){}

//TODO: Implement
void CPU::DEY(uint16_t data){}

//TODO: Implement
void CPU::EOR(uint16_t data){}

//TODO: Implement
void CPU::INC(uint16_t data){}

//TODO: Implement
void CPU::INX(uint16_t data){}

//TODO: Implement
void CPU::INY(uint16_t data){}

//TODO: Implement
void CPU::JMP(uint16_t data){}

//TODO: Implement
void CPU::JSR(uint16_t data){}

//TODO: Implement
void CPU::LDA(uint16_t data){}

//TODO: Implement
void CPU::LDX(uint16_t data){}

//TODO: Implement
void CPU::LDY(uint16_t data){}

//TODO: Implement
void CPU::LSR(uint16_t data){}

//TODO: Implement
void CPU::NOP(uint16_t data){}

//TODO: Implement
void CPU::ORA(uint16_t data){}

//TODO: Implement
void CPU::PHA(uint16_t data){}

//TODO: Implement
void CPU::PHP(uint16_t data){}

//TODO: Implement
void CPU::PLA(uint16_t data){}

//TODO: Implement
void CPU::PLP(uint16_t data){}

//TODO: Implement
void CPU::ROL(uint16_t data){}

//TODO: Implement
void CPU::ROR(uint16_t data){}

//TODO: Implement
void CPU::RTI(uint16_t data){}

//TODO: Implement
void CPU::RTS(uint16_t data){}

//TODO: Implement
void CPU::SBC(uint16_t data){}

//TODO: Implement
void CPU::SEC(uint16_t data){}

//TODO: Implement
void CPU::SED(uint16_t data){}

//TODO: Implement
void CPU::SEI(uint16_t data){}

//TODO: Implement
void CPU::STA(uint16_t data){}

//TODO: Implement
void CPU::STX(uint16_t data){}

//TODO: Implement
void CPU::STY(uint16_t data){}

//TODO: Implement
void CPU::TAX(uint16_t data){}

//TODO: Implement
void CPU::TAY(uint16_t data){}

//TODO: Implement
void CPU::TSX(uint16_t data){}

//TODO: Implement
void CPU::TXA(uint16_t data){}

//TODO: Implement
void CPU::TXS(uint16_t data){}

//TODO: Implement
void CPU::TYA(uint16_t data){}
