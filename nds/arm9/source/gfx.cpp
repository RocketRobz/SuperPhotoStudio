#include "common.hpp"
#include "color.h"
#include "lodepng.h"
#include "tonccpy.h"

#include <ctime>
#include <unistd.h>

#define charSpriteSize 0x18000

u16 bmpImageBuffer[2][256*192];
static u16 bgSpriteMem[(256*192)*3] = {0};
static u16 charSpriteMem[2][(256*192)*3];
static u8 charSpriteAlpha[2][(256*192)*3];
static u16* charSpriteMem3 = (u16*)0x02480000;
static u16* charSpriteMem4 = (u16*)0x024C8000;
static u16* charSpriteMem5 = (u16*)0x02510000;
static u8* charSpriteAlpha3 = (u8*)0x02558000;
static u8* charSpriteAlpha4 = (u8*)0x0257C000;
static u8* charSpriteAlpha5 = (u8*)0x025A0000;
static u16* bgSpriteMemExt[3] = {(u16*)0x025C8000, (u16*)0x02610000, (u16*)0x02658000};

static bool chracterSpriteLoaded = false;
static bool chracterSpriteFound[5] = {false};
static bool bgSpriteLoaded = false;

static bool titleBottomLoaded = false;
static bool animateTitle = true;

extern int studioBg;
extern u8 settingBits;
extern int iFps;

extern int bg3Main;
extern int bg2Main;
extern int bg3Sub;

extern bool showCursor;
extern int cursorAlpha;

bool animateBg = false;
static int bgAnimationFrame = 0;
static int bgAnimationCurrent = 0;
static int bgAnimationTime = 0;
static int bgAnimationDelay = 0;
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

extern int characterLimit;

void GFX::loadSheets() {
	if (!isDSiMode()) {
		//sysSetCartOwner (BUS_OWNER_ARM9);	// Allow arm9 to access GBA ROM (or in this case, the DS Memory Expansion Pak)
		*(vu32*)(0x08240000) = 1;
		if (*(vu32*)(0x08240000) == 1) {
			charSpriteMem3 = (u16*)0x09000000;
			charSpriteMem4 = (u16*)0x09048000;
			charSpriteMem5 = (u16*)0x09090000;
			charSpriteAlpha3 = (u8*)0x090D8000;
			charSpriteAlpha4 = (u8*)0x090FC000;
			charSpriteAlpha5 = (u8*)0x09120000;
			bgSpriteMemExt[0] = (u16*)0x09148000;
			bgSpriteMemExt[1] = (u16*)0x09190000;
			bgSpriteMemExt[2] = (u16*)0x091D8000;
		}
	}
	// Check for DS Debug RAM or DSi RAM
	*(vu32*)(0x0279FFFC) = 1;
	*(vu32*)(0x0239FFFC) = 0;
	if (*(vu32*)(0x0279FFFC) == 1 || *(vu32*)(0x08240000) == 1) {
		characterLimit = 4;	// Up the limit from 2 to 5 characters
	}

	int metalXpos = 0;
	int metalYpos = 0;

	std::vector<unsigned char> image;
	unsigned width, height;
	lodepng::decode(image, width, height, "nitro:/graphics/gui/titleMetal.png");
	for(unsigned i=0;i<image.size()/4;i++) {
		charSpriteMem[0][i] = image[i*4]>>3 | (image[(i*4)+1]>>3)<<5 | (image[(i*4)+2]>>3)<<10 | BIT(15);
	}
	image.clear();
	lodepng::decode(image, width, height, "nitro:/graphics/gui/title.png");
	for(unsigned i=0;i<image.size()/4;i++) {
		bmpImageBuffer[1][i] = image[i*4]>>3 | (image[(i*4)+1]>>3)<<5 | (image[(i*4)+2]>>3)<<10 | BIT(15);
		charSpriteAlpha[0][i] = image[(i*4)+3];
		if (i >= 256*72 && i < 256*131) {
			metalXpos++;
			if (metalXpos == 256) {
				metalXpos = 0;
				metalYpos++;
			}
		}
		if (image[(i*4)+3] == 255) {
			bmpImageBuffer[0][i] = bmpImageBuffer[1][i];
		} else if (image[(i*4)+3] == 0) {
			bmpImageBuffer[0][i] = charSpriteMem[0][(metalYpos*384)+metalXpos];
		} else if (i >= 256*72 && i < 256*131) {
			bmpImageBuffer[0][i] = alphablend(bmpImageBuffer[1][i], charSpriteMem[0][(metalYpos*384)+metalXpos], image[(i*4)+3]);
		}
	}
	image.clear();
	lodepng::decode(image, width, height, "nitro:/graphics/gui/photo_bg.png");
	for(unsigned i=0;i<image.size()/4;i++) {
		charSpriteMem[1][i] = image[i*4]>>3 | (image[(i*4)+1]>>3)<<5 | (image[(i*4)+2]>>3)<<10 | BIT(15);
	}

	dmaCopyWords(0, bmpImageBuffer[0], bgSpriteMem, 0x18000);
	dmaCopyWords(0, bmpImageBuffer[0], bgSpriteMem+(charSpriteSize/2), 0x18000);
	dmaCopyWords(0, bmpImageBuffer[0], bgSpriteMem+((charSpriteSize/2)*2), 0x18000);
}

void updateTitleScreen(const int metalXposBase) {
	if (!animateTitle) return;

	int metalXpos = metalXposBase;
	int metalYpos = 0;

	for (int i = 256*72; i < 256*131; i++) {
		metalXpos++;
		if (metalXpos == 256+metalXposBase) {
			metalXpos = metalXposBase;
			metalYpos++;
		}
		if (charSpriteAlpha[0][i] != 255) {
		  if (charSpriteAlpha[0][i] == 0) {
			bmpImageBuffer[0][i] = charSpriteMem[0][(metalYpos*384)+metalXpos];
		  } else if (i >= 256*72 && i < 256*131) {
			bmpImageBuffer[0][i] = alphablend(bmpImageBuffer[1][i], charSpriteMem[0][(metalYpos*384)+metalXpos], charSpriteAlpha[0][i]);
		  }
		}
	}

	dmaCopyWordsAsynch(0, bmpImageBuffer[0], bgGetGfxPtr(bg3Sub), 0x18000);
	if (!titleBottomLoaded) {
		dmaCopyWordsAsynch(1, charSpriteMem[1], bgGetGfxPtr(bg3Main), 0x18000);
		titleBottomLoaded = true;
	}
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

	animateTitle = false;

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
				bgPath = "nitro:/graphics/bg/Day_tropicaBeach_0.png";
			} else if (isEvening(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/Sunset_tropicaBeach_0.png";
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
		case 51:
			bgPath = "nitro:/graphics/bg/liveMusicClub3_0.png";
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
	animateBg = false;

	// Load animated parts
	if (*(vu32*)(0x0279FFFC) == 1 && (studioBg == 12 || studioBg == 51)
	&& (timeOutside == 0 || timeOutside == 1 || studioBg == 51)) {
		image.clear();
		if (studioBg == 51) {
			bgPath = "nitro:/graphics/bg/liveMusicClub3_1.png";
		} else if (timeOutside == 0) {
			bgPath = "nitro:/graphics/bg/Day_tropicaBeach_1.png";
		} else {
			bgPath = "nitro:/graphics/bg/Sunset_tropicaBeach_1.png";
		}
		lodepng::decode(image, width, height, bgPath);
		for(unsigned i=0;i<image.size()/4;i++) {
			bgSpriteMemExt[0][i] = image[i*4]>>3 | (image[(i*4)+1]>>3)<<5 | (image[(i*4)+2]>>3)<<10 | BIT(15);
		}
		image.clear();
		if (studioBg == 51) {
			bgPath = "nitro:/graphics/bg/liveMusicClub3_2.png";
		} else if (timeOutside == 0) {
			bgPath = "nitro:/graphics/bg/Day_tropicaBeach_2.png";
		} else {
			bgPath = "nitro:/graphics/bg/Sunset_tropicaBeach_2.png";
		}
		lodepng::decode(image, width, height, bgPath);
		for(unsigned i=0;i<image.size()/4;i++) {
			bgSpriteMemExt[1][i] = image[i*4]>>3 | (image[(i*4)+1]>>3)<<5 | (image[(i*4)+2]>>3)<<10 | BIT(15);
		}
		if (studioBg == 51) {
			image.clear();
			bgPath = "nitro:/graphics/bg/liveMusicClub3_3.png";
			lodepng::decode(image, width, height, bgPath);
			for(unsigned i=0;i<image.size()/4;i++) {
				bgSpriteMemExt[2][i] = image[i*4]>>3 | (image[(i*4)+1]>>3)<<5 | (image[(i*4)+2]>>3)<<10 | BIT(15);
			}
			bgAnimationDelay = iFps/2;
			bgAnimation[0] = 0;
			bgAnimation[1] = 1;
			bgAnimation[2] = 2;
			bgAnimation[3] = 3;
		} else {
			bgAnimationDelay = iFps;
			bgAnimation[0] = 0;
			bgAnimation[1] = 1;
			bgAnimation[2] = 2;
			bgAnimation[3] = 1;
		}
		bgAnimation[4] = 100;
		animateBg = true;
	}

	bgSpriteLoaded = true;
}

void GFX::unloadBgSprite() {
	if (!bgSpriteLoaded) return;

	animateBg = false;

	bgSpriteLoaded = false;
}

void GFX::reloadBgSprite() {
	unloadBgSprite();
	loadBgSprite();
	dmaCopyWords(0, bgSpriteMem, bmpImageBuffer[0], 0x18000);
	dmaCopyWordsAsynch(0, bmpImageBuffer[0], bgGetGfxPtr(bg3Sub), 0x18000);
}

bool GFX::loadCharSprite(int num, const char* t3xPathAllSeasons, const char* t3xPathOneSeason) {
	animateTitle = false;

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
	if (num == 4) {
		for(unsigned i=0;i<image.size()/4;i++) {
			charSpriteMem5[i] = image[i*4]>>3 | (image[(i*4)+1]>>3)<<5 | (image[(i*4)+2]>>3)<<10 | BIT(15);
			tonccpy(&charSpriteAlpha5[i], &image[(i*4)+3], 1);
		}
	} else if (num == 3) {
		for(unsigned i=0;i<image.size()/4;i++) {
			charSpriteMem4[i] = image[i*4]>>3 | (image[(i*4)+1]>>3)<<5 | (image[(i*4)+2]>>3)<<10 | BIT(15);
			tonccpy(&charSpriteAlpha4[i], &image[(i*4)+3], 1);
		}
	} else if (num == 2) {
		for(unsigned i=0;i<image.size()/4;i++) {
			charSpriteMem3[i] = image[i*4]>>3 | (image[(i*4)+1]>>3)<<5 | (image[(i*4)+2]>>3)<<10 | BIT(15);
			tonccpy(&charSpriteAlpha3[i], &image[(i*4)+3], 1);
		}
	} else {
		for(unsigned i=0;i<image.size()/4;i++) {
			charSpriteMem[num][i] = image[i*4]>>3 | (image[(i*4)+1]>>3)<<5 | (image[(i*4)+2]>>3)<<10 | BIT(15);
			charSpriteAlpha[num][i] = image[(i*4)+3];
		}
	}

	chracterSpriteFound[num] = true;

	return true;
}

ITCM_CODE void GFX::loadCharSpriteMem(int zoomIn, bool* flipH) {
	u16* bgLoc = 0;
	if (bgAnimationFrame == 0) {
		bgLoc = bgSpriteMem;
	} else {
		bgLoc = bgSpriteMemExt[bgAnimationFrame-1];
	}

	dmaCopyWords(0, bgLoc+((256*192)*zoomIn), bmpImageBuffer[0], 0x18000);
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
			if (timeOutside > 0) {
				blendAlpha = 32;
			}
			if (timeOutside == 1) {
				fg = RGB15(95/8, 47/8, 0);	// Tint for Sunset
			} else if (timeOutside == 2) {
				fg = RGB15(0, 0, 95/8);	// Tint for Nighttime
			}
			break;
		case 51:
			fg = RGB15(0, 0, 0);	// Tint for Live Music Club 3
			if (bgAnimationFrame==1 || bgAnimationFrame==3) {
				blendAlpha = 16;
			} else {
				blendAlpha = 40;
			}
			break;
	}

	int buffer = 0;
	int x2 = 0;
	u16 color = 0;
	if (chracterSpriteFound[2]) {
		// Character 1
		for (int y = 0; y < 192; y++) {
		  x2 = flipH[0] ? 255 : 0;
		  x2 -= (zoomIn==1 ? 96 : 50);
		  for (int x = 0; x < 256; x++) {
			if (x2 >= 0 && x2 < 256 && charSpriteAlpha[0][((y*256)+x)+((256*192)*zoomIn)] != 0) {
				color = charSpriteMem[0][((y*256)+x)+((256*192)*zoomIn)];
				if (blendAlpha > 0) {
					color = alphablend(fg, charSpriteMem[0][((y*256)+x)+((256*192)*zoomIn)], blendAlpha);
				}
				if (charSpriteAlpha[0][((y*256)+x)+((256*192)*zoomIn)] == 255) {
					bmpImageBuffer[0][(y*256)+x2] = color;
				} else {
					bmpImageBuffer[0][(y*256)+x2] = alphablend(color, bgLoc[((y*256)+x2)+((256*192)*zoomIn)], charSpriteAlpha[0][((y*256)+x)+((256*192)*zoomIn)]);
				}
			}
			flipH[0] ? x2-- : x2++;
		  }
		}
		// Character 2
		dmaCopyWords(0, bmpImageBuffer[0], bmpImageBuffer[1], 0x18000);
		for (int y = 0; y < 192; y++) {
		  x2 = flipH[1] ? 255 : 0;
		  for (int x = 0; x < 256; x++) {
			if (charSpriteAlpha[1][((y*256)+x)+((256*192)*zoomIn)] != 0) {
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
		// Character 3
		dmaCopyWords(0, bmpImageBuffer[1], bmpImageBuffer[0], 0x18000);
		for (int y = 0; y < 192; y++) {
		  x2 = flipH[2] ? 255 : 0;
		  x2 += (zoomIn==1 ? 96 : 50);
		  for (int x = 0; x < 256; x++) {
			if (x2 >= 0 && x2 < 256 && charSpriteAlpha3[((y*256)+x)+((256*192)*zoomIn)] != 0) {
				color = charSpriteMem3[((y*256)+x)+((256*192)*zoomIn)];
				if (blendAlpha > 0) {
					color = alphablend(fg, charSpriteMem3[((y*256)+x)+((256*192)*zoomIn)], blendAlpha);
				}
				if (charSpriteAlpha3[((y*256)+x)+((256*192)*zoomIn)] == 255) {
					bmpImageBuffer[0][(y*256)+x2] = color;
				} else {
					bmpImageBuffer[0][(y*256)+x2] = alphablend(color, bmpImageBuffer[1][(y*256)+x2], charSpriteAlpha3[((y*256)+x)+((256*192)*zoomIn)]);
				}
			}
			flipH[2] ? x2-- : x2++;
		  }
		}
	} else
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
					bmpImageBuffer[0][(y*256)+x2] = alphablend(color, bgLoc[((y*256)+x2)+((256*192)*zoomIn)], charSpriteAlpha[0][((y*256)+x)+((256*192)*zoomIn)]);
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
				bmpImageBuffer[0][(y*256)+x2] = alphablend(color, bgLoc[((y*256)+x2)+((256*192)*zoomIn)], charSpriteAlpha[0][((y*256)+x)+((256*192)*zoomIn)]);
			}
		}
		flipH[0] ? x2-- : x2++;
	  }
	}

	if (chracterSpriteFound[3]) {
		// Continued from "if (chracterSpriteFound[2])"
		// Character 4
		dmaCopyWords(0, bmpImageBuffer[0], bmpImageBuffer[1], 0x18000);
		int y2 = 72;
		for (int y = 0; y < 120; y++) {
		  x2 = flipH[3] ? 255 : 0;
		  x2 -= (zoomIn==1 ? 64 : 26);
		  for (int x = 0; x < 256; x++) {
			if (x2 >= 0 && x2 < 256 && charSpriteAlpha4[((y*256)+x)+((256*192)*zoomIn)] != 0) {
				color = charSpriteMem4[((y*256)+x)+((256*192)*zoomIn)];
				if (blendAlpha > 0) {
					color = alphablend(fg, charSpriteMem4[((y*256)+x)+((256*192)*zoomIn)], blendAlpha);
				}
				if (charSpriteAlpha4[((y*256)+x)+((256*192)*zoomIn)] == 255) {
					bmpImageBuffer[1][(y2*256)+x2] = color;
				} else {
					bmpImageBuffer[1][(y2*256)+x2] = alphablend(color, bmpImageBuffer[0][(y2*256)+x2], charSpriteAlpha4[((y*256)+x)+((256*192)*zoomIn)]);
				}
			}
			flipH[3] ? x2-- : x2++;
		  }
		  y2++;
		}
		buffer++;
	}
	if (chracterSpriteFound[4]) {
		// Character 5
		dmaCopyWords(0, bmpImageBuffer[1], bmpImageBuffer[0], 0x18000);
		int y2 = 72;
		for (int y = 0; y < 120; y++) {
		  x2 = flipH[4] ? 255 : 0;
		  x2 += (zoomIn==1 ? 64 : 26);
		  for (int x = 0; x < 256; x++) {
			if (x2 >= 0 && x2 < 256 && charSpriteAlpha5[((y*256)+x)+((256*192)*zoomIn)] != 0) {
				color = charSpriteMem5[((y*256)+x)+((256*192)*zoomIn)];
				if (blendAlpha > 0) {
					color = alphablend(fg, charSpriteMem5[((y*256)+x)+((256*192)*zoomIn)], blendAlpha);
				}
				if (charSpriteAlpha5[((y*256)+x)+((256*192)*zoomIn)] == 255) {
					bmpImageBuffer[0][(y2*256)+x2] = color;
				} else {
					bmpImageBuffer[0][(y2*256)+x2] = alphablend(color, bmpImageBuffer[1][(y2*256)+x2], charSpriteAlpha5[((y*256)+x)+((256*192)*zoomIn)]);
				}
			}
			flipH[4] ? x2-- : x2++;
		  }
		  y2++;
		}
		buffer--;
	}

	dmaCopyWordsAsynch(0, bmpImageBuffer[buffer], bgGetGfxPtr(bg3Sub), 0x18000);
	chracterSpriteLoaded = true;
}

void GFX::showBgSprite(int zoomIn) {
	//if (!bgSpriteLoaded) return;
}

void GFX::animateBgSprite(int zoomIn, bool* flipH) {
	if (!animateBg) return;

	// Animate background
	bgAnimationTime++;
	if (bgAnimationTime >= bgAnimationDelay) {
		bgAnimationCurrent++;
		if (bgAnimation[bgAnimationCurrent] == 100) {
			// Reset animation
			bgAnimationCurrent = 0;
		}
		bgAnimationFrame = bgAnimation[bgAnimationCurrent];
		loadCharSpriteMem(zoomIn, flipH);
		bgAnimationTime = 0;
	}
}

void GFX::showCharSprite(int num, bool flipH, int zoomIn, int fadeAlpha, bool lightingEffects) {
}

void GFX::DrawSprite(int img, int x, int y, float ScaleX, float ScaleY) {
}

void GFX::DrawSpriteLinear(int img, int x, int y, float ScaleX, float ScaleY) {
}

void GFX::DrawSpriteBlend(int img, float x, float y, u32 color, float ScaleX, float ScaleY) {
}

void GFX::drawCursor(int cX, int cY) {
}