#include "DebuggerScreen.hpp"
#include "imgui.h"
#include <cstdint>
#include <vector>

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
    
    ImGui::Button("");
    ImGui::SameLine();
    ImGui::Button("");
    
    ImGui::Text("Current instruction: %s",      this->getCurrentInstructionString());
    ImGui::Text("Program counter:     0x%04x",  this->cpuDebuggerInfo == nullptr ? 0 :  this->cpuDebuggerInfo->cpuPc);
    ImGui::Text("Stack pointer:       0x%02x",  this->cpuDebuggerInfo == nullptr ? 0 :  this->cpuDebuggerInfo->cpuSp);
    ImGui::Text("Accumulator:         0x%02x",  this->cpuDebuggerInfo == nullptr ? 0 :  this->cpuDebuggerInfo->cpuAcc);
    ImGui::Text("Index Register X:    0x%02x",  this->cpuDebuggerInfo == nullptr ? 0 :  this->cpuDebuggerInfo->cpuIdX);
    ImGui::Text("Index Register Y:    0x%02x",  this->cpuDebuggerInfo == nullptr ? 0 :  this->cpuDebuggerInfo->cpuIdY);
    ImGui::Text("Status Register (P): 0x%02x",  this->cpuDebuggerInfo == nullptr ? 0 :  this->cpuDebuggerInfo->cpuP);

    ImGui::End();
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
