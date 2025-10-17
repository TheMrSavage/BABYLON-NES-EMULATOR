#ifndef INTERFACE_H
    #define INTERFACE_H
    #include <SDL3/SDL.h>
    #include <array>
    #include <queue>
    #include <string>
    #include <events.hpp>
    #include "imgui.h"

    typedef struct sdl_data SDL_DATA;
    typedef struct screen_data SCREEN_DATA;
    class Interface {
        private:
            const ImVec4 CLEAR_COLOR = ImVec4(112.0f, 128.0f, 144.0f, 1.00f);
            const static SDL_WindowFlags WINDOW_FLAGS = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
            float main_scale;
            bool debugFlag = false;
            ImGuiIO imguiIO;
            const inline static std::string FONT_PATH = "resources/fonts/PixelatedElegance/PixelatedEleganceRegular-ovyAA.ttf";

            // TODO: Make all vecs fixed size and create an enum for each screen 
            enum SCREENS_ENUM {
                MAIN_SCREEN_ENUM,
                DEBUGGER_SCREEN_ENUM
            };

            std::array<SCREEN_DATA *, 2> screen_data_array;

            SDL_DATA * createSdlData(const char * windowName);    
            ImGuiContext * createImGuiContext();
            
            template<class T>
            SCREEN_DATA * createScreenData(
                    const char * windowName, 
                    bool& boolFlag
                );
            
            template<class T>
            SCREEN_DATA * createScreenData(
                    const char * windowName 
            );

            int pollScreenEvent();
            void showScreen(int i);
            std::queue<event_return>& event_pool;

        public:
            Interface(std::queue<event_return>& event_pool);
            void render();
            void setDebuggerScreenCpuInfo(
                const uint16_t& cpuPc,
                const uint8_t&  cpuSp,
                const uint8_t&  cpuAcc,
                const uint8_t&  cpuIdX,
                const uint8_t&  cpuIdY,
                const uint8_t&  cpuP
            );
            ~Interface();
    };
#endif
