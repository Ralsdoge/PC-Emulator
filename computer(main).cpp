// Idea make a viewer of memory and asembly instructions/procceses. 
// Then run simple assembly programs and show what is happening behind the scenes.
// Simple program:
#include "memory.h"
#include "cpu.h"
#include "passthrough.h"
#include "screen.h"
#include <iostream>

int main() {
    // Alocate and set up memory 
    Memory ram1(0,256); // Create
	std::vector<Memory*> memory = { &ram1 }; // add to vector to combine for the CPU
    /*Memory ram2(256,1024); // Create
    std::vector<Memory*> memory = { &ram1, &ram2 };*/
	CPU cpu(memory); // Feed memory to CPU
    
	// Write program to memory
    ram1.write(0, LDA); ram1.write(1, 10);
    ram1.write(2, ADD); ram1.write(3, 11);
    ram1.write(4, STA); ram1.write(5, 12);
    ram1.write(6, HLT);

    ram1.write(10, 5);   // value1
    ram1.write(11, 7);   // value2
    
    //Connect a screen to system
    Screen(60,40,false,4,3);


    //cpu.run();

// Processor stepping loop (No User edits below here)
    //Window loop stepthrough Vars.
    bool running = false;      // true => continuous stepping
    bool stepRequested = false; // true when user requests a single step
    bool stepNRequested = false;
    int stepsRemaining = 0;    // for stepping N instructions
    
    updateMemoryFrameRtex(ram1, cpu, cpu.getPC());//Initial state
    //Window loop    
    while (window.isOpen()) { // Main loop
        // 1) Handle OS/window events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::Space:
                    // Request exactly one step
                    stepRequested = true;
                    running = false; // stop continuous run if running
                    break;
                case sf::Keyboard::R:
                    // Toggle continuous run
                    running = !running;
                    break;
                case sf::Keyboard::N:
                    // Step N instructions, example N = 10
                    stepsRemaining = 5;
                    stepNRequested = true;
                    running = false;
                    break;
                case sf::Keyboard::E:
                    // Export the current frame to an image
                    exportMemoryFrameRtex();
                    break;
                case sf::Keyboard::Escape:
                    window.close();
                    break;
                default:
                    break;
                }
            }
        } // end event loop

        // 2) Perform stepping work (non-blocking)
        if (stepRequested) {
            cpu.step();
            stepRequested = false;
        }

        if (stepNRequested && stepsRemaining > 0) {
            cpu.step();
            --stepsRemaining;
            if (stepsRemaining == 0) stepNRequested = false;
        }

        if (running) {
            // If you want a throttle to avoid burning CPU, sleep for a tiny time
            // and/or only step every N ms:
            cpu.step();
            sf::sleep(sf::milliseconds(10)); // adjust speed: smaller = faster
        }

        // 3) Update the display once per loop iteration (after any steps)
        updateMemoryFrameRtex(ram1, cpu, cpu.getPC());

        // Optionally auto-export each frame while running:
        // if (running) exportMemoryFrameRtex();

        // 4) If CPU hal3ted, optionally stop running
        if (cpu.isHalted()) {
            running = false;
        }
    } // end main loop

    return 0;

    
}