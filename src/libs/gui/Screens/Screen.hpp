#ifndef SCREEN
    #define SCREEN
    #include <events.hpp>
    #include <queue>
    #include "SDL3/SDL_render.h"
    class Screen {
        private:
        protected:
            SDL_Renderer * INTERFACE_RENDERER;
            std::queue<event_return>& event_pool;
            void pushQueueElement(event_return event);
        public:
            Screen(SDL_Renderer * interface_renderer,
                    std::queue<event_return>& event_pool) : 
                INTERFACE_RENDERER(interface_renderer),
                event_pool(event_pool){};
            event_return popQueueElement();
            virtual void show(){};        
            virtual ~Screen(){};
            
    };
#endif
