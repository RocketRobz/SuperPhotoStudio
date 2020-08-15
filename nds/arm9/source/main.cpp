#include <maxmod9.h>
#include <nds.h>

#include <fat.h>
#include <limits.h>
#include <stdio.h>
#include <sys/stat.h>

#include <gl2d.h>
#include <string.h>
#include <unistd.h>

#include "rocketRobz.hpp"
#include "screen.hpp"

u32 hDown = 0;
u32 hHeld = 0;
touchPosition touch;

//---------------------------------------------------------------------------------
void stop(void) {
//---------------------------------------------------------------------------------
	while (1) {
		swiWaitForVBlank();
	}
}

void doPause() {
	while (1) {
		scanKeys();
		if (keysDown() & KEY_START)
			break;
		swiWaitForVBlank();
	}
	scanKeys();
}

int main(int argc, char **argv) {
	defaultExceptionHandler();

	while (1) {
		// Scan hid shared memory for input events
		scanKeys();

		hDown = keysDown();
		hHeld = keysHeld();

		touchRead(&touch);

		Gui::setScreen(std::make_unique<RocketRobz>(), false); // Set screen to product identification.
		Gui::ScreenLogic(hDown, hHeld, touch, true); // Call the logic of the current screen here.
	}

	return 0;
}
