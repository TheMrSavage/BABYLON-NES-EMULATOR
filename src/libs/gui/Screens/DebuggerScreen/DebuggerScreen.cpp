#include "DebuggerScreen.hpp"
#include "imgui.h"

DebuggerScreen::DebuggerScreen(SDL_Renderer * interface_renderer,
                    std::queue<event_return>& event_pool) : Screen(interface_renderer, event_pool) {

}

void DebuggerScreen::setCpuDebuggerInfo(   
                const uint16_t& cpuPc,
                const uint8_t&  cpuSp,
                const uint8_t&  cpuAcc,
                const uint8_t&  cpuIdX,
                const uint8_t&  cpuIdY,
                const uint8_t&  cpuP) {
    this->cpuDebuggerInfo = new CpuDebuggerInfo{
            cpuPc,
            cpuSp,
            cpuAcc,
            cpuIdX,
            cpuIdY,
            cpuP
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
    
    ImGui::Button("+ | -");
    ImGui::SameLine();
    ImGui::Button(">");
    
    ImGui::Text("Current instruction: ");
    ImGui::Text("Program counter: %d", this->cpuDebuggerInfo == nullptr ? 0 :  this->cpuDebuggerInfo->cpuPc);
    ImGui::Text("Stack pointer: %d",   this->cpuDebuggerInfo == nullptr ? 0 :  this->cpuDebuggerInfo->cpuSp);
    ImGui::Text("Accumulator: %d",     this->cpuDebuggerInfo == nullptr ? 0 :  this->cpuDebuggerInfo->cpuAcc);
    ImGui::Text("Index Register X: %d",this->cpuDebuggerInfo == nullptr ? 0 :  this->cpuDebuggerInfo->cpuIdX);
    ImGui::Text("Index Register Y: %d",this->cpuDebuggerInfo == nullptr ? 0 :  this->cpuDebuggerInfo->cpuIdY);
    ImGui::Text("Processor Status Register (P): %d", this->cpuDebuggerInfo == nullptr ? 0 : this->cpuDebuggerInfo->cpuP);

    ImGui::End();
}

// TODO
void DebuggerScreen::openAPUDebugger() {

}

// TODO
void DebuggerScreen::openPPUDebugger() {

}

void DebuggerScreen::showRoomMemory() {
    ImGui::Begin("Room memory");
    
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "ROM Memory");
    ImGui::BeginChild("ROM Memory");
    
    // TODO

    ImGui::EndChild();

    ImGui::End();
}
