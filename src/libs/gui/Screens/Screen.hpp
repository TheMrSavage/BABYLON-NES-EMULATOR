#ifndef SCREEN
    #define SCREEN
    #include "SDL3/SDL_render.h"
    class Screen {
        private:
        protected:
            SDL_Renderer * INTERFACE_RENDERER;
        public:
            Screen(SDL_Renderer * interface_renderer){this->INTERFACE_RENDERER = interface_renderer;};
            virtual void show(){};
            virtual ~Screen(){};
    };
#endif
