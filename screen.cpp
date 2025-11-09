#include "screen.h"
#include <SFML/Graphics.hpp>
#include <sstream>
#include "stdexcept"

// Font loading
sf::Font globalFont;
bool fontLoaded = false;
bool loadGlobalFont(const std::string& path) {
    if (!fontLoaded) {
        if (!globalFont.loadFromFile(path)) {
            throw std::runtime_error("Failed to load font: " + path);
        }
        fontLoaded = true;
    }
    return fontLoaded;
}

static sf::RenderTexture Flipframe; // Off-screen render texture for saving individual frames

Screen::Screen(int x, int y, bool color, int depth, int screenMap):
    sizeX(x),
    sizeY(y),
    resolution(x * y),
    color(color),
    bitDepth(depth),
	screenMap(screenMap)
{
    int windowW = sizeX * screenMap + 8; // Preload vars with window sizes
    int windowH = sizeY * screenMap + 48; // since you cannot in the function call
    static sf::RenderWindow window(sf::VideoMode({windowW, windowH}), "Screen Window"); // Window create
    static sf::RenderTexture Flipframe; // Off-screen render texture

    // Create or recreate off-screen texture if needed
    if (Flipframe.getSize().x != windowW|| Flipframe.getSize().y != windowH) {
        if (!Flipframe.create(windowW, windowH)) {
            throw std::runtime_error("Failed to create render texture with window size");
        }
    }
}

void Screen::drawPixel(int x, int y, int value)
{
    ;
}

void Screen::drawScreen(sf::Vector2u map) {
    //clear screen, can do a selective clearing of an icremental counter if one is added.
    Flipframe.clear(sf::Color(20, 20, 20)); // curently clearing whole screen but if it takes a few cycles to issue a draw pixel command I think it shouldn't 

    //Title text
    loadGlobalFont("InputMonoNarrow-Light.ttf");

    std::stringstream ss;
    ss << std::uppercase << std::setw(2) << std::setfill('0') << resolution << " (" << sizeX << "x" << sizeY << ":";
    sf::Text txt(ss.str(), globalFont, 30);
    txt.setFillColor(sf::Color::White);
    txt.Bold; // ???
    txt.setPosition({ 10,5 });
    Flipframe.draw(txt);

    // Screen box
    sf::RectangleShape panelBg;
    panelBg.setPosition({ 0, 40 });
    panelBg.setSize({ (sizeX * screenMap) + 2, (sizeY * screenMap) + 2 });
    panelBg.setFillColor(sf::Color(30, 30, 30));
    panelBg.setOutlineColor(sf::Color(100, 100, 100));
    panelBg.setOutlineThickness(2);

    //Simulated Computer Pixels
    int scalefac = 256 / (1 << bitDepth);
    int scalefac = 256 / (1 << bitDepth);
    int startX = 6; //5 + 1;
    int startY = 43; //42 + 1;

    for (int i = 0; i < resolution; i++) {
        sf::RectangleShape cell({ screenMap,screenMap });
        cell.setPosition(startX + i % sizeX * screenMap, startY + i / sizeX * screenMap);

        // Pre procesing (does ask meet abilities of "simulated display")
        // Color capabilitiy
        // Bit depth
            //bitDepth / 256 can map any 8 bit to a lower bit depth.
            // otherwise can use bitdepth to multiply up to 8 bit.

        if (!color) {
            int tc = value * scalefac; //temp color
            cell.setFillColor(sf::Color(tc, tc, tc));
        }
        else {
            //split value upinto RGB components
            int R = value1 * scalefac;
            int G = value2 * scalefac;
            int B = value3 * scalefac;
            int tc = 256 / (1 << bitDepth); //temp color
            cell.setFillColor(sf::Color(R, G, B));
        }
        Flipframe.draw(cell);
}