#include "memory.h" //include header
#include <stdexcept>
#include <vector>

Memory::Memory(uint16_t baseAddress, size_t sz) : //constructor
base(baseAddress), //starting address
size(sz), //size
data(sz, 0) // initialize all memory cells to zero
{} 

uint8_t Memory::read(uint16_t address) const {
    if (address >= data.size()) {
        throw std::out_of_range("Memory read out of range.");
    }
   return data[address];
}

void Memory::write(uint16_t address, uint8_t value) {
    if (address >= data.size()) {
        throw std::out_of_range("Memory read out of range.");
    }
    data[address] = value;
} 

std::vector<uint8_t> Memory::ILLEGAL_dumpMem() const { //Get data from linked object
    std::vector<uint8_t> data;
    data.reserve(size); // reserve space for efficiency
    for (uint16_t addr = 0; addr < size; ++addr) {
        data.push_back(read(addr)); // read each byte
    }
    return data; //Return
}
//Memory ram1(1024);
