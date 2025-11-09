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

class Screen {
public:
    Screen(int x, int y, bool color, int depth, int screenMap = 1);
    void drawPixel(int x, int y, int value);
    void drawScreen(sf::Vector2u map);
private:
    const int sizeX;
    const int sizeY;
    const int resolution;
    const int color;
    const int bitDepth;
	const int screenMap;
};