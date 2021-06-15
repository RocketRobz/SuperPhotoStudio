#include <nds.h>
#include <maxmod9.h>

#include <fat.h>
#include <limits.h>
#include <stdio.h>
#include <sys/stat.h>

#include <string.h>
#include <unistd.h>

#include "myDSiMode.h"
#include "nitrofs.h"
#include "sound.h"
#include "photoStudio.hpp"
#include "productIdent.hpp"
#include "rocketRobz.hpp"
#include "screen.hpp"

bool mepFound = false;
bool fatInited = false;
bool isInit = true;
bool exiting = false;
bool rocketRobzScreen = false;
int delay = 0;

char verText[32];
int studioBg = 0;
int iFps = 60;

extern int bg3Main;
extern bool ditherlaceOnVBlank;
extern bool secondFrame;
bool renderTop = true;	// Disable to prevent second character from flickering
bool doScreenshot = false;

static bool streamStarted = false;
void Play_Music(void) {
	streamStarted ? snd().updateStream() : snd().beginStream();
	streamStarted = true;
}

void sndSelect(void) {
	snd().playSelect();
}

void sndBack(void) {
	snd().playBack();
}

void sndHighlight(void) {
	snd().playHighlight();
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

	fatInited = fatInitDefault();
	bool nitroFSInited = nitroFSInit(argv[0]);

	if (!fatInited && !nitroFSInited) {
		consoleDemoInit();
		iprintf("fatInitDefault failed!");
		stop();
	}

	if (!nitroFSInited) {
		consoleDemoInit();
		iprintf("NitroFS init failed!");
		stop();
	}

	mkdir("/_nds", 0777);

	if (!dsiFeatures()) {
		sysSetCartOwner (BUS_OWNER_ARM9);	// Allow arm9 to access GBA ROM (or in this case, the DS Memory Expansion Pak)
		*(vu32*)(0x08240000) = 1;
		mepFound = (*(vu32*)(0x08240000) == 1);
	}
	// Check for DS Debug RAM or DSi RAM
	*(vu32*)(0x02403FFC) = 1;
	*(vu32*)(0x02003FFC) = 0;

	snd();

	Gui::init();
	GFX::loadSheets();

	Gui::setScreen(std::make_unique<ProductIdent>(), false); // Set screen to product identification.

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

		if (dsiFeatures() && !ditherlaceOnVBlank) {
			secondFrame ? bgShow(bg3Main) : bgHide(bg3Main);
			secondFrame = !secondFrame;
		}
		Gui::DrawScreen();
		Gui::ScreenLogic(hDown, hHeld, touch, false); // Call the logic of the current screen here.

		if (doScreenshot && fatInited) {
			ditherlaceOnVBlank = true;
			extern void screenshotbmp(void);
			screenshotbmp();
			doScreenshot = false;
			ditherlaceOnVBlank = false;
		}

		if ((hDown & KEY_UP)
		|| (hDown & KEY_DOWN)
		|| (hDown & KEY_LEFT)
		|| (hDown & KEY_RIGHT))
		{
			showCursor = true;
		} else if (hDown & KEY_TOUCH)
		{
			showCursor = false;
		}

		cursorAlpha = (showCursor ? 255 : 0);

		int fadeFPS;
		switch (iFps) {
			default:
				fadeFPS = 8;
				break;
			case 30:
				fadeFPS = 16;
				break;
			case 24:
				fadeFPS = 20;
				break;
		}

		Gui::fadeEffects(fadeFPS, fadeFPS);

		swiWaitForVBlank();
	}

	return 0;
}
