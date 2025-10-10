#include "DebuggerScreen.hpp"

DebuggerScreen::DebuggerScreen(SDL_Renderer * interface_renderer,
                    std::queue<event_return>& event_pool) : Screen(interface_renderer, event_pool) {

}

void DebuggerScreen::show(){}
