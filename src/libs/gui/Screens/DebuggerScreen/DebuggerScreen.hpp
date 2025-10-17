#ifndef DEBUGGER_SCREEN
    #define DEBUGGER_SCREEN
    
    #include "../Screen.hpp"
    #include "SDL3/SDL_render.h"
    #include <queue>
    class DebuggerScreen : public Screen { 
        private: 
            SDL_Renderer * INTERFACE_RENDERER;

            typedef struct CpuDebuggerInfo {    
              const uint16_t& cpuPc;
              const uint8_t&  cpuSp;
              const uint8_t&  cpuAcc;
              const uint8_t&  cpuIdX;
              const uint8_t&  cpuIdY;
              const uint8_t&  cpuP;
              const std::vector<uint8_t>& cpuMemory;
            }CpuDebuggerInfo;
            
            CpuDebuggerInfo * cpuDebuggerInfo = nullptr;

            void openCPUDebugger();
            void openAPUDebugger();
            void openPPUDebugger();
            void showRoomMemory();
        
        public:
            DebuggerScreen(SDL_Renderer * interface_renderer,
                    std::queue<event_return>& event_pool);
            ~DebuggerScreen(){};
            void show() override;
            void setCpuDebuggerInfo(   
               const uint16_t& cpuPc,
               const uint8_t&  cpuSp,
               const uint8_t&  cpuAcc,
               const uint8_t&  cpuIdX,
               const uint8_t&  cpuIdY,
               const uint8_t&  cpuP,
               const std::vector<uint8_t>& cpuMemory
            );
    };
#endif
