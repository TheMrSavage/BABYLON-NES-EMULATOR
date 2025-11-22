#include "Bus.hpp"
#include <cstdint>

// TODO: After debuging the Bus read-write for 6502 tests, change the value to MEMORY_SIZE 
Bus::Bus() : memory(0x10000)  {

}

void Bus::setDebugMode() {
    this->debbuging = true;
}

void Bus::writeMemoryAt(uint16_t address, uint8_t data) {
    this->memory[address] = data;
    
    if (this->debbuging) {
        BUS_OPERATION operation = {
            address,
            data,
            BUS_OPERATION_TYPES_ENUM::BUS_WRITE
        };

        this->operations.push_back(operation);
    }

    /*
     * TODO: After the debuging on Bus read-write for 6502 tests, start to really work here
     * if (address <= 0x1FFF) {
        this->memory[address & 0x07FF] = data;
    }
    else if (address <= 0x3FFF) {
        // TODO: Delegate to PPU using the registers enum logic and AND to mirror
    }
    else if (address <= 0x4017) {
        // TODO: Delegate to APU and I/O logic
    }
    else if (address <= 0x401F) {
        // TODO: "APU and I/O functionality that is normally disabled. See CPU Test Mode."
    }
    else {
        // TODO: Delegate to catridge
    }*/
}

uint8_t Bus::readByteAt(uint16_t address) {
    uint8_t data = this->memory[address];

    if (debbuging) {
        BUS_OPERATION operation = {
            address,
            data,
            BUS_OPERATION_TYPES_ENUM::BUS_READ
        };

        this->operations.push_back(operation);
    }

    return data;
    /*
     * TODO: After the debuging on Bus read-write for 6502 tests, start to really work here
     * if (address <= 0x1FFF) {
        return this->memory[address & 0x07FF];
    }
    else if (address <= 0x3FFF) {
        // TODO: Delegate to PPU using the registers enum logic and AND to mirror
    }
    else if (address <= 0x4017) {
        // TODO: Delegate to APU and I/O logic
    }
    else if (address <= 0x401F) {
        // TODO: "APU and I/O functionality that is normally disabled. See CPU Test Mode."
    }
    else {
        // TODO: Delegate to catridge
    }*/
}

// Return true if memory was loaded
bool Bus::loadMemory(const std::vector<uint8_t>& room) {
    if (room.size() > this->MEMORY_SIZE) return false;

    this->memory.assign(room.begin(), room.end());

    return true;
}
