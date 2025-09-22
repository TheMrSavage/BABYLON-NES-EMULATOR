#include "SDL3/SDL_render.h"
#ifndef MAIN_SCREEN
    #include <SDL3/SDL.h>
    class MainScreen {
        private:
            SDL_Texture * GAME_DRAW_PIXEL_MATRIX;
            SDL_Renderer * INTERFACE_RENDERER;
            static const int GAME_DRAW_PIXEL_MATRIX_WIDTH = 256;
            static const int GAME_DRAW_PIXEL_MATRIX_HEIGHT = 240;
            
            void randomizeMatrix();
        public:
            MainScreen(SDL_Renderer * interface_renderer);
            ~MainScreen();
            void show();
    };
#endif
