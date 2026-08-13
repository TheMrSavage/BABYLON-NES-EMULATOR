#include "EmulatorController.hpp"
#include "events.hpp"
#include "gui/Interface.hpp"
#include "nes/rp2a03/cpu/Cpu_6502.hpp"
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include <bus/Bus.hpp>

EmulatorController::EmulatorController() {}

// DONE 
EmulatorController::~EmulatorController() {}

void EmulatorController::start() {
    this->interface = std::make_unique<Interface>(this->interface_event_pool);
    
    while (true) {
        while (!this->interface_event_pool.empty()) {
            event_return interface_event_return = this->interface_event_pool.front();
            this->interface_event_pool.pop();

            EVENT_ENUM interface_event = interface_event_return.e;

            switch (interface_event) {
                case INTERFACE_CLOSE_EVENT : {
                    return;
                    break;
                }

                case INTERFACE_ROOM_OPEN_EVENT: {
                    if (!interface_event_return.data.has_value()) {
                        std::cout << "[-] NO DATA!" << std::endl;
                        break;
                    }

                    std::vector<uint8_t> room =  std::any_cast<std::vector<unsigned char>>(interface_event_return.data.value());
                    if (room.empty()) {
                        std::cout << "[-] NO DATA!" << std::endl;
                        break;
                    }

                    this->handleRoom(room);

                    break;
                }
                
                case INTERFACE_STEP_BY_STEP_DEBUGGER: {
                    this->stepByStepDebugger = !this->stepByStepDebugger;
                    break;
                }
                
                case INTERFACE_NEXT_STEP_DEBUGGER: {
                    this->execNextStep = true;   
                    break;
                }
            }
        }

       this->interface->render();
    }
}

void EmulatorController::handleRoom(const std::vector<uint8_t>& room) {
    /*for (uint8_t d : room) {
        std::cout <<  std::hex << std::setw(2) << (int)d << " ";
    }
    std::cout << std::endl;
    
    // As we want to debug 6502 CPU first we don't need (and don't want) to validate the file header
    if (!this->isValidRoom(room)) {
        std::cout << "[-] Invalid room!" << std::endl;
        return;
    }

    std::cout << "[+] Valid room!" << std::endl;*/
    
    std::thread cpuTest(&EmulatorController::mockCPU, this, room);
    
    cpuTest.detach(); 
}

// TODO: Validate not only the header, but total size and other aspects too...
bool EmulatorController::isValidRoom(const std::vector<uint8_t>& room) {
    const std::vector<uint8_t> header = {
        0x4e, // "N" 
        0x45, // "E"
        0x53, // "S"
        0x1A}; // \x1A
    
    return std::equal(
        room.begin(),
        room.begin() + 4,
        header.begin()
    );
}

// This should be (obviously) removed after all CPU tests
void EmulatorController::mockCPU(const std::vector<uint8_t>& room) {
    if (room.size() > 0xFFFF + 1) {
        std::cout << "Invalid room size!" << std::endl;
        return;
    }

    std::vector<uint8_t> mockedMemory(0xFFFF + 1); // 0 to 0xFFFF
    
    Bus bus;
    
    bus.setMemory(mockedMemory);

    if (bus.loadMemory(room)) {
        std::cout << "[+] ROM Loaded!";
    }
    else {
        std::cout << "[-] Error while reading ROM!";
        return;
    }

    const std::vector<uint8_t>& busMemory = bus.returnMemoryCopy();
    
    CPU cpu(bus);
    
    this->interface->setDebuggerScreenCpuInfo(      
        cpu.returnPc(),
        cpu.returnSp(),
        cpu.returnAcc(),
        cpu.returnIdX(),
        cpu.returnIdY(),
        cpu.returnP(),
        busMemory
    );
    
    int cycles = 0;
    while (true) {
        
        if ( !this->stepByStepDebugger || (this->stepByStepDebugger && this->execNextStep) ) {
            cycles = cpu.executeNextInstruction();
            this->execNextStep = false;
        }

        if (this->stepByStepDebugger) std::this_thread::sleep_for(std::chrono::milliseconds(1));
        else std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
}
