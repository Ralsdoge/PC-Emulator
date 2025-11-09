#pragma once
#include <cstdint> // bit types
#include <vector> // arrays
#include <string> // strings
#include <iostream> // console output
#include <sstream> // string stream
#include <iomanip> // io stream formatting
#include <SFML/Graphics.hpp> // Drawing to window and Textures
#include "memory.h" // Memory access
#include "screen.h" // Font loading
#include "filesystem" // Save images

// Global render window + text objects
static sf::RenderWindow window(sf::VideoMode({960u, 600u}), "Memory Viewer");
static int frameCounter = 0;
static sf::RenderTexture lastFrameTex; // Off-screen render texture


std::string dumpMem(const Memory& ramObj) { //Dump mem
    std::vector<uint8_t> ramDump = ramObj.ILLEGAL_dumpMem();
    std::ostringstream output;

    for (size_t i = 0; i < ramDump.size(); ++i) {
        output << "Address " << i << ": " << (int)ramDump[i] << "\n";
    }
    std::cout << output.str();
    return output.str();
}

// Draw current memory contents to a texture and then to the SFML window
inline void updateMemoryFrameRtex(const Memory& mem, const CPU& cpu, int pc = -1) {
    loadGlobalFont("InputMonoNarrow-Light.ttf");

    const int cols = 16;
    const float cellW = 45;
    const float cellH = 22;
    const float startX = 20;
    const float startY = 20;
    sf::Vector2u winSize = window.getSize();

    // Create or recreate off-screen texture if needed
    if (lastFrameTex.getSize().x != winSize.x || lastFrameTex.getSize().y != winSize.y) {
        if (!lastFrameTex.create(winSize.x, winSize.y)) {
            throw std::runtime_error("Failed to create render texture with window size");
        }
    }

    lastFrameTex.clear(sf::Color(20, 20, 20));

    for (int i = 0; i < mem.size; ++i) { //Memory drawing loop
        uint8_t value = mem.read(i);

        // Back Color
        sf::RectangleShape cell({ cellW, cellH });
        cell.setPosition(startX - 13.5 + i % cols * cellW, startY + i / cols * cellH);

        if (i == pc) {
            cell.setFillColor(sf::Color::Green);
            cell.setOutlineThickness(2);
        }
        else {
            if (!(i % 2)) {
                cell.setFillColor(sf::Color(60, 60, 60));
            }
            else {
                cell.setFillColor(sf::Color(40, 40, 40));
            }
        }
        // Text
        std::stringstream ss;
        ss << std::hex << std::uppercase << std::setw(2)
            << std::setfill('0') << (int)value;

        sf::Text txt(ss.str(), globalFont, 14);
        txt.setFillColor(sf::Color::White);

        int row = i / cols;
        int col = i % cols;

        txt.setPosition({ startX + col * cellW, startY + row * cellH });
        lastFrameTex.draw(cell);
        lastFrameTex.draw(txt);
    }

    //Draw register side pannel 
    float panelX = window.getSize().x - 160;
	float panelY = 0.0f;
	float panelW = 160.0f;
    float panelH = window.getSize().y;

    sf::RectangleShape panelBg;
    panelBg.setPosition({panelX, panelY});
    panelBg.setSize({panelW, panelH});
    panelBg.setFillColor(sf::Color(30, 30, 30));
    panelBg.setOutlineColor(sf::Color(100, 100, 100));
    panelBg.setOutlineThickness(2);
        
    // Draw CPU registers
    std::ostringstream oss;
    oss << std::hex << std::uppercase << std::setfill('0');
    oss << "PC : 0x" << std::setw(2) << cpu.getPC() << "\n";
    oss << "ACC: 0x" << std::setw(2) << (int)cpu.getACC() << "\n";
	oss << "Halted: " << (cpu.isHalted() ? "Yes" : "No") << "\n";

    sf::Text regText(oss.str(), globalFont, 16);
    regText.setFillColor(sf::Color::White);
    regText.setString(oss.str());
    regText.setPosition({ panelX+2, 20.f });

    //Draw to frame
    lastFrameTex.draw(panelBg);
    lastFrameTex.draw(regText);

    lastFrameTex.display();

    // Draw to on-screen window
    sf::Sprite spr(lastFrameTex.getTexture());
    window.clear();
    window.draw(spr);
    window.display();
}

// Save the current SFML frame to an image file numbered by frame
inline void exportMemoryFrameRtex(int pc = -1) {
    std::cout << "Attempt Export\n";

    if (!lastFrameTex.getTexture().getSize().x) {
        std::cerr << "No frame data available to export!\n";
        return;
    }

    sf::Image img = lastFrameTex.getTexture().copyToImage();

    std::ostringstream filename;
    filename << "frame_" << std::setw(3) << std::setfill('0')
        << frameCounter++ << ".png";

    if (!img.saveToFile(filename.str()))
        std::cerr << "Failed to save image!\n";
    else
        std::cout << "Exported: " << filename.str() << "\n";
}