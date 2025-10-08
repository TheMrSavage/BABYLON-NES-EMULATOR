#include "EmulatorController.hpp"
#include "events.hpp"
#include <any>
#include <iomanip>
#include <iostream>
#include <vector>

EmulatorController::EmulatorController() {
    this->nes = new Nes;
}

// TODO: Put smart pointers here too...
EmulatorController::~EmulatorController() {
    delete this->interface; 
    delete this->nes;
}

void EmulatorController::start() {
    this->interface = new Interface(this->interface_event_pool);
    
    while (true) {
        while (!this->interface_event_pool.empty()) {
            event_return interface_event_return = this->interface_event_pool.front();
            this->interface_event_pool.pop();

            EVENT_ENUM interface_event = interface_event_return.e;

            switch (interface_event) {
                case INTERFACE_CLOSE_EVENT:
                    return;
                    break;

                case INTERFACE_ROOM_OPEN_EVENT:
                    if (!interface_event_return.data.has_value()) {
                        std::cout << "[-] NO DATA!" << std::endl;
                    }

                    std::vector<unsigned char> room =  std::any_cast<std::vector<unsigned char>>(interface_event_return.data.value());
                    this->handleRoom(room);

                    break;
            }
        }

       this->interface->render();
    }
}

void EmulatorController::handleRoom(const std::vector<unsigned char>& room) {
    for (unsigned char d : room) {
        std::cout <<  std::hex << std::setw(2) << (int)d << " ";
    }
    std::cout << std::endl;

    if (!this->isValidRoom(room)) {
        std::cout << "[-] Invalid room!" << std::endl;
        return;
    }

    std::cout << "[+] Valid room!" << std::endl;

}

// TODO: Validate not only the header, but total size and other aspects too...
bool EmulatorController::isValidRoom(const std::vector<unsigned char>& room) {
    const std::vector<unsigned char> header = {
        0x4e, // "N" 
        0x45, // "E"
        0x53, // "S"
        0x1A}; // \x1A
    
    return std::equal(
        room.begin(),
        room.begin() + 3,
        header.begin()
    );
}
