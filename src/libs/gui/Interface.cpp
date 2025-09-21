#include "Interface.hpp"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"
#include <cstdlib>
#include <iostream>
#include <ostream>

Interface::Interface() {
    setupSdl();
    setupImGui();
}

void Interface::setupSdl() {

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout << "Error: SDL_Init(): " << SDL_GetError() << std::endl;
        exit(1);
    }

    this->main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    this->window = SDL_CreateWindow("NES Emulator", 
            (int)(1920 * this->main_scale), 
            (int)(1080 * this->main_scale), 
            WINDOW_FLAGS);
    
    if (window == nullptr) {
        std::cout << "Error: SDL_CreateWindow(): " << SDL_GetError() << std::endl;
        exit(1);
    }
    
    this->renderer = SDL_CreateRenderer(this->window, nullptr);
    SDL_SetRenderVSync(this->renderer, 1);

    if (this->renderer == nullptr) {
        std::cout << "Error: SDL_CreateRenderer(): " << SDL_GetError() << std::endl;
        exit(1);
    }   

    SDL_SetWindowPosition(this->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(this->window);
}

void Interface::setupImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    this->imguiIO = ImGui::GetIO();

    this->imguiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    
    ImGui::StyleColorsDark();
    
    ImGuiStyle& style = ImGui::GetStyle();
    
    style.ScaleAllSizes(this->main_scale);
    style.FontScaleDpi = this->main_scale;

    ImGui_ImplSDL3_InitForSDLRenderer(this->window, this->renderer);
    ImGui_ImplSDLRenderer3_Init(this->renderer);
}

void Interface::render() {
    while (true) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);

            if ( (event.type == SDL_EVENT_QUIT) || (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(this->window)) ) {
                return;
            }
        }

        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Test");
        ImGui::Text("Some test text");
        ImGui::End();

        ImGui::Render();
        SDL_SetRenderScale(this->renderer, this->imguiIO.DisplayFramebufferScale.x, this->imguiIO.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColorFloat(this->renderer,  CLEAR_COLOR.x, CLEAR_COLOR.y, CLEAR_COLOR.z, CLEAR_COLOR.w);
        SDL_RenderClear(this->renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), this->renderer);
        SDL_RenderPresent(this->renderer);
    }   
}


Interface::~Interface() {
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}
