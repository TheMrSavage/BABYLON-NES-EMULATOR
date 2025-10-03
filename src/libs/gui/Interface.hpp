#ifndef INTERFACE_H
    #define INTERFACE_H
    #include <SDL3/SDL.h>
    #include <vector>
    #include "Screens/Screen.hpp"
    #include "imgui.h"

    typedef struct sdl_data SDL_DATA;
    typedef struct screen_data SCREEN_DATA;
    class Interface {
        private:
            const ImVec4 CLEAR_COLOR = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
            const static SDL_WindowFlags WINDOW_FLAGS = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
            float main_scale;
            bool debugFlag = false;
            ImGuiIO imguiIO;

            // TODO: Make all vecs fixed size and create an enum for each screen 
            std::vector<SCREEN_DATA *> screen_data_vec;
            // std::vector<SDL_DATA *> sdl_data_vec;
            // std::vector<ImGuiContext *> contexts_vec;
            // std::vector<Screen *> screens_vec;

            SDL_DATA * createSdlData(const char * windowName);    
            ImGuiContext * createImGuiContext();
            template<typename T>
            SCREEN_DATA * createScreenData(const char * windowName);
            int pollScreenEvent();
            void showScreen(int i);
        public:
            Interface();
            void render();
            ~Interface();
    };
#endif
