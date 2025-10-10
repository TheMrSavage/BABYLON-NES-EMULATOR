#ifndef DEBUGGER_SCREEN
    #define DEBUGGER_SCREEN
    
    #include "../Screen.hpp"
    #include "SDL3/SDL_render.h"
    #include <queue>
class DebuggerScreen : public Screen { 
        private: 
            SDL_Renderer * INTERFACE_RENDERER;
        public:
            DebuggerScreen(SDL_Renderer * interface_renderer,
                    std::queue<event_return>& event_pool);
            ~DebuggerScreen(){};
            void show() override;
    };
#endif
