#include "common.hpp"
#include "color.h"
#include "lodepng.h"

#include <ctime>
#include <unistd.h>

#define charSpriteSize 0x18000

extern void bmpLoad(const char* filePath, u16* bgPath);

u16 bmpImageBuffer[2][256*192];
static u16 bgSpriteMem[(256*192)*3] = {0};
static u16 charSpriteMem[2][(256*192)*3];
static u8 charSpriteAlpha[2][(256*192)*3];

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
	bmpLoad("nitro:/graphics/gui/title.bmp", bgSpriteMem);

	dmaCopyWords(1, bgSpriteMem, bgSpriteMem+(charSpriteSize/2), 0x18000);
	dmaCopyWords(1, bgSpriteMem, bgSpriteMem+((charSpriteSize/2)*2), 0x18000);
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

	dmaFillWords(0xFFFFFFFF, bgGetGfxPtr(bg3Sub), 0x18000);

	timeOutside = 2;	// Default is Nighttime

	const char* bgPath;
	time_t t = time(0);
	int hour = localtime(&t)->tm_hour;
	int minutes = localtime(&t)->tm_min;

	switch (studioBg) {
		case 0:
		default:
			bgPath = "nitro:/graphics/bg/blue.png";
			break;
		case 1:
			if (isDaytime(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/Day_loversBell.png";
			} else if (isEvening(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/Sunset_loversBell.png";
			} else {
				bgPath = "nitro:/graphics/bg/Night_loversBell.png";
			}
			break;
		case 2:
			if (isDaytime(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/Day_bougainville.png";
			} else if (isEvening(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/Sunset_bougainville.png";
			} else {
				bgPath = "nitro:/graphics/bg/Night_bougainville.png";
			}
			break;
		case 3:
			bgPath = "nitro:/graphics/bg/nin10Pro.png";
			break;
		case 4:
			bgPath = "nitro:/graphics/bg/beautician4.png";
			break;
		case 5:
			bgPath = "nitro:/graphics/bg/hairSalon4.png";
			break;
		case 6:
			bgPath = "nitro:/graphics/bg/celestialHotel.png";
			break;
		case 7:
			bgPath = "nitro:/graphics/bg/liveMusicClub4.png";
			break;
		case 8:
			bgPath = "nitro:/graphics/bg/menswearShop.png";
			break;
		case 9:
			bgPath = "nitro:/graphics/bg/VIP.png";
			break;
		case 10:
			bgPath = "nitro:/graphics/bg/restauraunt4.png";
			break;
		case 11:
			bgPath = "nitro:/graphics/bg/cinema.png";
			break;
		case 12:
			if (isDaytime(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/Day_tropicaBeach.png";
			} else if (isEvening(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/Sunset_tropicaBeach.png";
			} else {
				bgPath = "nitro:/graphics/bg/Night_tropicaBeach.png";
			}
			break;
		case 13:
			if (isDaytime(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/Day_primrosePark.png";
			} else if (isEvening(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/Sunset_primrosePark.png";
			} else {
				bgPath = "nitro:/graphics/bg/Night_primrosePark.png";
			}
			break;
		case 14:
			if (isDaytime(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/Day_cafe3.png";
			} else if (isEvening(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/Sunset_cafe3.png";
			} else {
				bgPath = "nitro:/graphics/bg/Night_cafe3.png";
			}
			break;
		case 15:
			if (isDaytime(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/Day_mapleCrescent.png";
			} else if (isEvening(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/Sunset_mapleCrescent.png";
			} else {
				bgPath = "nitro:/graphics/bg/Night_mapleCrescent.png";
			}
			break;
		case 16:
			bgPath = "nitro:/graphics/bg/white.png";
			break;
		case 17:
			bgPath = "nitro:/graphics/bg/yellow.png";
			break;
		case 18:
			bgPath = "nitro:/graphics/bg/snowflakes.png";
			break;
		case 19:
			bgPath = "nitro:/graphics/bg/pinkShine.png";
			break;
		case 20:
			bgPath = "nitro:/graphics/bg/beigeWithCircles.png";
			break;
		case 21:
			bgPath = "nitro:/graphics/bg/gradientPurple.png";
			break;
		case 22:
			bgPath = "nitro:/graphics/bg/greenBirds.png";
			break;
		case 23:
			bgPath = "nitro:/graphics/bg/circlePattern.png";
			break;
		case 24:
			bgPath = "nitro:/graphics/bg/circleWindows.png";
			break;
		case 25:
			bgPath = "nitro:/graphics/bg/bigHearts.png";
			break;
		case 26:
			bgPath = "nitro:/graphics/bg/smallHearts.png";
			break;
		case 27:
			bgPath = "nitro:/graphics/bg/park2Spring.png";
			break;
		case 28:
			bgPath = "nitro:/graphics/bg/park2Summer.png";
			break;
		case 29:
			bgPath = "nitro:/graphics/bg/park2Fall.png";
			break;
		case 30:
			bgPath = "nitro:/graphics/bg/park2Winter.png";
			break;
		case 31:
			bgPath = "nitro:/graphics/bg/downtownSpring.png";
			break;
		case 32:
			bgPath = "nitro:/graphics/bg/downtownSummer.png";
			break;
		case 33:
			bgPath = "nitro:/graphics/bg/downtownFall.png";
			break;
		case 34:
			bgPath = "nitro:/graphics/bg/downtownWinter.png";
			break;
		case 35:
			bgPath = "nitro:/graphics/bg/cafe2Spring.png";
			break;
		case 36:
			bgPath = "nitro:/graphics/bg/cafe2Summer.png";
			break;
		case 37:
			bgPath = "nitro:/graphics/bg/cafe2Fall.png";
			break;
		case 38:
			bgPath = "nitro:/graphics/bg/cafe2Winter.png";
			break;
		case 39:
			if (isDaytime(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/Day_exhibitionHall2.png";
			} else {
				bgPath = "nitro:/graphics/bg/Night_exhibitionHall2.png";
			}
			break;
		case 40:
			bgPath = "nitro:/graphics/bg/beautician2.png";
			break;
		case 41:
			bgPath = "nitro:/graphics/bg/afterParty.png";
			break;
		case 42:
			bgPath = "nitro:/graphics/bg/graceShop.png";
			break;
		case 43:
			bgPath = "nitro:/graphics/bg/hairSalon1.png";
			break;
		case 44:
			bgPath = "nitro:/graphics/bg/beautician1.png";
			break;
		case 45:
			bgPath = "nitro:/graphics/bg/lifestyleShop2.png";
			break;
		case 46:
			bgPath = "nitro:/graphics/bg/liveMusicClub2.png";
			break;
		case 47:
			bgPath = "nitro:/graphics/bg/roseGarden2.png";
			break;
		case 48:
			bgPath = "nitro:/graphics/bg/carringtonInstitute.png";
			break;
		case 49:
			bgPath = "nitro:/graphics/bg/peachCastleOutside.png";
			break;
		case 50:
			bgPath = "nitro:/graphics/bg/peachCastle.png";
			break;
	}
	std::vector<unsigned char> image;
	unsigned width, height;
	lodepng::decode(image, width, height, bgPath);
	for(unsigned i=0;i<image.size()/4;i++) {
		bgSpriteMem[i] = image[i*4]>>3 | (image[(i*4)+1]>>3)<<5 | (image[(i*4)+2]>>3)<<10 | BIT(15);
	}

	bgSpriteLoaded = true;
	bgAnimationFrame = 0;
	bgAnimationCurrent = 0;
	bgAnimationTime = 0;
	bgCanAnimate = false;

	// Load animated parts
	/*if (studioBg == 12 && (timeOutside == 0 || timeOutside == 1)) {
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
	}*/

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
	dmaCopyWords(0, bgSpriteMem, bmpImageBuffer, 0x18000);
	dmaCopyWordsAsynch(0, bmpImageBuffer, bgGetGfxPtr(bg3Sub), 0x18000);
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
		charSpriteAlpha[num][i] = image[(i*4)+3];
	}

	chracterSpriteFound[num] = true;

	return true;
}

void GFX::loadCharSpriteMem(int zoomIn, bool* flipH) {
	dmaCopyWords(0, bgSpriteMem+((0x18000/2)*zoomIn), bmpImageBuffer[0], 0x18000);
	if (!chracterSpriteFound[0]) {
		dmaCopyWordsAsynch(0, bmpImageBuffer[0], bgGetGfxPtr(bg3Sub), 0x18000);
		return;
	}

	u16 fg = 0;
	u8 blendAlpha = 0;

	switch (studioBg) {
		default:
			break;
		case 7:
			fg = RGB15(0, 0, 95/8);	// Tint for Live Music Club 4
			blendAlpha = 32;
			break;
		case 46:
			fg = RGB15(31/8, 31/8, 95/8);	// Tint for Live Music Club 2
			blendAlpha = 24;
			break;
		case 11:
			fg = RGB15(191/8, 63/8, 87/8);	// Tint for Cinema
			blendAlpha = 16;
			break;
		case 1:
		case 12:
		case 13:
		case 14:
		case 15:
			blendAlpha = 32;
			if (timeOutside == 1) {
				fg = RGB15(95/8, 47/8, 0);	// Tint for Sunset
			} else if (timeOutside == 2) {
				fg = RGB15(0, 0, 95/8);	// Tint for Nighttime
			}
		break;
	}

	int buffer = 0;
	int x2 = 0;
	u16 color = 0;
	if (chracterSpriteFound[1]) {
		// Character 1
		for (int y = 0; y < 192; y++) {
		  x2 = flipH[0] ? 255 : 0;
		  x2 -= (zoomIn==1 ? 64 : 26);
		  for (int x = 0; x < 256; x++) {
			if (x2 >= 0 && x2 < 256 && charSpriteAlpha[0][((y*256)+x)+((256*192)*zoomIn)] != 0) {
				color = charSpriteMem[0][((y*256)+x)+((256*192)*zoomIn)];
				if (blendAlpha > 0) {
					color = alphablend(fg, charSpriteMem[0][((y*256)+x)+((256*192)*zoomIn)], blendAlpha);
				}
				if (charSpriteAlpha[0][((y*256)+x)+((256*192)*zoomIn)] == 255) {
					bmpImageBuffer[0][(y*256)+x2] = color;
				} else {
					bmpImageBuffer[0][(y*256)+x2] = alphablend(color, bgSpriteMem[((y*256)+x2)+((256*192)*zoomIn)], charSpriteAlpha[0][((y*256)+x)+((256*192)*zoomIn)]);
				}
			}
			flipH[0] ? x2-- : x2++;
		  }
		}
		// Character 2
		dmaCopyWords(0, bmpImageBuffer[0], bmpImageBuffer[1], 0x18000);
		for (int y = 0; y < 192; y++) {
		  x2 = flipH[1] ? 255 : 0;
		  x2 += (zoomIn==1 ? 64 : 26);
		  for (int x = 0; x < 256; x++) {
			if (x2 >= 0 && x2 < 256 && charSpriteAlpha[1][((y*256)+x)+((256*192)*zoomIn)] != 0) {
				color = charSpriteMem[1][((y*256)+x)+((256*192)*zoomIn)];
				if (blendAlpha > 0) {
					color = alphablend(fg, charSpriteMem[1][((y*256)+x)+((256*192)*zoomIn)], blendAlpha);
				}
				if (charSpriteAlpha[1][((y*256)+x)+((256*192)*zoomIn)] == 255) {
					bmpImageBuffer[1][(y*256)+x2] = color;
				} else {
					bmpImageBuffer[1][(y*256)+x2] = alphablend(color, bmpImageBuffer[0][(y*256)+x2], charSpriteAlpha[1][((y*256)+x)+((256*192)*zoomIn)]);
				}
			}
			flipH[1] ? x2-- : x2++;
		  }
		}
		buffer++;
	} else
	for (int y = 0; y < 192; y++) {
	  x2 = flipH[0] ? 255 : 0;
	  for (int x = 0; x < 256; x++) {
		if (charSpriteAlpha[0][((y*256)+x)+((256*192)*zoomIn)] != 0) {
			color = charSpriteMem[0][((y*256)+x)+((256*192)*zoomIn)];
			if (blendAlpha > 0) {
				color = alphablend(fg, charSpriteMem[0][((y*256)+x)+((256*192)*zoomIn)], blendAlpha);
			}
			if (charSpriteAlpha[0][((y*256)+x)+((256*192)*zoomIn)] == 255) {
				bmpImageBuffer[0][(y*256)+x2] = color;
			} else {
				bmpImageBuffer[0][(y*256)+x2] = alphablend(color, bgSpriteMem[((y*256)+x2)+((256*192)*zoomIn)], charSpriteAlpha[0][((y*256)+x)+((256*192)*zoomIn)]);
			}
		}
		flipH[0] ? x2-- : x2++;
	  }
	}
	dmaCopyWordsAsynch(0, bmpImageBuffer[buffer], bgGetGfxPtr(bg3Sub), 0x18000);
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
}