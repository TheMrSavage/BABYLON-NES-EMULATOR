#include "DebuggerScreen.hpp"
#include "events.hpp"
#include "imgui.h"
#include <cstdint>
#include <vector>

#define OnOffString(x) \
    (x) ? ("On") : ("Off")

DebuggerScreen::DebuggerScreen(SDL_Renderer * interface_renderer,
                    std::queue<event_return>& event_pool) : Screen(interface_renderer, event_pool) {

}

// TODO: Remove the memory reference 
// TODO: Create a new schema for BUS 
void DebuggerScreen::setCpuDebuggerInfo(   
                const uint16_t& cpuPc,
                const uint8_t&  cpuSp,
                const uint8_t&  cpuAcc,
                const uint8_t&  cpuIdX,
                const uint8_t&  cpuIdY,
                const uint8_t&  cpuP,
                const std::vector<uint8_t>& memory
                ) {
    this->cpuDebuggerInfo = new CpuDebuggerInfo{
            cpuPc,
            cpuSp,
            cpuAcc,
            cpuIdX,
            cpuIdY,
            cpuP,
            memory
    };
}

void DebuggerScreen::show(){
    float padding = 5;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(padding, padding));

    this->showRoomMemory();
    this->openCPUDebugger();
    this->openAPUDebugger();
    this->openPPUDebugger();
    ImGui::PopStyleVar(2);
}

void DebuggerScreen::openCPUDebugger() {
    ImGui::Begin("CPU Debugger");
    
    if (ImGui::Button("")) {
        event_return stepByStep = {
            INTERFACE_STEP_BY_STEP_DEBUGGER,
            nullptr
        };

        this->pushQueueElement(stepByStep);
    }
    ImGui::SameLine();
    if (ImGui::Button("")) {
        event_return nextStep = {
            INTERFACE_NEXT_STEP_DEBUGGER,
            nullptr
        };

        this->pushQueueElement(nextStep);
    }
    
    ImGui::Text("Current instruction: %s",      this->getCurrentInstructionString());
    ImGui::Text("Program counter:     0x%04x",  this->cpuDebuggerInfo == nullptr ? 0 :  this->cpuDebuggerInfo->cpuPc);
    ImGui::Text("Stack pointer:       0x%02x",  this->cpuDebuggerInfo == nullptr ? 0 :  this->cpuDebuggerInfo->cpuSp);
    ImGui::Text("Accumulator:         0x%02x",  this->cpuDebuggerInfo == nullptr ? 0 :  this->cpuDebuggerInfo->cpuAcc);
    ImGui::Text("Index Register X:    0x%02x",  this->cpuDebuggerInfo == nullptr ? 0 :  this->cpuDebuggerInfo->cpuIdX);
    ImGui::Text("Index Register Y:    0x%02x",  this->cpuDebuggerInfo == nullptr ? 0 :  this->cpuDebuggerInfo->cpuIdY);
    ImGui::Text("Status Register (P): 0x%02x",  this->cpuDebuggerInfo == nullptr ? 0 :  this->cpuDebuggerInfo->cpuP);
    
    this->showStatusRegisterInfo();

    ImGui::End();
}

void DebuggerScreen::showStatusRegisterInfo () {
    uint8_t statusRegister = this->cpuDebuggerInfo == nullptr ? 0 : this->cpuDebuggerInfo->cpuP;

    uint8_t carry               = statusRegister & 0b00000001; 
    uint8_t zero                = statusRegister & 0b00000010; 
    uint8_t interruptDisable    = statusRegister & 0b00000100; 
    uint8_t decimal             = statusRegister & 0b00001000; 
    uint8_t bFlag               = statusRegister & 0b00010000; 
    uint8_t alwaysOneFlag       = statusRegister & 0b00100000;
    uint8_t overflow            = statusRegister & 0b01000000; 
    uint8_t negative            = statusRegister & 0b10000000; 
    
    ImGui::TextWrapped("Carry Flag: %s | Zero flag: %s | Interrupt Disable flag: %s | Decimal flag: %s | B Flag: %s | Always One flag: %s | Overflow flag: %s | Negative flag: %s" ,
            OnOffString(carry),
            OnOffString(zero),
            OnOffString(interruptDisable),
            OnOffString(decimal),
            OnOffString(bFlag),
            OnOffString(alwaysOneFlag),
            OnOffString(overflow),
            OnOffString(negative)
        );
}

const char * DebuggerScreen::getCurrentInstructionString() {
    if (this->cpuDebuggerInfo == nullptr) {
        return "NO CPU ATTACHED";
    }

    uint8_t opcode = this->cpuDebuggerInfo->cpuMemory[this->cpuDebuggerInfo->cpuPc];

    if (this->opcodesToInstructionsMap.find(opcode) == this->opcodesToInstructionsMap.end()) {
        return "NOT IMPLEMENTED";
    }

    return this->opcodesToInstructionsMap.at(opcode);
}

// TODO
void DebuggerScreen::openAPUDebugger() {

}

// TODO
void DebuggerScreen::openPPUDebugger() {

}

void DebuggerScreen::showRoomMemory() {
    ImVec4 yellowVec(1, 1, 0, 1);

    ImGui::Begin("Room memory");
    
    ImGui::TextColored(yellowVec, "ROM Memory");
    ImGui::BeginChild("ROM Memory");
    
    if (this->cpuDebuggerInfo != nullptr) {
        const std::vector<uint8_t>& memory = cpuDebuggerInfo->cpuMemory;
        
        for (int pos = 0; pos < (int)memory.size(); pos++) {
            if (pos % 16 == 0) {
                ImGui::Text("%04x: ", pos);
                ImGui::SameLine();
            }
            if (pos == this->cpuDebuggerInfo->cpuPc) ImGui::TextColored(yellowVec, "%02x", memory[pos]);
            else ImGui::Text("%02x", memory[pos]);
            if ( (pos + 1) % 16 != 0) ImGui::SameLine();
        }
    }

    ImGui::EndChild();

    ImGui::End();
}
