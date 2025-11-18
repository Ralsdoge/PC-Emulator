#pragma once
#include <SFML/Graphics.hpp> // Drawing to window and Textures
#include "memory.h" // Memory access
#include "screen.h" // Screen access
#include <cstdint> // bit types
#include <iostream> // console output

//Memory Map
// Today I learned about "constexpr". Ok..
uint8_t constexpr GPU_CMD = 0xF0; // GPU Command Register
uint8_t constexpr X_POS = 0xF1; // X Position Register
uint8_t constexpr Y_POS = 0xF2; // Y Position Register
uint8_t constexpr COLOR_R = 0xF3; // Red/Greyscale Color Register
uint8_t constexpr COLOR_G = 0xF4; // Green Color Register
uint8_t constexpr COLOR_B = 0xF5; // liue Color Register
uint32_t constexpr FLAG_REGISTER = 0xF6; // Flags: bool color, uint2_t bit depth, resolution
//0xFA Next free
/*
uint8_t constexpr FLAG_REGISTER = 0xF6; // Flags: bool color, uint2_t bit depth, resolution
uint8_t constexpr X_DIMENTIONS1 = 0xF7; //X Dem 1
uint8_t constexpr X_DIMENTIONS2 = 0xF8; //X Dem 2 //Probably can delete and change the fist part to uint16_t
uint8_t constexpr Y_DIMENTIONS1 = 0xF9; //Y Dem 1
uint8_t constexpr Y_DIMENTIONS2 = 0xFA; //Y Dem 2
*/

class GPU {
public:
	GPU(std::vector<Memory*>& memories, Screen& screen);
	void tick(); // GPU tick like CPU tick
	void processDisplayDevice(); // Read and write screen hardware info to memory
private:
	std::vector<Memory*>& mems; // Store reference to memory vector
	Screen& scr; //Screen object reference
	void pixelDraw(); // Draw a pixel at given position with given color
	void drawScreen(); // Draw the entire screen from memory bitmap
	uint8_t read(uint16_t addr);
	void write(uint16_t addr, uint8_t value);
};
