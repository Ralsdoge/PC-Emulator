#include "cpu.h"
#include <iostream>
#include "gpu.h"

CPU::CPU(std::vector<Memory*>& mems, GPU* gpu = nullptr): //constructor 
memories(mems), 
gpuDevice(gpu), 
ACC(0),
PC(0),
halted(false)
{}

Memory* CPU::currentMemory() {
    return currentMemory(PC);  // instruction fetch
}

Memory* CPU::currentMemory(uint16_t address) {
    for (auto mem : memories) { // assuming all memory modules start at 0
        if (PC >= mem->base && PC < mem->base + mem->size) {
            return mem; // Oei
        }
    }
    return nullptr; //Program Counter is out of range
}

void CPU::run() {
    while (!halted) {
        step(); //run a step at a time until HLT
    }
}

void CPU::step() {
    if (halted) {
        return;
    }
    Memory* mem = currentMemory();
    if (!mem) { //if out of range alert.
        std::cout << "Error: PC " << PC << " is out of memory range!\n";
        halted = true;
        return;
    }
    uint8_t opcode = mem->read(PC - mem->base); //fetch
    PC++;
    execute(opcode);
	if (gpuDevice) {
            gpuDevice->tick();
	}
    //pause for step by step here

}

void CPU::execute(uint8_t opcode) {
    Memory* mem = currentMemory();
    
    switch (opcode) {
        case LDA: {
            uint8_t addr = mem->read(PC - mem->base);
            PC++;
            Memory* targetMem = currentMemory(addr);
            if (!targetMem) { 
                halted = true; 
                return; }
            ACC = mem->read(addr);
            break;
        }
        case STA: {
            uint8_t addr = mem->read(PC - mem->base);
            PC++;
            Memory* targetMem = currentMemory(addr);
            if (!targetMem) { 
                halted = true; 
                return; }
            mem->write(addr,ACC);
            break;
        }
        case ADD: {
            uint8_t addr = mem->read(PC - mem->base);
            PC++;
            Memory* targetMem = currentMemory(addr);
            if (!targetMem) { 
                halted = true; 
                return; }
            ACC += mem->read(addr);
            break;
        }
        case JMP: {
            uint8_t addr = mem->read(PC - mem->base);
            PC = addr;
            break;
        }
        case HLT: {
            halted = true;
            break;
        }
        default: {
            std::cout << "Uknown opcode: " << (int)opcode << " at " << PC << "\n";
            halted = true;
            break;
        }
    }
}