#include "MainScreen.hpp"
#include "imgui.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <ios>
#include <iostream>
#include <iterator>
#include <nfd.h>
#include <queue>
#include <vector>

MainScreen::MainScreen(SDL_Renderer * interface_renderer,
        std::queue<event_return>& event_pool) : Screen(interface_renderer, event_pool) {
    this->GAME_DRAW_PIXEL_MATRIX = SDL_CreateTexture(
            this->INTERFACE_RENDERER,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_STREAMING,
            this->GAME_DRAW_PIXEL_MATRIX_WIDTH,
            this->GAME_DRAW_PIXEL_MATRIX_HEIGHT
        );
}

void MainScreen::show() {
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
                std::cout << "To do" << std::endl;
            }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
        }
        
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
    
    for (int i = 0; i < pitch * this->GAME_DRAW_PIXEL_MATRIX_HEIGHT; i++) {
     pixels[i] = rand() % 255;   
    }

    SDL_UnlockTexture(this->GAME_DRAW_PIXEL_MATRIX);
}

std::vector<unsigned char> MainScreen::openFileFunction() {
    nfdchar_t * roomPath = nullptr;
    nfdresult_t result = NFD_OpenDialog(nullptr, nullptr, &roomPath);

    if ( result == NFD_OKAY) {
        std::ifstream room(roomPath, std::ios::binary);
        
        free(roomPath);

        if (room) {
            const std::vector<unsigned char> roomData(std::istream_iterator<char>(room), {});

            return roomData;
        }
    }
    
    return std::vector<unsigned char>();
}

MainScreen::~MainScreen() {
    SDL_DestroyTexture(this->GAME_DRAW_PIXEL_MATRIX);
}
