#include "screen.h"
#include <SFML/Graphics.hpp>
#include <sstream>
#include <tuple> //for carring multiple bytes of color data between methods
#include "stdexcept"
#include <cstdint> // bit types
#include <fstream> // for file reading (testing)
#include <iostream> // for error output

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

inline uint8_t quantize8(uint8_t v, int bitDepth) {// bitdepth adjuster needs to be looked at and understood
	if (bitDepth >= 8) return v;
	if (bitDepth <= 0) return 0;

	int levels = 1 << bitDepth;          // 8 levels for bitDepth=3
	int step = 255 / (levels - 1);       // 36 for 3-bit

	int q = v / step;                    // integer divide
	int out = q * step;                  // expand back
	return std::clamp(out, 0, 255);
}

Screen::Screen(int x, int y, bool color, int depth, int screenMap, int Grey3Channel):
    sizeX(x),
    sizeY(y),
    resolution(x * y),
    color(color),
    bitDepth(depth),
	screenMap(screenMap),
	Grey3Channel(Grey3Channel),
	window(sf::VideoMode(x * screenMap + 8, y * screenMap + 48), "Screen Window") // Wow, it's the object unique window

{
	unsigned int windowW = Screen::sizeX * Screen::screenMap + 8; // Preload vars with window sizes
	unsigned int windowH = Screen::sizeY * Screen::screenMap + 48; // since you cannot in the function call
    //static sf::RenderWindow window(sf::VideoMode({windowW, windowH}), "Screen Window"); // Window create
    static int frameCounter = 0;
	//static sf::RenderTexture Flipframe; // Off-screen render texture

    // Create or recreate off-screen texture if needed
    if (!Flipframe.create(windowW, windowH)) {
        throw std::runtime_error("Failed to create render texture with window size");
    }
};

void Screen::Render(sf::RenderWindow& window) {
	//sf::Image img;
	//img.create(w,h,sf::Color::Black);
	
	//Display flipframe so its ready for drawing to window
	Flipframe.display();


	//Title text
    loadGlobalFont("InputMonoNarrow-Light.ttf");

    std::stringstream ss;
    ss << std::uppercase << std::setw(2) << std::setfill('0') << resolution << " (" << sizeX << "x" << sizeY << "):";
    sf::Text txt(ss.str(), globalFont, 30);
    txt.setFillColor(sf::Color::White);
    txt.setPosition({ 10,5 });
    window.draw(txt);

    // Screen box
    sf::RectangleShape panelBg;
    panelBg.setPosition({ 0, 40 });
	float RectW = (Screen::sizeX * screenMap) + 2; //Neccessary, unfortunately
    float RectH = (Screen::sizeY * screenMap) + 2;
    panelBg.setSize({RectW,RectH});
    panelBg.setFillColor(sf::Color(30, 30, 30));
    panelBg.setOutlineColor(sf::Color(100, 100, 100));
    panelBg.setOutlineThickness(2);
	window.draw(panelBg);
    // Draw to on-screen window 
    sf::Sprite spr(Flipframe.getTexture());
    //window.clear();
	
    window.draw(spr);
    window.display();

	Screen::Flipframe.display();
}
void Screen::drawPixel(int x, int y, uint8_t value[3], bool directin, bool Grey3Channel) {
    int pixelX;
    int pixelY;
    if (!directin) { // if not already calculated, calculate pixel pos.
        pixelX = startX + (x * screenMap);
        pixelY = startY + (y * screenMap);
    }
	else { // if pre-calculated, passthrough values
        pixelX = x;
        pixelY = y;
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

    // Color capabilitiy
    if (!color) {
        uint8_t R = quantize8(value[0],bitDepth);
        cell.setFillColor(sf::Color(R, R, R));
    }
    else {
        //split value upinto RGB components
        uint8_t R = quantize8(value[0],bitDepth);
        uint8_t G = quantize8(value[1],bitDepth);
        uint8_t B = quantize8(value[2],bitDepth);
        cell.setFillColor(sf::Color(R, G, B));
    }
    Flipframe.draw(cell);
};

void Screen::drawScreen(std::vector<uint24_t> bitmap) {
    //clear screen, can do a selective clearing of an icremental counter if one is added.
    Flipframe.clear(sf::Color(20, 20, 20)); // curently clearing whole screen but if it takes a few cycles to issue a draw pixel command I think it shouldn't 

    //Title text
    loadGlobalFont("InputMonoNarrow-Light.ttf");

    std::stringstream ss;
    ss << std::uppercase << std::setw(2) << std::setfill('0') << resolution << " (" << sizeX << "x" << sizeY << ":";
    sf::Text txt(ss.str(), globalFont, 30);
    txt.setFillColor(sf::Color::White);
    txt.setPosition({ 10,5 });
    Flipframe.draw(txt);

    // Screen box
    sf::RectangleShape panelBg;
    panelBg.setPosition({ 0, 40 });
	float RectW = (Screen::sizeX * screenMap) + 2; //Neccessary, unfortunately
    float RectH = (Screen::sizeY * screenMap) + 2;
    panelBg.setSize({RectW,RectH});
    panelBg.setFillColor(sf::Color(30, 30, 30));
    panelBg.setOutlineColor(sf::Color(100, 100, 100));
    panelBg.setOutlineThickness(2);

    for (int i = 0; i < resolution; i++) {
        //sf::Vector2u map //Break vector into individual pixel data.
		if (!color) { // greyscale
			if (!Grey3Channel) { //single pixel
				uint8_t value[3] = {0x5F, 0x00, 0x00}; // temp value
				Screen::drawPixel(startX + i % sizeX * screenMap, startY + i / sizeX * screenMap, value, true);
			}
			else { // 3 simoultaniously
				uint8_t value[3] = {0x0F, 0x4F, 0x8F}; // temp value
				Screen::drawPixel(startX + i % sizeX * screenMap, startY + i / sizeX * screenMap, value, true, true);
			}
		}  
		else {
			//std::tuple<uint8_t, uint8_t, uint8_t>
			uint8_t value3[3] = {0x5F, 0x3A, 0xC2}; // temp value
			Screen::drawPixel(startX + i % sizeX * screenMap, startY + i / sizeX * screenMap, value3, true);

		} 
    };

}

bool Screen::loadCSVImage(const std::string& absolutePath)
{
    std::ifstream file(absolutePath);
    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open CSV file: " << absolutePath << "\n";
        return false;
    }
	std::cout << "Loading: " << absolutePath << "\n";

    std::string line;
    int y = 0;

    while (std::getline(file, line)) {
        if (y >= this->sizeY) break;   // Prevent overflow
        
        std::stringstream ss(line);
        std::string token;

        int x = 0;
        int rgbIndex = 0;
        uint8_t rgb[3] = {0,0,0};

        while (std::getline(ss, token, ',')) {

            if (rgbIndex < 3) {
                rgb[rgbIndex] = static_cast<uint8_t>(std::stoi(token));
                rgbIndex++;
            }

            if (rgbIndex == 3) {
                if (x < this->sizeX) {
                    this->drawPixel(x, y, rgb);
                }
                x++;
                rgbIndex = 0;
            }
        }

        y++;
    }

    file.close();
    return true;
}


uint32_t Screen::FHardwareInfoRequest() {
    uint32_t flag_btyes = 0x00000000;
	//For every atribute possibly needed, set bits in flag_bytes to represent it.
	// Flag bytes OR(+) with Screen space value shifted to correct bit position)
	bool colorSet = Screen::color; //Far left
    flag_btyes |= (uint32_t(color & 0x1) << 31);         // 1 bit for color capability
    flag_btyes |= (uint32_t(bitDepth & 0x07) << 28);     // 3 bits for bit depth
    flag_btyes |= (uint32_t(sizeX & 0x0FFF) << 16);      // 12 bits for X resolution
    flag_btyes |= (uint32_t(sizeY & 0x0FFF) << 4);       // 12 bits for Y resolution
	//1 + 3 + 12 + 12 = 28 ... 4 unused bits
    return flag_btyes;
}