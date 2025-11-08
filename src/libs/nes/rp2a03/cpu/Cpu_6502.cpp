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

uint8_t CPU::stackPop() {
    this->sp++;
    
    uint8_t element = this->fetchByteAt(0x0100 | this->sp);   

    return element;
}

void CPU::stackPush(uint8_t data) {
    this->writeByteAt(0x0100 | this->sp, data);

    this->sp--;
}

void CPU::writeByteAt(uint16_t address, uint8_t data) {
    this->memory[address] = data;
}

// DONE: Implement addressing modes and return the properly address specified by it
// TODO: Figure out how to verify if there's any pagecrossing in *PAGE*/INDIRECT_* instructions. Maybe reference-value return? I think it's a good approach
uint16_t CPU::getNextAddress(enum ADDRESSING_MODE_ENUM adressingMode) {
    uint16_t address = 0x0000;

    switch (adressingMode) {
        
        // The source/destiny is implicit in operation 
        case IMPLIED : {
            address = 0x0000;
            break;
        }
        
        // Again, here the destiny is accumulator and the rest is implicit in operation
        case ACCUMULATOR : {
            address = 0x0000;
            break;
        }
        
        // As obelisk guide says: "Immediate addressing allows the programmer to directly specify an 8 bit constant within the instruction. It is indicated by a '#' symbol followed by an numeric expression."
        // So, literally the adress is just the next byte. 
        // I mean, i can put address as 0x0000 and use fetchNextByte, but i think this way it'll be more clear. 
        case IMMEDIATE : {
            address = (this->pc++);
            break;
        }
        
        // As obelisk guide says: "An instruction using zero page addressing mode has only an 8 bit address operand."
        case ZERO_PAGE : {
            address = this->fetchNextByte(); 
            break;
        }
        
        // " The address calculation wraps around if the sum of the base address and the register exceed $FF. If we repeat the last example but with $FF in the X register then the accumulator will be loaded from $007F (e.g. $80 + $FF => $7F) and not $017F."
        case ZERO_PAGE_X : {
            address = (this->fetchNextByte() + this->idX) & 0xFF;
            break;
        }
        
        case ZERO_PAGE_Y : {
            address = (this->fetchNextByte() + this->idY) & 0xFF;
            break;
        }
        
        // As obelisk guide says: "Relative addressing mode is used by branch instructions (e.g. BEQ, BNE, etc.) which contain a signed 8 bit relative offset (e.g. -128 to +127) which is added to program counter if the condition is true."
        // So, again, i just need to take the next adresses, but, in instructions, i need to do the sum logic in PC
        case RELATIVE : {
            address = (this->pc++);
            break;
        }
        
        // As obelisk guide says: "Instructions using absolute addressing contain a full 16 bit address to identify the target location."
        // Now things got interesting, i need to get the two part of adresses and put them together.
        // And, remember, 6502 is little endian
        case ABSOLUTE : {
            uint8_t LSB = this->fetchNextByte();
            uint8_t MSB = this->fetchNextByte();

            address = (MSB << 8) | LSB;
            break;
        }

        case ABSOLUTE_X : {
            uint8_t LSB = this->fetchNextByte();
            uint8_t MSB = this->fetchNextByte();

            address = ( (MSB << 8) | LSB) + this->idX;

            break;
        }

        case ABSOLUTE_Y : {
            uint8_t LSB = this->fetchNextByte();
            uint8_t MSB = this->fetchNextByte();

            address = ( (MSB << 8) | LSB) + this->idY;

            break;
        }
        
        // As obelisk guide says: "The instruction contains a 16 bit address which identifies the location of the least significant byte of another 16 bit memory address which is the real target of the instruction."
        // So, again, same logic as absolute, but in instruction logic, we need to fetch the byte at position and position +1 to get the effective address.
        // BTW, this have a strange behavior. The +1 sum must be done *just* in least significant byte. So, for example, if we have 0xC0FF, and try to read the next byte, the CPU will try to read at 0xC000 insted of 0xC1000.
        // And thanks gemini for this info, it probably avoided a lot of work
        case INDIRECT : {
            uint8_t LSB = this->fetchNextByte();
            uint8_t MSB = this->fetchNextByte();
            
            uint16_t intermediaryAddressLSB = (MSB << 8) | LSB;
            uint16_t intermediaryAddressMSB = (MSB << 8) | ( (LSB + 1) & 0xFF);
            
            uint8_t finalLSB = this->fetchByteAt(intermediaryAddressLSB);
            uint8_t finalMSB = this->fetchByteAt(intermediaryAddressMSB);

            address = (finalMSB << 8) | finalLSB;

            break;
        }
        
        // As obelisk guide says: "Indexed indirect addressing is normally used in conjunction with a table of address held on zero page. The address of the table is taken from the instruction and the X register added to it (with zero page wrap around) to give the location of the least significant byte of the target address."
        // This is *not* intuitive, because normal indirect can take *any* address.
        case INDIRECT_X : {
            uint8_t zeroPageAdress = (this->fetchNextByte() + this->idX) & 0xFF; // Not necessary, but this maybe be more legible 
            
            uint8_t LSB = this->fetchByteAt(zeroPageAdress);
            uint8_t MSB = this->fetchByteAt( (zeroPageAdress + 1) & 0xFF);
            
            address = (MSB << 8) | LSB;

            break;
        }
        
        // As obelisk guide says: "Indirect indirect addressing is the most common indirection mode used on the 6502. In instruction contains the zero page location of the least significant byte of 16 bit address. The Y register is dynamically added to this value to generated the actual target address for operation."
        // So, it's basically same logic as X, but added *before*
        // One interesting this: This don't have the same bug as indirect
        case INDIRECT_Y : {
            uint8_t zeroPageAdress = this->fetchNextByte(); 
            
            uint8_t LSB = this->fetchByteAt(zeroPageAdress);
            uint8_t MSB = this->fetchByteAt( (zeroPageAdress + 1) & 0xFF);
            
            address = ( (MSB << 8) | LSB) + this->idY;

            break;
        }

        default : {
            throw std::runtime_error("Illegal adressing mode");
        }
    }

    return address;
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
            data = this->fetchByteAt(addressToFetchData);
            this->AND(data);
            return 2; 
                      
        case AND_ZERO_PAGE:
            addressToFetchData = this->getNextAddress(ZERO_PAGE);
            data = this->fetchByteAt(addressToFetchData);
            this->AND(data);
            return 3; 
                      
        case AND_ZERO_PAGE_X:
            addressToFetchData = this->getNextAddress(ZERO_PAGE_X);
            data = this->fetchByteAt(addressToFetchData);
            this->AND(data);
            return 4; 
                      
        case AND_ABSOLUTE:
            addressToFetchData = this->getNextAddress(ABSOLUTE);
            data = this->fetchByteAt(addressToFetchData);
            this->AND(data);
            return 4; 
                      
        case AND_ABSOLUTE_X:
            addressToFetchData = this->getNextAddress(ABSOLUTE_X);
            data = this->fetchByteAt(addressToFetchData);
            this->AND(data);
            return 4; // (+1 if page crossed)
                      
        case AND_ABSOLUTE_Y:
            addressToFetchData = this->getNextAddress(ABSOLUTE_Y);
            data = this->fetchByteAt(addressToFetchData);
            this->AND(data);
            return 4; // (+1 if page crossed)
                      
        case AND_INDIRECT_X:
            addressToFetchData = this->getNextAddress(INDIRECT_X);
            data = this->fetchByteAt(addressToFetchData);
            this->AND(data);
            return 6; 
                      
        case AND_INDIRECT_Y:
            addressToFetchData = this->getNextAddress(INDIRECT_Y);
            data = this->fetchByteAt(addressToFetchData);
            this->AND(data);
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
            this->CLC();
            return 2; 
                      
        case CLD_IMPLIED:
            this->CLD();
            return 2; 
                      
        case CLI_IMPLIED:
            this->CLI();
            return 2; 
                      
        case CLV_IMPLIED:
            this->CLV();
            return 2; 
                      
        case CMP_IMMEDIATE:
            addressToFetchData = this->getNextAddress(IMMEDIATE);
            data = this->fetchByteAt(addressToFetchData);
            this->CMP(data);
            return 2; 
                      
        case CMP_ZERO_PAGE:
            addressToFetchData = this->getNextAddress(ZERO_PAGE);
            data = this->fetchByteAt(addressToFetchData);
            this->CMP(data);
            return 3; 
                      
        case CMP_ZERO_PAGE_X:
            addressToFetchData = this->getNextAddress(ZERO_PAGE_X);
            data = this->fetchByteAt(addressToFetchData);
            this->CMP(data);
            return 4; 
                      
        case CMP_ABSOLUTE:
            addressToFetchData = this->getNextAddress(ABSOLUTE);
            data = this->fetchByteAt(addressToFetchData);
            this->CMP(data);
            return 4; 
                      
        case CMP_ABSOLUTE_X:
            addressToFetchData = this->getNextAddress(ABSOLUTE_X);
            data = this->fetchByteAt(addressToFetchData);
            this->CMP(data);
            return 4; // (+1 if page crossed)
                      
        case CMP_ABSOLUTE_Y:
            addressToFetchData = this->getNextAddress(ABSOLUTE_Y);
            data = this->fetchByteAt(addressToFetchData);
            this->CMP(data);
            return 4; // (+1 if page crossed)
                      
        case CMP_INDIRECT_X:
            addressToFetchData = this->getNextAddress(INDIRECT_X);
            data = this->fetchByteAt(addressToFetchData);
            this->CMP(data);
            return 6; 
                      
        case CMP_INDIRECT_Y:
            addressToFetchData = this->getNextAddress(INDIRECT_Y);
            data = this->fetchByteAt(addressToFetchData);
            this->CMP(data);
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
            this->DEX();
            return 2; 
                      
        case DEY_IMPLIED:
            this->DEY();
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
            this->INX();
            return 2; 
                      
        case INY_IMPLIED:
            this->INY();
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
            this->NOP();
            return 2; 
                      
        case ORA_IMMEDIATE:
            addressToFetchData = this->getNextAddress(IMMEDIATE);
            data = this->fetchByteAt(addressToFetchData);
            this->ORA(data);
            return 2; 
                      
        case ORA_ZERO_PAGE:
            addressToFetchData = this->getNextAddress(ZERO_PAGE);
            data = this->fetchByteAt(addressToFetchData);
            this->ORA(data);
            return 3; 
                      
        case ORA_ZERO_PAGE_X:
            addressToFetchData = this->getNextAddress(ZERO_PAGE_X);
            data = this->fetchByteAt(addressToFetchData);
            this->ORA(data);
            return 4; 
                      
        case ORA_ABSOLUTE:
            addressToFetchData = this->getNextAddress(ABSOLUTE);
            data = this->fetchByteAt(addressToFetchData);
            this->ORA(data);
            return 4; 
                      
        case ORA_ABSOLUTE_X:
            addressToFetchData = this->getNextAddress(ABSOLUTE_X);
            data = this->fetchByteAt(addressToFetchData);
            this->ORA(data);
            return 4; // (+1 if page crossed)
                      
        case ORA_ABSOLUTE_Y:
            addressToFetchData = this->getNextAddress(ABSOLUTE_Y);
            data = this->fetchByteAt(addressToFetchData);
            this->ORA(data);
            return 4; // (+1 if page crossed)
                      
        case ORA_INDIRECT_X:
            addressToFetchData = this->getNextAddress(INDIRECT_X);
            data = this->fetchByteAt(addressToFetchData);
            this->ORA(data);
            return 6; 
                      
        case ORA_INDIRECT_Y:
            addressToFetchData = this->getNextAddress(INDIRECT_Y);
            data = this->fetchByteAt(addressToFetchData);
            this->ORA(data);
            return 5; // (+1 if page crossed)
                      
        case PHA_IMPLIED:
            this->PHA();
            return 3; 
                      
        case PHP_IMPLIED:
            this->PHP();
            return 3; 
                      
        case PLA_IMPLIED:
            this->PLA();
            return 4; 
                      
        case PLP_IMPLIED:
            this->PLP();
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
            this->RTI();
            return 6; 
                      
        case RTS_IMPLIED:
            this->RTS();
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
            this->SEC();
            return 2; 
                      
        case SED_IMPLIED:
            this->SED();
            return 2; 
                      
        case SEI_IMPLIED:
            this->SEI();
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
            this->TAX();
            return 2; 
                      
        case TAY_IMPLIED:
            this->TAY();
            return 2; 
                      
        case TSX_IMPLIED:
            this->TSX();
            return 2; 
                      
        case TXA_IMPLIED:
            this->TXA();
            return 2; 
                      
        case TXS_IMPLIED:
            addressToFetchData = this->getNextAddress(IMPLIED);
            this->TXS(addressToFetchData);
            return 2; 
                      
        case TYA_IMPLIED:
            this->TYA();
            return 2; 

        default:
            return 0;
            // throw std::runtime_error("Invalid opcode. Exiting...");
    }
}

// All instructions below
// TODO: There's some instructions that recive an address and other then recive a data... With time i change the signature of functions to match each case, but i want to have the proper skeleton for now

//TODO: Implement
void CPU::ADC(uint16_t data){}

// Done
// "A logical AND is performed, bit by bit, on the accumulator contents using the contents of a byte of memory."
void CPU::AND(uint8_t data){
    this->acc &= data;

    this->p &= 0b01111101;

    // "Set if A is zero"
    if (this->acc == 0) this->p |= 0b00000010;
    
    // "Set if bit 7 of A is set"
    this->p |= this->acc & 0b10000000;
    
}

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

// Done
// "Set the carry flag to zero."
void CPU::CLC(){
    this->p &= 0b11111110;
}

// Done
// "Sets the decimal mode flag to zero."
void CPU::CLD(){
    this->p &= 0b11110111;
}

// Done
// "Clears the interrupt disable flag allowing normal interrupt requests to be serviced."
void CPU::CLI(){
    this->p &= 0b11111011;
}

// Done
// "Clears the overflow flag."
void CPU::CLV(){
    this->p &= 0b10111111;
}

// Done
// "This instruction compares the contents of the accumulator with another memory held value and sets the zero and carry flags as appropriate."
void CPU::CMP(uint8_t data){
    this->p &= 0b01111100;
    
    // "Set if A >= M"
    if (this->acc >= data) this->p |= 0b00000001;
    
    // "Set if A = M"
    if (this->acc == data) this->p |= 0b00000010;

    // "Set if bit 7 of the result is set"
    this->p |= 0b10000000;
}

//TODO: Implement
void CPU::CPX(uint16_t data){}

//TODO: Implement
void CPU::CPY(uint16_t data){}

//TODO: Implement
void CPU::DEC(uint16_t data){}

// Done
// "Subtracts one from the X register setting the zero and negative flags as appropriate."
void CPU::DEX(){
    this->idX--;
    
    this->p &= 0b01111101;

    // "Set if X is zero"
    if (this->idX == 0) this->p |= 0b00000010;
    
    // "Set if bit 7 of X is set"
    this->p |= this->idX & 0b10000000;
}

// Done
// "Subtracts one from the Y register setting the zero and negative flags as appropriate."
void CPU::DEY(){
    this->idY--;
    
    this->p &= 0b01111101;

    // "Set if Y is zero"
    if (this->idY == 0) this->p |= 0b00000010;
    
    // "Set if bit 7 of Y is set"
    this->p |= this->idY & 0b10000000;
}

//TODO: Implement
void CPU::EOR(uint16_t data){}

//TODO: Implement
void CPU::INC(uint16_t data){}

// Done
// "Adds one to the X register setting the zero and negative flags as appropriate."
void CPU::INX(){
    this->idX++;
    
    this->p &= 0b01111101;

    // "Set if X is zero"
    if (this->idX == 0) this->p |= 0b00000010;
    
    // "Set if bit 7 of X is set"
    this->p |= this->idX & 0b10000000;
}

// Done
// "Adds one to the Y register setting the zero and negative flags as appropriate."
void CPU::INY(){
    this->idY++;
    
    this->p &= 0b01111101;

    // "Set if Y is zero"
    if (this->idY == 0) this->p |= 0b00000010;
    
    // "Set if bit 7 of Y is set"
    this->p |= this->idY & 0b10000000;
}

// Done
// "Sets the program counter to the address specified by the operand."
// "An original 6502 has does not correctly fetch the target address if the indirect vector falls on a page boundary (e.g. $xxFF where xx is any value from $00 to $FF). In this case fetches the LSB from $xxFF as expected but takes the MSB from $xx00. This is fixed in some later chips like the 65SC02 so for compatibility always ensure the indirect vector is not at the end of the page."
void CPU::JMP(uint16_t finalAddress){
    this->pc = finalAddress;
}

// Done
// "(JSR will first push the high-byte of the return address [PC+2] onto the stack, then the low-byte. The stack will then contain, seen from the bottom or from the most recently added byte, [PC+2]-L [PC+2]-H.)"
void CPU::JSR(uint16_t finalAddress){
    uint8_t MSB = ( (this->pc - 1) & 0xFF00) >> 8;
    uint8_t LSB = ( (this->pc - 1) & 0x00FF);

    this->stackPush(MSB);
    this->stackPush(LSB);

    this->pc = finalAddress;
}

//TODO: Implement
void CPU::LDA(uint16_t data){}

//TODO: Implement
void CPU::LDX(uint16_t data){}

//TODO: Implement
void CPU::LDY(uint16_t data){}

//TODO: Implement
void CPU::LSR(uint16_t data){}

// Done
// Literally, just do nothing
void CPU::NOP(){
    return;
}

// DONE
// "An inclusive OR is performed, bit by bit, on the accumulator contents using the contents of a byte of memory."
void CPU::ORA(uint8_t data){
    this->acc |= data;

    this->p &= 0b01111101;

    // "Set if A is zero"
    if (this->acc == 0) this->p |= 0b00000010;
    
    // "Set if bit 7 of A is set"
    this->p |= this->acc & 0b10000000;
    
}

// Done
// "Pushes a copy of the accumulator on to the stack."
void CPU::PHA(){
    this->stackPush(this->acc);
}

// Done
// "Pushes a copy of the status flags on to the stack."
void CPU::PHP(){
    this->stackPush(this->p | 0b00110000);
}

// Done
// "Pulls an 8 bit value from the stack and into the accumulator. The zero and negative flags are set as appropriate."
void CPU::PLA(){
    uint8_t value = this->stackPop();
       
    this->p &= 0b01111101;

    this->acc = value;

    // "Set if A is zero"
    if (this->acc == 0) this->p |= 0b00000010;
    
    // "Set if bit 7 of A is set"
    this->p |= this->acc & 0b10000000;
}

// Done
// "Pulls an 8 bit value from the stack and into the processor flags. The flags will take on new states as determined by the value pulled."
void CPU::PLP(){
    uint8_t value = this->stackPop();
       
    this->p = value;
}

//TODO: Implement
void CPU::ROL(uint16_t data){}

//TODO: Implement
void CPU::ROR(uint16_t data){}

// Done
// "The status register is pulled with the break flag and bit 5 ignored. Then PC is pulled from the stack.
// From "ignored" he's saying: "Let it as original status register"
void CPU::RTI(){
    uint8_t mask = 0b11001111;

    uint8_t newPFlag = this->stackPop() & mask;

    this->p = (this->p & (~mask)) | newPFlag;
  
    uint8_t LSB = this->stackPop();
    uint8_t MSB = this->stackPop();

    this->pc = ( (MSB << 8) | LSB);
}

// Done
// Here obelisk guide became a little bit confuse. In docs: "The RTS instruction is used at the end of a subroutine to return to the calling routine. It pulls the program counter (minus one) from the stack." But this don't make sense (or i've had a bad interpretation), because it seems like: "Ok, pulls the previous adresses from stack (that is PC before the JSR minus 1) and then sum one" => But this will result on a infinite loop.
/*Here https://www.masswerk.at/6502/6502_instruction_set.html#RTS was more clear: "pull PC, PC+1 -> PC", which makes total sense with JSR: 
    push (PC+2),
    operand 1st byte -> PCL
    operand 2nd byte -> PCH
    N	Z	C	I	D	V
    -	-	-	-	-	-
    addressing	assembler	opc	bytes	cycles
    absolute	JSR oper	20	3	6  
 */
/*
 JSR and RTS affect the stack as the return address is pushed onto or pulled from the stack, respectively.
(JSR will first push the high-byte of the return address [PC+2] onto the stack, then the low-byte. The stack will then contain, seen from the bottom or from the most recently added byte, [PC+2]-L [PC+2]-H.)
 */
void CPU::RTS(){
    uint8_t LSB = this->stackPop();
    uint8_t MSB = this->stackPop();

    this->pc = ( (MSB << 8) | LSB) + 1;
}

//TODO: Implement
void CPU::SBC(uint16_t data){}

// Done
// "Set the carry flag to one."
void CPU::SEC(){
    this->p |= 0b00000001;
}

// Done
// "Set the decimal mode flag to one."
void CPU::SED(){
    this->p |= 0b00001000;
}

// Done
// "Set the interrupt disable flag to one."
void CPU::SEI(){
    this->p |= 0b00000100;
}

//TODO: Implement
void CPU::STA(uint16_t data){}

//TODO: Implement
void CPU::STX(uint16_t data){}

//TODO: Implement
void CPU::STY(uint16_t data){}

// Done
// "Copies the current contents of the accumulator into the X register and sets the zero and negative flags as appropriate."
void CPU::TAX(){
    this->idX = this->acc;

    this->p &= 0b01111101;

    // "Set if X is zero"
    if (this->idX == 0) this->p |= 0b00000010;
    
    // "Set if bit 7 of X is set"
    this->p |= this->idX & 0b10000000;
}

// Done
// "Copies the current contents of the accumulator into the Y register and sets the zero and negative flags as appropriate."
void CPU::TAY(){
    this->idY = this->acc;
    
    this->p &= 0b01111101;
    
    // "Set if Y is zero"
    if (this->idY == 0) this->p |= 0b00000010;
    
    // "Set if bit 7 of Y is set"
    this->p |= this->idY & 0b10000000;
}

//DONE
// "Copies the current contents of the stack register into the X register and sets the zero and negative flags as appropriate."
void CPU::TSX(){
    this->idX = this->sp;
    
    this->p &= 0b01111101;
    
    // "Set if X is zero"
    if (this->idX == 0) this->p |= 0b00000010;
    
    // "Set if bit 7 of X is set"
    this->p |= this->idX & 0b10000000;
}

// Done
// "Copies the current contents of the X register into the accumulator and sets the zero and negative flags as appropriate."
void CPU::TXA(){
    this->acc = this->idX;
    
    this->p &= 0b01111101;

    // "Set if acc is zero"
    if (this->acc == 0) this->p |= 0b00000010;
    
    // "Set if bit 7 of acc is set"
    this->p |= this->acc & 0b10000000;
}

//TODO: Implement
void CPU::TXS(uint16_t data){}

// Done
// "Copies the current contents of the Y register into the accumulator and sets the zero and negative flags as appropriate."
void CPU::TYA(){
    this->acc = this->idY;
    
    this->p &= 0b01111101;

    // "Set if acc is zero"
    if (this->acc == 0) this->p |= 0b00000010;
    
    // "Set if bit 7 of acc is set"
    this->p |= this->acc & 0b10000000;
}
