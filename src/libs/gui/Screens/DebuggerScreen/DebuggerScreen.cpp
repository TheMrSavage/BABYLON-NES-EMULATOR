#include "DebuggerScreen.hpp"
#include "imgui.h"

DebuggerScreen::DebuggerScreen(SDL_Renderer * interface_renderer,
                    std::queue<event_return>& event_pool) : Screen(interface_renderer, event_pool) {

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
    ImGui::Text("Program counter: ");
    ImGui::Text("Stack pointer: ");
    ImGui::Text("Accumulator: ");
    ImGui::Text("Index Register X: ");
    ImGui::Text("Index Register Y: ");
    ImGui::Text("Processor Status Register (P): ");

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
