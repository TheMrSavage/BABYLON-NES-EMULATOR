#include "Interface.hpp"
#include "Screens/MainScreen/MainScreen.hpp"
#include "Screens/Screen.hpp"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"
#include "imgui.h"
#include <cstdlib>
#include <iostream>
#include <ostream>

struct sdl_data {
    SDL_Surface * winSurface;
    SDL_Window * window;
    SDL_Renderer* renderer;
};

Interface::Interface() {
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout << "Error: SDL_Init(): " << SDL_GetError() << std::endl;
        exit(1);
    }
    this->main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());

    IMGUI_CHECKVERSION();

    this->contexts_vec.push_back(this->createImGuiContext("Nes Emulator"));
    
    this->screens_vec.push_back(new MainScreen(this->sdl_data_vec[0]->renderer));
    
    this->contexts_vec.push_back(this->createImGuiContext("LMAO"));
    
    this->screens_vec.push_back(new MainScreen(this->sdl_data_vec[1]->renderer));
}

SDL_DATA * Interface::createSdlData(const char * windowName) {
    SDL_DATA * data = new SDL_DATA();
    
    data->window = SDL_CreateWindow(windowName, 
            (int)(1920 * this->main_scale), 
            (int)(1080 * this->main_scale), 
            WINDOW_FLAGS);
    
    if (data->window == nullptr) {
        std::cout << "Error: SDL_CreateWindow(): " << SDL_GetError() << std::endl;
        exit(1);
    }
    
    data->renderer = SDL_CreateRenderer(data->window, nullptr);
    
    SDL_SetRenderVSync(data->renderer, 1);

    if (data->renderer == nullptr) {
        std::cout << "Error: SDL_CreateRenderer(): " << SDL_GetError() << std::endl;
        exit(1);
    }   

    SDL_SetWindowPosition(data->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(data->window);

    return data;
}

ImGuiContext * Interface::createImGuiContext(const char* windowName) {
    ImGuiContext * context = ImGui::CreateContext();
    
    ImGui::SetCurrentContext(context);

    SDL_DATA * sdl_data = this->createSdlData(windowName);
    
    this->sdl_data_vec.push_back(sdl_data);
    
    ImGuiIO context_io = ImGui::GetIO();

    this->imguiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    
    ImGui::StyleColorsDark();
    
    ImGuiStyle& style = ImGui::GetStyle();
    
    style.ScaleAllSizes(this->main_scale);
    style.FontScaleDpi = this->main_scale;
    style.WindowPadding = ImVec2(0, 0);

    ImGui_ImplSDL3_InitForSDLRenderer(sdl_data->window, sdl_data->renderer);
    ImGui_ImplSDLRenderer3_Init(sdl_data->renderer);

    return context;
}

void Interface::render() {
    while (true) {
        if (this->pollScreenEvent() == -1) return;
        
        for (int i = 0; i < this->screens_vec.size(); i++) {
            this->showScreen(i);           
        }
    }
}

// TODO: create a enum for each possible state (idk if theres more than quit, but anyway is better)
int Interface::pollScreenEvent() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        for (int i = 0; i < this->contexts_vec.size(); i++) {
            ImGuiContext * context = this->contexts_vec[i];
            ImGui::SetCurrentContext(context);
            ImGui_ImplSDL3_ProcessEvent(&event);
        }
        
        for (int i = 0; i < this->contexts_vec.size(); i++) {
            ImGuiContext * context = this->contexts_vec[i];
            SDL_DATA * sdl_data = this->sdl_data_vec[i];
            if ( (event.type == SDL_EVENT_QUIT) 
                || (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED 
                    && event.window.windowID == SDL_GetWindowID(sdl_data->window)
                    ) 
            ) {
                return -1;
            }
        }
    }
    
    return 0;
}

void Interface::showScreen(int i) {
    ImGuiContext * context = this->contexts_vec[i];
    Screen * screen = this->screens_vec[i];
    SDL_DATA * sdl_data = this->sdl_data_vec[i];
    ImGui::SetCurrentContext(context);

    ImGuiIO imguiIO = ImGui::GetIO();

    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    
    screen->show();
    
    ImGui::Render();
    SDL_SetRenderScale(sdl_data->renderer, imguiIO.DisplayFramebufferScale.x, imguiIO.DisplayFramebufferScale.y);
    SDL_SetRenderDrawColorFloat(sdl_data->renderer,  CLEAR_COLOR.x, CLEAR_COLOR.y, CLEAR_COLOR.z, CLEAR_COLOR.w);
    SDL_RenderClear(sdl_data->renderer);
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), sdl_data->renderer);
    SDL_RenderPresent(sdl_data->renderer);   
}

Interface::~Interface() {
    for (ImGuiContext * context : this->contexts_vec) {
        ImGui::SetCurrentContext(context);

        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();

        ImGui::DestroyContext();
    }
    
    for (SDL_DATA * sdl_data : this->sdl_data_vec) {
        SDL_DestroyRenderer(sdl_data->renderer);
        SDL_DestroyWindow(sdl_data->window);
    }
    
    SDL_Quit();

    for (Screen * screen : this->screens_vec) {
        delete screen;
    }
}
