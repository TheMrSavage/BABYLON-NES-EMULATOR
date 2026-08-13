#ifndef EMULATOR_CONTROLLER
    #define EMULATOR_CONTROLLER
    #include <memory>
#include <nes/Nes.hpp>
    #include <gui/Interface.hpp>
    #include <events.hpp>    

    class EmulatorController {
        private:
            std::unique_ptr<Interface> interface;
            std::unique_ptr<Nes> nes;

            std::queue<event_return> interface_event_pool;
            void handleRoom(const std::vector<unsigned char>& room);
            static bool isValidRoom(const std::vector<unsigned char>& room);
            void mockCPU(const std::vector<unsigned char>& room);

            bool stepByStepDebugger = false;
            bool execNextStep = false;
        public:
            void start();
            EmulatorController();
            ~EmulatorController();
    };
#endif
