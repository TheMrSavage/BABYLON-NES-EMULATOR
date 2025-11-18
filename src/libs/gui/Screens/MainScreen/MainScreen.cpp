#include "MainScreen.hpp"
#include "imgui.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <ios>
#include <iostream>
#include <nfd.h>
#include <queue>
#include <vector>

MainScreen::MainScreen(
        SDL_Renderer * interface_renderer,
        std::queue<event_return>& event_pool,
        bool& showDebugger) : Screen(interface_renderer, event_pool), showDebugger(showDebugger) {
    this->GAME_DRAW_PIXEL_MATRIX = SDL_CreateTexture(
            this->INTERFACE_RENDERER,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_STREAMING,
            this->GAME_DRAW_PIXEL_MATRIX_WIDTH,
            this->GAME_DRAW_PIXEL_MATRIX_HEIGHT
        );
}

void MainScreen::show() {
    float padding = 5;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(padding, padding));

    if (showMenu && ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Game")) {
            if (ImGui::MenuItem("Open file")) {
                std::vector<unsigned char> roomFile = openFileFunction();
                
                struct event_return event = {
                    INTERFACE_ROOM_OPEN_EVENT,
                    roomFile
                };
                this->pushQueueElement(event);
            }
            
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Settings")) {
            if (ImGui::MenuItem("Controller")) {
                std::cout << "To do" << std::endl;
            }

            if (ImGui::MenuItem("Music")) {
                std::cout << "To do" << std::endl;
            }
            
            if (ImGui::MenuItem("Screen")) {
                std::cout << "To do" << std::endl;
            }
            
            if (ImGui::MenuItem("Debbuging")) {
                if (!this->showDebugger) {
                    this->showDebugger = true;
                }   
            }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
        }
        
        ImGui::PopStyleVar(2);

        ImVec2 viewportWorkPos = ImGui::GetMainViewport()->WorkPos;
        
        // viewportWorkPos[1] += 10;
        ImGui::SetNextWindowPos(viewportWorkPos);
        ImVec2 viewportWorkSize = ImGui::GetMainViewport()->WorkSize; 
        ImGui::SetNextWindowSize(viewportWorkSize);

        ImGui::Begin("GameDraw", 
                 nullptr,
                 ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoTitleBar |
                 ImGuiWindowFlags_NoBringToFrontOnFocus |
                 ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoScrollbar |
                 ImGuiWindowFlags_NoBackground |
                 ImGuiWindowFlags_NoDecoration
                 );
        
        this->randomizeMatrix();

        ImGui::Image(
        (ImTextureID)(intptr_t)this->GAME_DRAW_PIXEL_MATRIX,
        viewportWorkSize
        );

        ImGui::End();

        if (ImGui::IsKeyReleased(ImGuiKey_Escape))
            this->showMenu = !this->showMenu;
}

void MainScreen::randomizeMatrix() {
    unsigned char * pixels;
    int pitch;

    SDL_LockTexture(this->GAME_DRAW_PIXEL_MATRIX, NULL, (void **)&pixels, &pitch);
    
    for (int i = 0; i < pitch * this->GAME_DRAW_PIXEL_MATRIX_HEIGHT; i += 4) {
        pixels[i] = 0xFF;   
        pixels[i + 1] = rand() % 255;   
        pixels[i + 2] = rand() % 255;   
        pixels[i + 3] = rand() % 255;   
    }

    SDL_UnlockTexture(this->GAME_DRAW_PIXEL_MATRIX);
}

std::vector<uint8_t> MainScreen::openFileFunction() {
    nfdchar_t * roomPath = nullptr;
    nfdresult_t result = NFD_OpenDialog(nullptr, nullptr, &roomPath);

    if ( result == NFD_OKAY) {
        std::ifstream room(roomPath, std::ios::binary);
        
        free(roomPath);

        if (room) {
            std::vector<uint8_t> roomData;
            char byte;

            while (room.get(byte)) {
                roomData.push_back(byte);
            }

            return roomData;
        }
    }
    
    return std::vector<uint8_t>();
}

MainScreen::~MainScreen() {
    SDL_DestroyTexture(this->GAME_DRAW_PIXEL_MATRIX);
}
