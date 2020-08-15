#include "common.hpp"
#include "lodepng.h"

#include <ctime>
#include <unistd.h>

#define charSpriteSize 0x18000

static u16 bmpImageBuffer[256*192];
static u16 bgSpriteMem[(charSpriteSize/2)] = {0};
static u16 charSpriteMem[2][(charSpriteSize/2)*3];

static bool chracterSpriteLoaded = false;
static bool chracterSpriteFound[5] = {false};
static bool bgSpriteLoaded = false;

extern int studioBg;
extern u8 settingBits;
extern int iFps;

extern int bg3Main;
extern int bg2Main;
extern int bg3Sub;

extern bool showCursor;
extern int cursorAlpha;

bool animateBg = false;
bool bgCanAnimate = false;
static int bgAnimationFrame = 0;
static int bgAnimationCurrent = 0;
static int bgAnimationTime = 0;
//static int bgAnimationDelay = 0;
static int bgAnimation[8] = {100};

static int timeOutside = 0;	// 0 == Day, 1 == Sunset, 2 == Night

void GFX::resetCharStatus(int num) {
	if (num > -1) {
		chracterSpriteFound[num] = false;
		return;
	}

	chracterSpriteFound[0] = false;
	chracterSpriteFound[1] = false;
	chracterSpriteFound[2] = false;
	chracterSpriteFound[3] = false;
	chracterSpriteFound[4] = false;
}

void GFX::loadSheets() {
	FILE* file = fopen("nitro:/graphics/gui/title.bmp", "rb");

	if (file) {
		// Start loading
		fseek(file, 0xe, SEEK_SET);
		u8 pixelStart = (u8)fgetc(file) + 0xe;
		fseek(file, pixelStart, SEEK_SET);
		fread(bmpImageBuffer, 2, 0x18000/2, file);
		u16* src = bmpImageBuffer;
		int x = 0;
		int y = 191;
		for (int i=0; i<256*192; i++) {
			if (x >= 256) {
				x = 0;
				y--;
			}
			u16 val = *(src++);
			bgSpriteMem[y*256+x] = ((val>>10)&0x1f) | ((val)&(0x1f<<5)) | (val&0x1f)<<10 | BIT(15);
			x++;
		}
	}

	fclose(file);

	dmaCopyWords(1, (void*)bgSpriteMem[0], (void*)bgSpriteMem[charSpriteSize], 0x18000);
	dmaCopyWords(1, (void*)bgSpriteMem[0], (void*)bgSpriteMem[charSpriteSize*2], 0x18000);
}

void GFX::unloadSheets() {
}

static inline bool isDaytime(int hour, int minutes) {
	if ((hour >= 7 && hour < 20) || (hour == 20 && minutes >= 0 && minutes < 45)) {
		timeOutside = 0;
		return true;
	}
	return false;
}

static inline bool isEvening(int hour, int minutes) {
	if ((hour == 20 && minutes >= 45 && minutes < 60) || (hour == 21 && minutes >= 0 && minutes < 45)) {
		timeOutside = 1;
		return true;
	}
	return false;
}

void GFX::loadBgSprite(void) {
	if (bgSpriteLoaded) return;

	timeOutside = 2;	// Default is Nighttime

	const char* bgPath;
	time_t t = time(0);
	int hour = localtime(&t)->tm_hour;
	int minutes = localtime(&t)->tm_min;

	switch (studioBg) {
		case 0:
		default:
			bgPath = "nitro:/graphics/bg/bg_blue.png";
			break;
		case 1:
			if (isDaytime(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/bgDay_loversBell.png";
			} else if (isEvening(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/bgSunset_loversBell.png";
			} else {
				bgPath = "nitro:/graphics/bg/bgNight_loversBell.png";
			}
			break;
		case 2:
			if (isDaytime(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/bgDay_bougainville.png";
			} else if (isEvening(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/bgSunset_bougainville.png";
			} else {
				bgPath = "nitro:/graphics/bg/bgNight_bougainville.png";
			}
			break;
		case 3:
			bgPath = "nitro:/graphics/bg/bg_nin10Pro.png";
			break;
		case 4:
			bgPath = "nitro:/graphics/bg/bg_beautician4.png";
			break;
		case 5:
			bgPath = "nitro:/graphics/bg/bg_hairSalon4.png";
			break;
		case 6:
			bgPath = "nitro:/graphics/bg/bg_celestialHotel.png";
			break;
		case 7:
			bgPath = "nitro:/graphics/bg/bg_liveMusicClub4.png";
			break;
		case 8:
			bgPath = "nitro:/graphics/bg/bg_menswearShop.png";
			break;
		case 9:
			bgPath = "nitro:/graphics/bg/bg_VIP.png";
			break;
		case 10:
			bgPath = "nitro:/graphics/bg/bg_restauraunt4.png";
			break;
		case 11:
			bgPath = "nitro:/graphics/bg/bg_cinema.png";
			break;
		case 12:
			if (isDaytime(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/bgDay_tropicaBeach_0.png";
			} else if (isEvening(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/bgSunset_tropicaBeach_0.png";
			} else {
				bgPath = "nitro:/graphics/bg/bgNight_tropicaBeach.png";
			}
			break;
		case 13:
			if (isDaytime(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/bgDay_primrosePark.png";
			} else if (isEvening(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/bgSunset_primrosePark.png";
			} else {
				bgPath = "nitro:/graphics/bg/bgNight_primrosePark.png";
			}
			break;
		case 14:
			if (isDaytime(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/bgDay_cafe3.png";
			} else if (isEvening(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/bgSunset_cafe3.png";
			} else {
				bgPath = "nitro:/graphics/bg/bgNight_cafe3.png";
			}
			break;
		case 15:
			if (isDaytime(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/bgDay_mapleCrescent.png";
			} else if (isEvening(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/bgSunset_mapleCrescent.png";
			} else {
				bgPath = "nitro:/graphics/bg/bgNight_mapleCrescent.png";
			}
			break;
		case 16:
			bgPath = "nitro:/graphics/bg/bg_white.png";
			break;
		case 17:
			bgPath = "nitro:/graphics/bg/bg_yellow.png";
			break;
		case 18:
			bgPath = "nitro:/graphics/bg/bg_snowflakes.png";
			break;
		case 19:
			bgPath = "nitro:/graphics/bg/bg_pinkShine.png";
			break;
		case 20:
			bgPath = "nitro:/graphics/bg/bg_beigeWithCircles.png";
			break;
		case 21:
			bgPath = "nitro:/graphics/bg/bg_gradientPurple.png";
			break;
		case 22:
			bgPath = "nitro:/graphics/bg/bg_greenBirds.png";
			break;
		case 23:
			bgPath = "nitro:/graphics/bg/bg_circlePattern.png";
			break;
		case 24:
			bgPath = "nitro:/graphics/bg/bg_circleWindows.png";
			break;
		case 25:
			bgPath = "nitro:/graphics/bg/bg_bigHearts.png";
			break;
		case 26:
			bgPath = "nitro:/graphics/bg/bg_smallHearts.png";
			break;
		case 27:
			bgPath = "nitro:/graphics/bg/bg_park2Spring.png";
			break;
		case 28:
			bgPath = "nitro:/graphics/bg/bg_park2Summer.png";
			break;
		case 29:
			bgPath = "nitro:/graphics/bg/bg_park2Fall.png";
			break;
		case 30:
			bgPath = "nitro:/graphics/bg/bg_park2Winter.png";
			break;
		case 31:
			bgPath = "nitro:/graphics/bg/bg_downtownSpring.png";
			break;
		case 32:
			bgPath = "nitro:/graphics/bg/bg_downtownSummer.png";
			break;
		case 33:
			bgPath = "nitro:/graphics/bg/bg_downtownFall.png";
			break;
		case 34:
			bgPath = "nitro:/graphics/bg/bg_downtownWinter.png";
			break;
		case 35:
			bgPath = "nitro:/graphics/bg/bg_cafe2Spring.png";
			break;
		case 36:
			bgPath = "nitro:/graphics/bg/bg_cafe2Summer.png";
			break;
		case 37:
			bgPath = "nitro:/graphics/bg/bg_cafe2Fall.png";
			break;
		case 38:
			bgPath = "nitro:/graphics/bg/bg_cafe2Winter.png";
			break;
		case 39:
			if (isDaytime(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/bgDay_exhibitionHall2.png";
			} else {
				bgPath = "nitro:/graphics/bg/bgNight_exhibitionHall2.png";
			}
			break;
		case 40:
			bgPath = "nitro:/graphics/bg/bg_beautician2.png";
			break;
		case 41:
			bgPath = "nitro:/graphics/bg/bg_afterParty.png";
			break;
		case 42:
			bgPath = "nitro:/graphics/bg/bg_graceShop.png";
			break;
		case 43:
			bgPath = "nitro:/graphics/bg/bg_hairSalon1.png";
			break;
		case 44:
			bgPath = "nitro:/graphics/bg/bg_beautician1.png";
			break;
		case 45:
			bgPath = "nitro:/graphics/bg/bg_lifestyleShop2.png";
			break;
		case 46:
			bgPath = "nitro:/graphics/bg/bg_liveMusicClub2.png";
			break;
		case 47:
			bgPath = "nitro:/graphics/bg/bg_roseGarden2.png";
			break;
		case 48:
			bgPath = "nitro:/graphics/bg/bg_carringtonInstitute.png";
			break;
		case 49:
			bgPath = "nitro:/graphics/bg/bg_peachCastleOutside.png";
			break;
		case 50:
			bgPath = "nitro:/graphics/bg/bg_peachCastle.png";
			break;
	}
	FILE* bgFile = fopen(bgPath, "rb");
	//fread((void*)bgSpriteMem[0], 1, 0x200000, bgFile);
	fclose(bgFile);

	bgSpriteLoaded = true;
	bgAnimationFrame = 0;
	bgAnimationCurrent = 0;
	bgAnimationTime = 0;
	bgCanAnimate = false;

	// Load animated parts
	if (studioBg == 12 && (timeOutside == 0 || timeOutside == 1)) {
		if (timeOutside == 0) {
			bgPath = "nitro:/graphics/bg/bgDay_tropicaBeach_1.png";
		} else {
			bgPath = "nitro:/graphics/bg/bgSunset_tropicaBeach_1.png";
		}
		bgFile = fopen(bgPath, "rb");
		//fread((void*)bgSpriteMem[1], 1, 0x200000, bgFile);
		fclose(bgFile);
		if (timeOutside == 0) {
			bgPath = "nitro:/graphics/bg/bgDay_tropicaBeach_2.png";
		} else {
			bgPath = "nitro:/graphics/bg/bgSunset_tropicaBeach_2.png";
		}
		bgFile = fopen(bgPath, "rb");
		//fread((void*)bgSpriteMem[2], 1, 0x200000, bgFile);
		fclose(bgFile);
		//bgAnimationDelay = iFps;
		bgAnimation[0] = 0;
		bgAnimation[1] = 1;
		bgAnimation[2] = 2;
		bgAnimation[3] = 1;
		bgAnimation[4] = 100;
		bgCanAnimate = true;
	}

	bgSpriteLoaded = true;
}

void GFX::unloadBgSprite() {
	if (!bgSpriteLoaded) return;

	animateBg = false;
	bgCanAnimate = false;

	bgSpriteLoaded = false;
}

void GFX::reloadBgSprite() {
	unloadBgSprite();
	loadBgSprite();
}

bool GFX::loadCharSprite(int num, const char* t3xPathAllSeasons, const char* t3xPathOneSeason) {
	if (chracterSpriteLoaded) {
		chracterSpriteLoaded = false;
	}
	chracterSpriteFound[num] = false;
	bool allSeasons = true;
	bool fileFound = false;
	fileFound = (access(t3xPathAllSeasons, F_OK) == 0);
	if (!fileFound) {
		allSeasons = false;
		fileFound = (access(t3xPathOneSeason, F_OK) == 0);
	}

	if (!fileFound) {
		return false;
	}

	std::vector<unsigned char> image;
	unsigned width, height;
	lodepng::decode(image, width, height, allSeasons ? t3xPathAllSeasons : t3xPathOneSeason);
	for(unsigned i=0;i<image.size()/4;i++) {
		charSpriteMem[num][i] = image[i*4]>>3 | (image[(i*4)+1]>>3)<<5 | (image[(i*4)+2]>>3)<<10 | BIT(15);
	}

	chracterSpriteFound[num] = true;

	return true;
}

void GFX::loadCharSpriteMem(int num) {
	if (!chracterSpriteFound[num]) return;
	dmaCopyWords(1, bgSpriteMem, bmpImageBuffer, 0x18000);
	for (int i = 0; i < 256*192; i++) {
		if (charSpriteMem[num][i] != 0xFC1F) {
			bmpImageBuffer[i] = charSpriteMem[num][i];
		}
	}
	dmaCopyWordsAsynch(1, bmpImageBuffer, bgGetGfxPtr(bg3Sub), 0x18000);
	chracterSpriteLoaded = true;
}

void GFX::showBgSprite(int zoomIn) {
	if (!bgSpriteLoaded) return;
}

void GFX::animateBgSprite(void) {
	if (!animateBg) return;

	// Animate background
	bgAnimationTime++;
	if (bgAnimationTime >= iFps) {
		bgAnimationCurrent++;
		if (bgAnimation[bgAnimationCurrent] == 100) {
			// Reset animation
			bgAnimationCurrent = 0;
		}
		bgAnimationFrame = bgAnimation[bgAnimationCurrent];
		//bgSprite = C2D_SpriteSheetLoadFromMem(bgSpriteMem[bgAnimationFrame], 0x200000);
		bgAnimationTime = 0;
	}
}

void GFX::showCharSprite(int num, bool flipH, int zoomIn, int fadeAlpha, bool lightingEffects) {
	if (!chracterSpriteLoaded) return;

	int xPos = 0;
	int yPos = 0;
	if ((num == 3 || num == 4) && chracterSpriteFound[3]) {
		if (zoomIn == 0) {
			return;
		}
		yPos += 80;
		switch (num) {
			case 3:
				xPos -= 80;
				break;
			case 4:
				xPos += 80;
				break;
		}
	} else if (chracterSpriteFound[0] && chracterSpriteFound[1] && chracterSpriteFound[2]) {
		if (zoomIn == 1) {
		switch (num) {
			case 0:
				xPos -= 152;
				break;
			case 2:
				xPos += 152;
				break;
		}
		} else {
		switch (num) {
			case 0:
				xPos -= 64;
				break;
			case 2:
				xPos += 64;
				break;
		}
		}
	} else if (chracterSpriteFound[0] && chracterSpriteFound[1]) {
		if (zoomIn == 1) {
		switch (num) {
			case 0:
				xPos -= 80;
				break;
			case 1:
				xPos += 80;
				break;
		}
		} else {
		switch (num) {
			case 0:
				xPos -= 32;
				break;
			case 1:
				xPos += 32;
				break;
		}
		}
	}
}

void GFX::DrawSprite(int img, int x, int y, float ScaleX, float ScaleY) {
}

void GFX::DrawSpriteLinear(int img, int x, int y, float ScaleX, float ScaleY) {
}

void GFX::DrawSpriteBlend(int img, float x, float y, u32 color, float ScaleX, float ScaleY) {
}

void GFX::drawCursor(int cX, int cY) {
	if (cursorAlpha == 0) return;
	DrawSprite(sprites_cursor_idx, cX, cY);
}