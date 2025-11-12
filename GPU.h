#pragma once
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
uint8_t constexpr FLAG_REGISTER = 0xF6; // Flags: bool color, uint2_t bit depth, resolution
uint8_t constexpr RESERVED4 = 0xF7; //Reserved for future use
uint8_t constexpr RESERVED5 = 0xF8; //Reserved for future use


class GPU {
public:
	GPU(Memory& memory, Screen& screen) ///combined memory objsects and screen(s)
		: mem(memory), scr(screen) {
	}; //Constructor?
	void tick() { //GPU tick like CPU tick
		uint8_t command = mem.read(GPU_CMD);

		switch (command) {
			case 0x01: { //Draw Pixel Command
				pixelDraw();
			}
			case 0x00: //No Operation
			default: { 
				break;
			}
		}
	}
private:
	Memory& mem;
	Screen& scr;
	void processDisplayDevice() {
		// Read display object variables
		scr.
		// set flags
		mem.write(
		// color supported? (Save 'clock' cycles from reading two extra locations)

		// bit depth? (I dont know what use this does besides us posibly being able to use a proper data type. Which would work itself out anyway.)

		// resolution? (Prevemt wasted cycles sending data? maybe? Not really..)
	}
	void pixelDraw() {
		uint8_t pix_X = mem.read(X_POS);
		uint8_t pix_T = mem.read(Y_POS);
		uint8_t pix_C = mem.read(COLOR);
		scr.drawPixel(pix_X, pix_T, pix_C);
	}
	/// Could have a triple pixel render function using the other two color bytes simultaniously. This would make the resoultionflag useful? No it wouldnt. Screen does pixel fitting. 
}
}