#include <bitset>
#include <cstdint>
#include <iostream>
#include <nes/rp2a03/cpu/Cpu_6502.hpp>
#include <nes/rp2a03/cpu/instructions/InstructionsOpcodeEnum.hpp>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include <nlohmann/json_fwd.hpp>
#include <ostream>
#include <stdexcept>
#include <vector>

bool execute6502Test(const nlohmann::json& test) {
    try {
         std::vector<uint8_t> memory(0x10000);
         CPU cpu(memory);
         
         const std::string& testName = test["name"];

         std::cout << "[+] Executing test: " << testName << std::endl;
            
         uint16_t mockedPc   = test["initial"]["pc"];
         uint8_t mockedSp    = test["initial"]["s"];
         uint8_t mockedAcc   = test["initial"]["a"];
         uint8_t mockedIdX   = test["initial"]["x"];
         uint8_t mockedIdY   = test["initial"]["y"];
         uint8_t mockedPFlag = test["initial"]["p"];
         
         uint16_t finalPc   = test["final"]["pc"];
         uint8_t finalSp    = test["final"]["s"];
         uint8_t finalAcc   = test["final"]["a"];
         uint8_t finalIdX   = test["final"]["x"];
         uint8_t finalIdY   = test["final"]["y"];
         uint8_t finalPFlag = test["final"]["p"];

         std::cout << "[+] Initial Pc: " << (int)mockedPc << std::endl;
         std::cout << "[+] Initial Sp: " << (int)mockedSp << std::endl;
         std::cout << "[+] Initial Acc: " << (int)mockedAcc << std::endl;
         std::cout << "[+] Initial IdX: " << (int)mockedIdX << std::endl;
         std::cout << "[+] Initial IdY: " << (int)mockedIdY << std::endl;
         std::cout << "[+] Initial PFlag: " << (int)mockedPFlag << std::endl;

         cpu.setPc(mockedPc);
         cpu.setSp(mockedSp);
         cpu.setAcc(mockedAcc);
         cpu.setIdX(mockedIdX);
         cpu.setIdY(mockedIdY);
         cpu.setP(mockedPFlag);
         
         for (const auto& memoryValues : test["initial"]["ram"]) {
             std::cout << "[+] Writting to memory at " << memoryValues[0] << " value: 0x" << std::hex << (int)memoryValues[1] << std::endl;

             memory[memoryValues[0]] = memoryValues[1];
         }
         
         // TODO: Add a more properly way to debug CPU cycles 
         // IT's interesting that the test suit itself already have a way to test BUS instructions. That's pretty cool
         uint8_t cycles = cpu.executeNextInstruction();

         const uint16_t& cpuPc   = cpu.returnPc();
         const uint8_t& cpuSp    = cpu.returnSp();
         const uint8_t& cpuAcc   = cpu.returnAcc();
         const uint8_t& cpuIdX   = cpu.returnIdX();
         const uint8_t& cpuIdY   = cpu.returnIdY();
         const uint8_t& cpuPFlag = cpu.returnP();

         if (cpuPc != finalPc) {
             std::cout << "[-] Cpu Pc != Final Pc: (" << (int)cpuPc << ", " << (int)finalPc << ")" << std::endl;
             std::cout << "[-] Failed test: " << testName << std::endl;
             return false;
         }
         
         if (cpuSp != finalSp) {
             std::cout << "[-] Cpu Sp != Final Sp: (" << (int)cpuSp << ", " << (int)finalSp << ")" << std::endl;
             std::cout << "[-] Failed test: " << testName << std::endl;
             return false;
         }

         if (cpuAcc != finalAcc) {
             std::cout << "[-] Cpu Acc != Final Acc: (" << (int)cpuAcc << ", " << (int)finalAcc << ")" << std::endl;
             std::cout << "[-] Failed test: " << testName << std::endl;
             return false;
         }
         
         if (cpuIdX != finalIdX) {
             std::cout << "[-] Cpu IdX != Final IdX: (" << (int)cpuIdX << ", " << (int)finalIdX << ")" << std::endl;
             std::cout << "[-] Failed test: " << testName << std::endl;
             return false;
         }
         
         if (cpuIdY != finalIdY) {
             std::cout << "[-] Cpu IdY != Final IdY: (" << (int)cpuIdY << ", " << (int)finalIdY << ")" << std::endl;
             std::cout << "[-] Failed test: " << testName << std::endl;
             return false;
         }
         
         if (cpuPFlag != finalPFlag) {
             std::cout << "[-] Cpu PFlag != Final PFlag: (" << std::bitset<8>((int)cpuPFlag) << ", " << std::bitset<8>((int)finalPFlag) << ")" << std::endl;
             std::cout << "[-] Failed test: " << testName << std::endl;
             return false;
         }
    }
    // TODO: Create a specific error for invalid opcode in CPU class
    catch (std::runtime_error error) {
        std::cout << "[-] Invalid opcode!" << std::endl;
    }
    
    std::cout << "[+] Test passed!" << std::endl; 
    return true;
}

bool execute6502Tests(const nlohmann::json& tests) {
    for (const auto& test : tests) {
        if (!execute6502Test(test)) return false;
    }

    return true;
}

void CPU6502Test(const std::string& testsPath) {
    for (const auto& entry : std::filesystem::directory_iterator(testsPath)) {
        const std::string& filePath = entry.path();

        std::cout << "[+] Current test file: " << filePath << std::endl;

        std::fstream testFile(entry.path());
        
        nlohmann::json testsJson = nlohmann::json::parse(testFile);

        bool result = execute6502Tests(testsJson);

        if (!result) {
            std::cout << "[-] Failed at: " << filePath << std::endl;
            return;
        }
        else {
            std::cout << "[+] Test " << filePath << " passed!" << std::endl;
        }
        
        std::cout << std::endl;
    }
}

void Cpu6502Tests() {
    CPU6502Test("testcase/6502/nes");
}

int main() {
    Cpu6502Tests();
}
