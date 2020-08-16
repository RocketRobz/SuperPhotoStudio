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
#include "soundbank.h"
#include "photoStudio.hpp"
#include "productIdent.hpp"
#include "rocketRobz.hpp"
#include "screen.hpp"

bool isInit = true;
bool exiting = false;
bool rocketRobzScreen = false;
int delay = 0;

char verText[32];
int studioBg = 0;
int iFps = 60;

bool renderTop = true;	// Disable to prevent second character from flickering

static mm_sound_effect snd_select;
static mm_sound_effect snd_back;
static mm_sound_effect snd_highlight;

void Play_Music(void) {
}

void sndSelect(void) {
	mmEffectEx(&snd_select);
}

void sndBack(void) {
	mmEffectEx(&snd_back);
}

void sndHighlight(void) {
	mmEffectEx(&snd_highlight);
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

	FILE* soundBank = fopen("nitro:/soundbank.bin", "rb");
	fread((void*)0x02FA0000, 1, 0x20000, soundBank);
	fclose(soundBank);

	mmInitDefaultMem((mm_addr)0x02FA0000);

	mmLoadEffect( SFX_SELECT );
	mmLoadEffect( SFX_BACK );
	mmLoadEffect( SFX_HIGHLIGHT );

	snd_select = {
		{ SFX_SELECT } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		128,	// panning
	};

	snd_back = {
		{ SFX_BACK } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		128,	// panning
	};

	snd_highlight = {
		{ SFX_HIGHLIGHT } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		128,	// panning
	};

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

		Gui::ScreenLogic(hDown, hHeld, touch, false); // Call the logic of the current screen here.

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
