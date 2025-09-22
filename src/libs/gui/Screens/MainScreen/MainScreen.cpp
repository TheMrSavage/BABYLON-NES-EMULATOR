#include "MainScreen.hpp"
#include "SDL3/SDL_gpu.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_render.h"
#include "imgui.h"
#include <cstdint>
#include <cstdlib>
#include <iostream>

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
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Game")) {
            if (ImGui::MenuItem("Open file")) {
                std::cout << "To do" << std::endl;
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
    }
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

MainScreen::~MainScreen() {
// SDL_DestroyTexture(this->GAME_DRAW_PIXEL_MATRIX);
}
