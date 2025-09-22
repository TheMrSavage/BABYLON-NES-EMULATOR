#include "MainScreen.hpp"
#include "imgui.h"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <iterator>
#include <nfd.h>
#include <vector>

MainScreen::MainScreen(SDL_Renderer * interface_renderer) : INTERFACE_RENDERER(interface_renderer) {
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
                openFileFunction();
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

const std::vector<unsigned char> MainScreen::getRoomData() {
    nfdchar_t * roomPath = nullptr;
    nfdresult_t result = NFD_OpenDialog(nullptr, nullptr, &roomPath);

    if ( result == NFD_OKAY) {
        std::ifstream room(roomPath, std::ios::binary);
        
        if (room) {
            const std::vector<unsigned char> roomData(std::istream_iterator<char>(room), {});

            return roomData;
        }
    }
    
    return std::vector<unsigned char>();
}

void MainScreen::openFileFunction() {
    const std::vector<unsigned char> roomData = getRoomData();

    for (unsigned char d : roomData) {
        std::cout <<  std::hex << std::setw(2) << (int)d << " ";
    }
    std::cout << std::endl;
    
    if (isValidRoom(roomData) ) {
        std::cout << "VALID ROOM!";
    }
    else {
        std::cout << "INVALID ROOM";
    }
    std::cout << std::endl;

}

// TODO: Validate not only the header, but total size and other aspects too...
bool MainScreen::isValidRoom(const std::vector<unsigned char>& room) {
    const std::vector<unsigned char> header= {
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

MainScreen::~MainScreen() {
// SDL_DestroyTexture(this->GAME_DRAW_PIXEL_MATRIX);
}
