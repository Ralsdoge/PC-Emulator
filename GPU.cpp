#include "gpu.h"

GPU::GPU(std::vector<Memory*>& memories, Screen& screen): //combined memory objsects and screen(s) 
	mems(memories), 
	scr(screen) 
	{}; //Constructor
uint8_t GPU::read(uint16_t addr) { // Read from memory helper
    for (auto* m : mems) {
        if (addr >= m->base && addr < m->base + m->size)
            return m->read(addr);
    }
    return 0; // default
}
void GPU::write(uint16_t addr, uint8_t value) { // write to memory helper
    for (auto* m : mems) {
        if (addr >= m->base && addr < m->base + m->size) {
            m->write(addr, value);
            return;
        }
    }
}
void GPU::tick() { //GPU tick like CPU tick
	uint8_t command = read(GPU_CMD);

	switch (command) {
		case 0x01: { //Draw Pixel Command
			pixelDraw();
			break;
		}
		case 0x02: { //Draw Screen Command
			drawScreen();
			break;
		}
		case 0x00: //No Operation
		default: { 
			break;
		}
	}
	write(GPU_CMD, 0);

}
void GPU::processDisplayDevice() {
	// Read display object variables
	uint32_t flagData = scr.FHardwareInfoRequest();
	for (int i = 0; i < 4; i++) { // for 4 eight byte segments break the 32 bit value and write sequentialy int memory at the FLAG_REGISTER marker
		write(FLAG_REGISTER + i, (flagData >> (24-8*i)) & 0xFF);
	}
	/* How to read:
	bool color      = (flagData >> 31) & 0x1;
	uint8_t bitDepth = (flagData >> 28) & 0x7;
	uint16_t sizeX   = (flagData >> 16) & 0xFFF;
	uint16_t sizeY   = (flagData >> 4)  & 0xFFF;
	*/
	// set flags
	// color supported? (Save 'clock' cycles from reading two extra locations)
	// bit depth? (I dont know what use this does besides us posibly being able to use a proper data type. Which would work itself out anyway.)
	// resolution? (Prevemt wasted cycles sending data? maybe? Not really..)
}

void GPU::pixelDraw() {
	uint8_t ColorDat[3] = {(0,0,0)};
	//if color flag is set to 0 dont read?
	uint8_t pix_X = read(X_POS);
	uint8_t pix_T = read(Y_POS);
	uint8_t Color_Flag = read(FLAG_REGISTER);
	if ((Color_Flag >> 7) & 0x01 ) { //check first bit of flag register for color flag and if it is 1 then do color
		//color
		ColorDat[0] = read(COLOR_R);
		ColorDat[1] = read(COLOR_G);
		ColorDat[2] = read(COLOR_B);
	} 
	else {//greyscale
		//if (!QuickDraw) enabled here (3 channel greyscale)
			ColorDat[0] = ColorDat[1] = ColorDat[2] = mem.read(COLOR_R);
	}
	scr.drawPixel(pix_X, pix_T, ColorDat);
}
void GPU::drawScreen() {
	// get screen data from memory?
	// scr.drawScreen(data);
}
/// Could have a triple pixel render function using the other two color bytes simultaniously. This would make the resoultionflag useful? No it wouldnt. Screen does pixel fitting.
