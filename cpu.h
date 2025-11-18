#pragma once //prevents multiple inclusions
#include <cstdint> //bit types
#include <vector> //memory array
#include <iostream>
#include "instructions.h"
#include "memory.h"
#include "gpu.h"

class CPU {
    public:
        CPU(std::vector<Memory*>& mems, GPU* gpu); //reference to existing memory class and vector array of memory objects
        void run();
        void step();
        void execute(uint8_t opcode); //executes a single instruction.
        bool isHalted() const { return halted; }
        uint16_t getPC() const { return PC; }
        uint8_t getACC() const { return ACC; }

    private:
        std::vector<Memory*> memories; //all memory objects
        GPU* gpuDevice; // <-- Add this field to store GPU pointer
        uint8_t ACC; //accumulator
        uint16_t PC; //Program Counter
        bool halted; // Halt from HLT instruction
        Memory* currentMemory(); //uses PC
        Memory* currentMemory(uint16_t address); //uses given Address
};
