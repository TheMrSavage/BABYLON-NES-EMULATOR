#ifndef DEBUGGER_SCREEN
    #define DEBUGGER_SCREEN
    
    #include "../Screen.hpp"
    #include "SDL3/SDL_render.h"
class DebuggerScreen : Screen { 
        private: 
            SDL_Renderer * INTERFACE_RENDERER;
        public:
            DebuggerScreen(SDL_Renderer * interface_renderer) : Screen(interface_renderer){};
            ~DebuggerScreen();
            void show() override;
    };
#endif
