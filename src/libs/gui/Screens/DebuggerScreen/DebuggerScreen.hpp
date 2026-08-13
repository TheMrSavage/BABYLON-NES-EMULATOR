#ifndef DEBUGGER_SCREEN
    #define DEBUGGER_SCREEN
    
    #include "../Screen.hpp"
    #include "SDL3/SDL_render.h"
    #include <cstdint>
#include <memory>
    #include <queue>
    #include <string>
    #include <unordered_map>
    #include "../../../nes/rp2a03/cpu/instructions/InstructionsOpcodeEnum.hpp"
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
            
            std::unique_ptr<CpuDebuggerInfo> cpuDebuggerInfo;

            void openCPUDebugger();
            void openAPUDebugger();
            void openPPUDebugger();
            void showRoomMemory();
            const char * getCurrentInstructionString();
            void showStatusRegisterInfo();
            const static inline std::unordered_map<uint8_t, const char *> opcodesToInstructionsMap = {
                {ADC_IMMEDIATE, "ADC_IMMEDIATE"},
                {ADC_ZERO_PAGE, "ADC_ZERO_PAGE"},
                {ADC_ZERO_PAGE_X, "ADC_ZERO_PAGE_X"},
                {ADC_ABSOLUTE, "ADC_ABSOLUTE"},
                {ADC_ABSOLUTE_X, "ADC_ABSOLUTE_X"},
                {ADC_ABSOLUTE_Y, "ADC_ABSOLUTE_Y"},
                {ADC_INDIRECT_X, "ADC_INDIRECT_X"},
                {ADC_INDIRECT_Y, "ADC_INDIRECT_Y"},
                {AND_IMMEDIATE, "AND_IMMEDIATE"},
                {AND_ZERO_PAGE, "AND_ZERO_PAGE"},
                {AND_ZERO_PAGE_X, "AND_ZERO_PAGE_X"},
                {AND_ABSOLUTE, "AND_ABSOLUTE"},
                {AND_ABSOLUTE_X, "AND_ABSOLUTE_X"},
                {AND_ABSOLUTE_Y, "AND_ABSOLUTE_Y"},
                {AND_INDIRECT_X, "AND_INDIRECT_X"},
                {AND_INDIRECT_Y, "AND_INDIRECT_Y"},
                {ASL_ACCUMULATOR, "ASL_ACCUMULATOR"},
                {ASL_ZERO_PAGE, "ASL_ZERO_PAGE"},
                {ASL_ZERO_PAGE_X, "ASL_ZERO_PAGE_X"},
                {ASL_ABSOLUTE, "ASL_ABSOLUTE"},
                {ASL_ABSOLUTE_X, "ASL_ABSOLUTE_X"},
                {BCC_RELATIVE, "BCC_RELATIVE"},
                {BCS_RELATIVE, "BCS_RELATIVE"},
                {BEQ_RELATIVE, "BEQ_RELATIVE"},
                {BIT_ZERO_PAGE, "BIT_ZERO_PAGE"},
                {BIT_ABSOLUTE, "BIT_ABSOLUTE"},
                {BMI_RELATIVE, "BMI_RELATIVE"},
                {BNE_RELATIVE, "BNE_RELATIVE"},
                {BPL_RELATIVE, "BPL_RELATIVE"},
                {BRK_IMPLIED, "BRK_IMPLIED"},
                {BVC_RELATIVE, "BVC_RELATIVE"},
                {BVS_RELATIVE, "BVS_RELATIVE"},
                {CLC_IMPLIED, "CLC_IMPLIED"},
                {CLD_IMPLIED, "CLD_IMPLIED"},
                {CLI_IMPLIED, "CLI_IMPLIED"},
                {CLV_IMPLIED, "CLV_IMPLIED"},
                {CMP_IMMEDIATE, "CMP_IMMEDIATE"},
                {CMP_ZERO_PAGE, "CMP_ZERO_PAGE"},
                {CMP_ZERO_PAGE_X, "CMP_ZERO_PAGE_X"},
                {CMP_ABSOLUTE, "CMP_ABSOLUTE"},
                {CMP_ABSOLUTE_X, "CMP_ABSOLUTE_X"},
                {CMP_ABSOLUTE_Y, "CMP_ABSOLUTE_Y"},
                {CMP_INDIRECT_X, "CMP_INDIRECT_X"},
                {CMP_INDIRECT_Y, "CMP_INDIRECT_Y"},
                {CPX_IMMEDIATE, "CPX_IMMEDIATE"},
                {CPX_ZERO_PAGE, "CPX_ZERO_PAGE"},
                {CPX_ABSOLUTE, "CPX_ABSOLUTE"},
                {CPY_IMMEDIATE, "CPY_IMMEDIATE"},
                {CPY_ZERO_PAGE, "CPY_ZERO_PAGE"},
                {CPY_ABSOLUTE, "CPY_ABSOLUTE"},
                {DEC_ZERO_PAGE, "DEC_ZERO_PAGE"},
                {DEC_ZERO_PAGE_X, "DEC_ZERO_PAGE_X"},
                {DEC_ABSOLUTE, "DEC_ABSOLUTE"},
                {DEC_ABSOLUTE_X, "DEC_ABSOLUTE_X"},
                {DEX_IMPLIED, "DEX_IMPLIED"},
                {DEY_IMPLIED, "DEY_IMPLIED"},
                {EOR_IMMEDIATE, "EOR_IMMEDIATE"},
                {EOR_ZERO_PAGE, "EOR_ZERO_PAGE"},
                {EOR_ZERO_PAGE_X, "EOR_ZERO_PAGE_X"},
                {EOR_ABSOLUTE, "EOR_ABSOLUTE"},
                {EOR_ABSOLUTE_X, "EOR_ABSOLUTE_X"},
                {EOR_ABSOLUTE_Y, "EOR_ABSOLUTE_Y"},
                {EOR_INDIRECT_X, "EOR_INDIRECT_X"},
                {EOR_INDIRECT_Y, "EOR_INDIRECT_Y"},
                {INC_ZERO_PAGE, "INC_ZERO_PAGE"},
                {INC_ZERO_PAGE_X, "INC_ZERO_PAGE_X"},
                {INC_ABSOLUTE, "INC_ABSOLUTE"},
                {INC_ABSOLUTE_X, "INC_ABSOLUTE_X"},
                {INX_IMPLIED, "INX_IMPLIED"},
                {INY_IMPLIED, "INY_IMPLIED"},
                {JMP_ABSOLUTE, "JMP_ABSOLUTE"},
                {JMP_INDIRECT, "JMP_INDIRECT"},
                {JSR_ABSOLUTE, "JSR_ABSOLUTE"},
                {LDA_IMMEDIATE, "LDA_IMMEDIATE"},
                {LDA_ZERO_PAGE, "LDA_ZERO_PAGE"},
                {LDA_ZERO_PAGE_X, "LDA_ZERO_PAGE_X"},
                {LDA_ABSOLUTE, "LDA_ABSOLUTE"},
                {LDA_ABSOLUTE_X, "LDA_ABSOLUTE_X"},
                {LDA_ABSOLUTE_Y, "LDA_ABSOLUTE_Y"},
                {LDA_INDIRECT_X, "LDA_INDIRECT_X"},
                {LDA_INDIRECT_Y, "LDA_INDIRECT_Y"},
                {LDX_IMMEDIATE, "LDX_IMMEDIATE"},
                {LDX_ZERO_PAGE, "LDX_ZERO_PAGE"},
                {LDX_ZERO_PAGE_Y, "LDX_ZERO_PAGE_Y"},
                {LDX_ABSOLUTE, "LDX_ABSOLUTE"},
                {LDX_ABSOLUTE_Y, "LDX_ABSOLUTE_Y"},
                {LDY_IMMEDIATE, "LDY_IMMEDIATE"},
                {LDY_ZERO_PAGE, "LDY_ZERO_PAGE"},
                {LDY_ZERO_PAGE_X, "LDY_ZERO_PAGE_X"},
                {LDY_ABSOLUTE, "LDY_ABSOLUTE"},
                {LDY_ABSOLUTE_X, "LDY_ABSOLUTE_X"},
                {LSR_ACCUMULATOR, "LSR_ACCUMULATOR"},
                {LSR_ZERO_PAGE, "LSR_ZERO_PAGE"},
                {LSR_ZERO_PAGE_X, "LSR_ZERO_PAGE_X"},
                {LSR_ABSOLUTE, "LSR_ABSOLUTE"},
                {LSR_ABSOLUTE_X, "LSR_ABSOLUTE_X"},
                {NOP_IMPLIED, "NOP_IMPLIED"},
                {ORA_IMMEDIATE, "ORA_IMMEDIATE"},
                {ORA_ZERO_PAGE, "ORA_ZERO_PAGE"},
                {ORA_ZERO_PAGE_X, "ORA_ZERO_PAGE_X"},
                {ORA_ABSOLUTE, "ORA_ABSOLUTE"},
                {ORA_ABSOLUTE_X, "ORA_ABSOLUTE_X"},
                {ORA_ABSOLUTE_Y, "ORA_ABSOLUTE_Y"},
                {ORA_INDIRECT_X, "ORA_INDIRECT_X"},
                {ORA_INDIRECT_Y, "ORA_INDIRECT_Y"},
                {PHA_IMPLIED, "PHA_IMPLIED"},
                {PHP_IMPLIED, "PHP_IMPLIED"},
                {PLA_IMPLIED, "PLA_IMPLIED"},
                {PLP_IMPLIED, "PLP_IMPLIED"},
                {ROL_ACCUMULATOR, "ROL_ACCUMULATOR"},
                {ROL_ZERO_PAGE, "ROL_ZERO_PAGE"},
                {ROL_ZERO_PAGE_X, "ROL_ZERO_PAGE_X"},
                {ROL_ABSOLUTE, "ROL_ABSOLUTE"},
                {ROL_ABSOLUTE_X, "ROL_ABSOLUTE_X"},
                {ROR_ACCUMULATOR, "ROR_ACCUMULATOR"},
                {ROR_ZERO_PAGE, "ROR_ZERO_PAGE"},
                {ROR_ZERO_PAGE_X, "ROR_ZERO_PAGE_X"},
                {ROR_ABSOLUTE, "ROR_ABSOLUTE"},
                {ROR_ABSOLUTE_X, "ROR_ABSOLUTE_X"},
                {RTI_IMPLIED, "RTI_IMPLIED"},
                {RTS_IMPLIED, "RTS_IMPLIED"},
                {SBC_IMMEDIATE, "SBC_IMMEDIATE"},
                {SBC_ZERO_PAGE, "SBC_ZERO_PAGE"},
                {SBC_ZERO_PAGE_X, "SBC_ZERO_PAGE_X"},
                {SBC_ABSOLUTE, "SBC_ABSOLUTE"},
                {SBC_ABSOLUTE_X, "SBC_ABSOLUTE_X"},
                {SBC_ABSOLUTE_Y, "SBC_ABSOLUTE_Y"},
                {SBC_INDIRECT_X, "SBC_INDIRECT_X"},
                {SBC_INDIRECT_Y, "SBC_INDIRECT_Y"},
                {SEC_IMPLIED, "SEC_IMPLIED"},
                {SED_IMPLIED, "SED_IMPLIED"},
                {SEI_IMPLIED, "SEI_IMPLIED"},
                {STA_ZERO_PAGE, "STA_ZERO_PAGE"},
                {STA_ZERO_PAGE_X, "STA_ZERO_PAGE_X"},
                {STA_ABSOLUTE, "STA_ABSOLUTE"},
                {STA_ABSOLUTE_X, "STA_ABSOLUTE_X"},
                {STA_ABSOLUTE_Y, "STA_ABSOLUTE_Y"},
                {STA_INDIRECT_X, "STA_INDIRECT_X"},
                {STA_INDIRECT_Y, "STA_INDIRECT_Y"},
                {STX_ZERO_PAGE, "STX_ZERO_PAGE"},
                {STX_ZERO_PAGE_Y, "STX_ZERO_PAGE_Y"},
                {STX_ABSOLUTE, "STX_ABSOLUTE"},
                {STY_ZERO_PAGE, "STY_ZERO_PAGE"},
                {STY_ZERO_PAGE_X, "STY_ZERO_PAGE_X"},
                {STY_ABSOLUTE, "STY_ABSOLUTE"},
                {TAX_IMPLIED, "TAX_IMPLIED"},
                {TAY_IMPLIED, "TAY_IMPLIED"},
                {TSX_IMPLIED, "TSX_IMPLIED"},
                {TXA_IMPLIED, "TXA_IMPLIED"},
                {TXS_IMPLIED, "TXS_IMPLIED"},
                {TYA_IMPLIED, "TYA_IMPLIED"}
            };
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
