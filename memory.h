#pragma once //prevents multiple inclusions
#include <cstdint> //bit types
#include <vector> //for array of data
//class
class Memory {
    public:
        Memory(uint16_t baseAddress, size_t size);
        uint8_t read(uint16_t address) const;
        void write(uint16_t address, uint8_t value);
        uint16_t base; //starting address
        size_t size; //in bytes
        std::vector<uint8_t> ILLEGAL_dumpMem() const;
    private: 
        std::vector<uint8_t> data;
};
//table
//memory
//access
//write
