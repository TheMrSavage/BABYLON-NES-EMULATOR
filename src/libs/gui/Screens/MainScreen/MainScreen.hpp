#include "events.hpp"
#include <queue>
#include <vector>
#ifndef MAIN_SCREEN
    #include <SDL3/SDL.h>
    #include "../Screen.hpp"
    class MainScreen : public Screen {
        private:
            SDL_Texture * GAME_DRAW_PIXEL_MATRIX;
            static const int GAME_DRAW_PIXEL_MATRIX_WIDTH = 256;
            static const int GAME_DRAW_PIXEL_MATRIX_HEIGHT = 240;
            bool showMenu = true;
            static std::vector<unsigned char> openFileFunction();
            void randomizeMatrix();
        public:
            MainScreen(SDL_Renderer * interface_renderer, std::queue<event_return>& event_pool);
            ~MainScreen();
            void show() override;
    };
#endif
