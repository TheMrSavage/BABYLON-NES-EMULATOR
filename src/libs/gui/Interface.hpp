#ifndef INTERFACE_H
    #define INTERFACE_H
    #include <SDL3/SDL.h>
    #include "imgui.h"    

    class Interface {
        private:
            SDL_Surface* winSurface;
            SDL_Window* window;
            const ImVec4 CLEAR_COLOR = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
            const static SDL_WindowFlags WINDOW_FLAGS = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
            SDL_Renderer* renderer;
            float main_scale;
            bool debugFlag = false;
            ImGuiIO imguiIO;
            void setupSdl();
            void setupImGui();

        public:
            Interface();
            void render();
            ~Interface();
    };
#endif
