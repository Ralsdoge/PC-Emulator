#include "screen.h"
#include <SFML/Graphics.hpp>
#include <sstream>
#include "stdexcept"

//Reused Variables
int startX = 6; //5 + 1;
int startY = 43; //42 + 1;


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
    static sf::RenderWindow window(sf::VideoMode({windowW, windowH}), "Screen Window"); // Window create
    static sf::RenderTexture Flipframe; // Off-screen render texture

    // Create or recreate off-screen texture if needed
    if (Flipframe.getSize().x != windowW|| Flipframe.getSize().y != windowH) {
        if (!Flipframe.create(windowW, windowH)) {
            throw std::runtime_error("Failed to create render texture with window size");
        }
    }
}
void Screen::drawPixel(int x, int y, int value, bool directin) {
	int pixelX;
	int pixelY;
	if (!directin) { // if not already calculated, calculate pixel pos.
		int pixelX = startX + (x * screenMap);
		int pixelY = startY + (y * screenMap);
	}
	else { // if pre-calculated, passthrough values
		int pixelX = x;
		int pixelY = y;
	}
	// Simulated Computer Pixels
	// Size on screen
	sf::RectangleShape cell({ static_cast<float>(screenMap),static_cast<float>(screenMap) }); //needs to be floats or SFML yells at you
	// Position
	cell.setPosition(pixelX, pixelY);
	// Pre procesing (does ask meet abilities of "simulated display")
	// Bit depth
	//bitDepth / 256 can map any 8 bit to a lower bit depth.
	// otherwise can use bitdepth to multiply up to 8 bit.
	int scalefac = 256 / (1 << bitDepth);

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
		float RectW = (sizeX * screenMap) + 2; //Neccessary, unfortunately
		float RectH = (sizeY * screenMap) + 2;
		panelBg.setSize({RectW,RectH});
		panelBg.setFillColor(sf::Color(30, 30, 30));
		panelBg.setOutlineColor(sf::Color(100, 100, 100));
		panelBg.setOutlineThickness(2);

		for (int i = 0; i < resolution; i++) {
			//sf::Vector2u map //Break vector into individual pixel data.
			int value = map.x + (map.y << 8); // temp value from map vector
			Screen::drawPixel(startX + i % sizeX * screenMap, startY + i / sizeX * screenMap, value);
		}
	}