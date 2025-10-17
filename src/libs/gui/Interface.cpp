#include "Interface.hpp"
#include "SDL3/SDL_video.h"
#include "Screens/MainScreen/MainScreen.hpp"
#include "Screens/Screen.hpp"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"
#include "events.hpp"
#include "gui/Screens/DebuggerScreen/DebuggerScreen.hpp"
#include "imgui.h"
#include <cstdlib>
#include <iostream>
#include <optional>
#include <ostream>
#include <filesystem>

struct sdl_data {
    SDL_Surface * winSurface;
    SDL_Window * window;
    SDL_Renderer* renderer;
};

struct screen_data {
    SDL_DATA * sdl_data;
    ImGuiContext * context;
    Screen * screen;
};

Interface::Interface(std::queue<event_return>& event_pool) : event_pool(event_pool) {
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout << "Error: SDL_Init(): " << SDL_GetError() << std::endl;
        exit(1);
    }
    this->main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());

    IMGUI_CHECKVERSION();

    this->screen_data_array[MAIN_SCREEN_ENUM] = this->createScreenData<MainScreen>(
            "BABYLON - An NES emulator",
            this->debugFlag
        );
    
    this->screen_data_array[DEBUGGER_SCREEN_ENUM] = this->createScreenData<DebuggerScreen>(
            "Debugger" 
        );
}

// TODO: Remove duplicated code
// TODO: Really need to find a better way to pass class to function... Or find some way to restrict the template
template<class T>
SCREEN_DATA * Interface::createScreenData(const char * windowName, bool& boolFlag) {
    SCREEN_DATA * screen_data = new SCREEN_DATA();

    ImGuiContext * context = this->createImGuiContext();
    SDL_DATA * sdl_data = this->createSdlData(windowName);
    
    ImGui_ImplSDL3_InitForSDLRenderer(sdl_data->window, sdl_data->renderer);
    ImGui_ImplSDLRenderer3_Init(sdl_data->renderer);

    screen_data->sdl_data = sdl_data;
    screen_data->context = context;

    screen_data->screen = new T(sdl_data->renderer, this->event_pool, boolFlag);

    return screen_data;
}

// TODO: Really need to find a better way to pass class to function... Or find some way to restrict the template
template<class T>
SCREEN_DATA * Interface::createScreenData(const char * windowName) {
    SCREEN_DATA * screen_data = new SCREEN_DATA();

    ImGuiContext * context = this->createImGuiContext();
    SDL_DATA * sdl_data = this->createSdlData(windowName);
    
    ImGui_ImplSDL3_InitForSDLRenderer(sdl_data->window, sdl_data->renderer);
    ImGui_ImplSDLRenderer3_Init(sdl_data->renderer);

    screen_data->sdl_data = sdl_data;
    screen_data->context = context;

    screen_data->screen = new T(sdl_data->renderer, this->event_pool);

    return screen_data;
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

ImGuiContext * Interface::createImGuiContext() {
    ImGuiContext * context = ImGui::CreateContext();
    
    ImGui::SetCurrentContext(context);

    ImGuiIO context_io = ImGui::GetIO();

    context_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        
    if (!std::filesystem::exists(this->FONT_PATH)) {
       context_io.Fonts->AddFontDefault();
    } 
    else {
        if (!context_io.Fonts->AddFontFromFileTTF(this->FONT_PATH.c_str(), 12.0f)) {
            context_io.Fonts->AddFontDefault();
        }
    }

    ImGui::StyleColorsDark();
    
    ImGuiStyle& style = ImGui::GetStyle();
    
    style.ScaleAllSizes(this->main_scale);
    style.FontScaleDpi = this->main_scale;
    style.WindowPadding = ImVec2(0, 0);

    return context;
}

void Interface::render() {
    if (this->pollScreenEvent() == -1) { 
        this->event_pool.push(event_return{INTERFACE_CLOSE_EVENT, std::nullopt});

        return;
    }
    
    // TODO: Put the first conditional in a function   
    for (long unsigned int i = 0; i < this->screen_data_array.size(); i++) {
            if (i == DEBUGGER_SCREEN_ENUM) {
                if (!this->debugFlag) {
                    continue;
                }

                SDL_ShowWindow(this->screen_data_array[i]->sdl_data->window);
            }
            this->showScreen(i);           
    }

    return;
}

int Interface::pollScreenEvent() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        for (long unsigned int i = 0; i < this->screen_data_array.size(); i++) {
            if (i == DEBUGGER_SCREEN_ENUM && !this->debugFlag) continue;

            ImGuiContext * context = this->screen_data_array[i]->context;
            ImGui::SetCurrentContext(context);
            ImGui_ImplSDL3_ProcessEvent(&event);
        }
        
        for (long unsigned int i = 0; i < this->screen_data_array.size(); i++) {
            SDL_DATA * sdl_data = this->screen_data_array[i]->sdl_data;
            if ( (event.type == SDL_EVENT_QUIT) 
                || (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED 
                    && event.window.windowID == SDL_GetWindowID(sdl_data->window)
                ) 
            ) {
                switch (i) {
                    case MAIN_SCREEN_ENUM:
                        return -1;

                    case DEBUGGER_SCREEN_ENUM:
                        SDL_HideWindow(this->screen_data_array[i]->sdl_data->window);
                        this->debugFlag = false;
                        break;
                }
            }
        }
    }
    
    return 0;
}

void Interface::showScreen(int i) {
    ImGuiContext * context = this->screen_data_array[i]->context;
    Screen * screen = this->screen_data_array[i]->screen;
    SDL_DATA * sdl_data = this->screen_data_array[i]->sdl_data;
    ImGui::SetCurrentContext(context);

    ImGuiIO imguiIO = ImGui::GetIO();

    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    
    screen->show();
    
    ImGui::Render();
    SDL_SetRenderScale(sdl_data->renderer, imguiIO.DisplayFramebufferScale.x, imguiIO.DisplayFramebufferScale.y);
    SDL_SetRenderDrawColorFloat(sdl_data->renderer,  CLEAR_COLOR.x / 255, CLEAR_COLOR.y / 255, CLEAR_COLOR.z / 255, CLEAR_COLOR.w);
    SDL_RenderClear(sdl_data->renderer);
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), sdl_data->renderer);
    SDL_RenderPresent(sdl_data->renderer);   
}

// TODO: Use smart pointers insted of raw ones 
Interface::~Interface() {
    for (SCREEN_DATA * screen_data : this->screen_data_array) {
        ImGuiContext * context = screen_data->context;
        SDL_DATA * sdl_data = screen_data->sdl_data;
        Screen * screen = screen_data->screen;

        ImGui::SetCurrentContext(context);

        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();

        ImGui::DestroyContext();

        SDL_DestroyRenderer(sdl_data->renderer);
        SDL_DestroyWindow(sdl_data->window);

        delete screen;
    }
       
    SDL_Quit();
}

void Interface::setDebuggerScreenCpuInfo (
                const uint16_t& cpuPc,
                const uint8_t&  cpuSp,
                const uint8_t&  cpuAcc,
                const uint8_t&  cpuIdX,
                const uint8_t&  cpuIdY,
                const uint8_t&  cpuP,
                const std::vector<uint8_t>& memory
                ) {
    ((DebuggerScreen *)this->screen_data_array[DEBUGGER_SCREEN_ENUM]->screen)->setCpuDebuggerInfo(
        cpuPc,
        cpuSp,
        cpuAcc,
        cpuIdX,
        cpuIdY,
        cpuP,
        memory
    );
}
