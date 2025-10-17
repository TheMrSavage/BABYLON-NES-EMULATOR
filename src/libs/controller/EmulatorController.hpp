#ifndef EMULATOR_CONTROLLER
    #define EMULATOR_CONTROLLER
    #include <nes/Nes.hpp>
    #include <gui/Interface.hpp>
    #include <events.hpp>    

    class EmulatorController {
        private:
            Interface * interface = nullptr;
            Nes * nes = nullptr;

            std::queue<event_return> interface_event_pool;
            void handleRoom(const std::vector<unsigned char>& room);
            static bool isValidRoom(const std::vector<unsigned char>& room);
            void mockCPU(const std::vector<unsigned char>& room);
        public:
            void start();
            EmulatorController();
            ~EmulatorController();
    };
#endif
