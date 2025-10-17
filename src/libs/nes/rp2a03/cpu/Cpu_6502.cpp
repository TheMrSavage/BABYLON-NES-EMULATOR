#include "Cpu_6502.hpp"

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
