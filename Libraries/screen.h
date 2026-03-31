#pragma once
#include <cstdint> // bit types
#include <vector> //for array of data
#include <string>
#include "filesystem"
#include <SFML/Graphics.hpp>
//#include <iostream>

// Font loading
extern sf::Font globalFont;
extern bool font_loaded;

bool loadGlobalFont(const std::string& path);

// 24 bit unsigned integer struct
	// for carrying RGB uint8_t values from GPU draw screen input array to pixel draw function 
	// YOU LEFT OFF HERE
struct uint24_t {
	uint8_t bytes[3];

	uint32_t toInt() const {
		return (bytes[0]) | (bytes[1] << 8) | (bytes[2] << 16);
	}
	void fromInt(uint32_t v) { //Uses bitshifting to store in a 32 bit unsigned integer
		bytes[0] = v & 0xFF;
		bytes[1] = (v >> 8) & 0xFF;
		bytes[2] = (v >> 16) & 0xFF;
	}
};

class Screen {
public:
	Screen(int x, int y, bool color, int depth, int screenMap = 1, int Grey3Channel = 0);
    void Render(sf::RenderWindow& window);
	void drawPixel(int x, int y, uint8_t value[3], bool directin = false, bool GreyChannelmode = false);
    void drawScreen(std::vector<uint24_t> bitmap);
	uint32_t FHardwareInfoRequest();
	bool loadCSVImage(const std::string& absolutePath);
	sf::RenderWindow& getWindowILLEGAL() { return window; }
private:
    const int sizeX;
    const int sizeY;
    const int resolution;
    const int color;
    const int bitDepth;
	const int screenMap;
	const int Grey3Channel;

	sf::RenderWindow window; // unique per object
	sf::RenderTexture Flipframe; // unique per object
	};