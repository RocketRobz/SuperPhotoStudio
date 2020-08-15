#include <maxmod9.h>
#include <nds.h>

#include <fat.h>
#include <limits.h>
#include <stdio.h>
#include <sys/stat.h>

#include <gl2d.h>
#include <string.h>
#include <unistd.h>

#include "nitrofs.h"
#include "photoStudio.hpp"
#include "rocketRobz.hpp"
#include "screen.hpp"

bool isInit = true;
bool exiting = false;
bool rocketRobzScreen = true;
int delay = 0;

char verText[32];
int studioBg = 0;
int iFps = 60;

bool renderTop = true;	// Disable to prevent second character from flickering

void Play_Music(void) {
}

void sndSelect(void) {
}

void sndBack(void) {
}

void sndHighlight(void) {
}

bool showCursor = false;
int cursorAlpha = 0;

u32 hDown = 0;
u32 hHeld = 0;
touchPosition touch;

bool touchingBackButton(void) {
	return (touch.px >= 5 && touch.px < 5+32 && touch.py >= 158 && touch.py < 158+34);
}

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

	if (!fatInitDefault()) {
		consoleDemoInit();
		iprintf("fatInitDefault failed!");
		stop();
	}

	if (!nitroFSInit(argv[0])) {
		consoleDemoInit();
		iprintf("NitroFS init failed!");
		stop();
	}

	Gui::init();

	Gui::setScreen(std::make_unique<RocketRobz>(), false); // Set screen to product identification.

	while (1) {
		// Scan hid shared memory for input events
		scanKeys();

		hDown = keysDown();
		hHeld = keysHeld();

		if (exiting) {
			if (!fadeout)	break;
		}

		if (isInit) {
			delay++;
			if (rocketRobzScreen) {
				if (delay > iFps*9) {
					Gui::setScreen(std::make_unique<PhotoStudio>(), true); // Set after delay to the photo studio.
					isInit = false;
				}
			} else
			if (delay > iFps*6) {
				fadeout = true;
				fadealpha = 255;
				Gui::setScreen(std::make_unique<RocketRobz>(), true); // Set after delay to RocketRobz's screen.
				rocketRobzScreen = true;
			}
		}

		touchRead(&touch);

		Gui::ScreenLogic(hDown, hHeld, touch, true); // Call the logic of the current screen here.
		swiWaitForVBlank();
	}

	return 0;
}
