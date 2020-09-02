#include "common.hpp"
#include "color.h"
#include "lodepng.h"
#include "tonccpy.h"
#include "sound.h"

#include "photo_bg.h"

#include <ctime>
#include <unistd.h>

#define charSpriteSize 0x18000

u16 bmpImageBuffer[2][256*192];
u16 bmpImageBuffer2[2][256*192];
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
static u16* charSpriteMem_2[2] = {(u16*)0x026F0000, (u16*)0x02738000};
static u16* charSpriteMem3_2 = (u16*)0x02780000;
static u16* charSpriteMem4_2 = (u16*)0x027C8000;
static u16* charSpriteMem5_2 = (u16*)0x02810000;
static u16* bgSpriteMem2 = (u16*)0x02980000;
static u16* bgSpriteMemExt2[3] = {(u16*)0x029C8000, (u16*)0x02A10000, (u16*)0x02A58000};

static bool chracterSpriteLoaded = false;
static bool chracterSpriteFound[5] = {false};
static bool bgSpriteLoaded = false;

static bool titleBottomLoaded = false;
static bool animateTitle = true;

extern int studioBg;
extern u8 settingBits;
extern int iFps;

extern int bg2Main;
extern int bg3Main;
extern int bg3Sub;

extern bool showCursor;
extern int cursorAlpha;

bool displayChars = true;
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

//extern int characterLimit;

void GFX::loadSheets() {
	/*if (!isDSiMode()) {
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
	}*/

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
	bool alternatePixel = false;
	for(unsigned i=0;i<image.size()/4;i++) {
		if (i >= 256*72 && i < 256*131) {
			metalXpos++;
			if (metalXpos == 256) {
				metalXpos = 0;
				metalYpos++;
			}
		}
		charSpriteAlpha[0][i] = image[(i*4)+3];
		image[(i*4)+3] = 0;
		if (alternatePixel) {
			if (image[(i*4)] >= 0x4) {
				image[(i*4)] -= 0x4;
				image[(i*4)+3] |= BIT(0);
			}
			if (image[(i*4)+1] >= 0x4) {
				image[(i*4)+1] -= 0x4;
				image[(i*4)+3] |= BIT(1);
			}
			if (image[(i*4)+2] >= 0x4) {
				image[(i*4)+2] -= 0x4;
				image[(i*4)+3] |= BIT(2);
			}
		}
		bmpImageBuffer[1][i] = image[i*4]>>3 | (image[(i*4)+1]>>3)<<5 | (image[(i*4)+2]>>3)<<10 | BIT(15);
		if (charSpriteAlpha[0][i] == 255) {
			bmpImageBuffer[0][i] = bmpImageBuffer[1][i];
		} else if (charSpriteAlpha[0][i] == 0) {
			bmpImageBuffer[0][i] = charSpriteMem[0][(metalYpos*384)+metalXpos];
		} else if (i >= 256*72 && i < 256*131) {
			bmpImageBuffer[0][i] = alphablend(bmpImageBuffer[1][i], charSpriteMem[0][(metalYpos*384)+metalXpos], charSpriteAlpha[0][i]);
		}
		if (alternatePixel) {
			if (image[(i*4)+3] & BIT(0)) {
				image[(i*4)] += 0x4;
			}
			if (image[(i*4)+3] & BIT(1)) {
				image[(i*4)+1] += 0x4;
			}
			if (image[(i*4)+3] & BIT(2)) {
				image[(i*4)+2] += 0x4;
			}
		} else {
			if (image[(i*4)] >= 0x4) {
				image[(i*4)] -= 0x4;
			}
			if (image[(i*4)+1] >= 0x4) {
				image[(i*4)+1] -= 0x4;
			}
			if (image[(i*4)+2] >= 0x4) {
				image[(i*4)+2] -= 0x4;
			}
		}
		bmpImageBuffer2[1][i] = image[i*4]>>3 | (image[(i*4)+1]>>3)<<5 | (image[(i*4)+2]>>3)<<10 | BIT(15);
		if (charSpriteAlpha[0][i] == 255) {
			bmpImageBuffer2[0][i] = bmpImageBuffer2[1][i];
		} else if (charSpriteAlpha[0][i] == 0) {
			bmpImageBuffer2[0][i] = charSpriteMem[0][(metalYpos*384)+metalXpos];
		} else if (i >= 256*72 && i < 256*131) {
			bmpImageBuffer2[0][i] = alphablend(bmpImageBuffer2[1][i], charSpriteMem[0][(metalYpos*384)+metalXpos], charSpriteAlpha[0][i]);
		}
		if ((i % 256) == 255) alternatePixel = !alternatePixel;
		alternatePixel = !alternatePixel;
	}

	dmaCopyHalfWords(0, bmpImageBuffer[0], bgSpriteMem, 0x18000);
	dmaCopyHalfWords(1, bmpImageBuffer2[0], bgSpriteMem2, 0x18000);
	dmaCopyHalfWords(0, bmpImageBuffer[0], bgSpriteMem+(charSpriteSize/2), 0x18000);
	dmaCopyHalfWords(1, bmpImageBuffer2[0], bgSpriteMem2+(charSpriteSize/2), 0x18000);
	dmaCopyHalfWords(0, bmpImageBuffer[0], bgSpriteMem+((charSpriteSize/2)*2), 0x18000);
	dmaCopyHalfWords(1, bmpImageBuffer2[0], bgSpriteMem2+((charSpriteSize/2)*2), 0x18000);
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
			bmpImageBuffer2[0][i] = charSpriteMem[0][(metalYpos*384)+metalXpos];
		  } else if (i >= 256*72 && i < 256*131) {
			bmpImageBuffer[0][i] = alphablend(bmpImageBuffer[1][i], charSpriteMem[0][(metalYpos*384)+metalXpos], charSpriteAlpha[0][i]);
			bmpImageBuffer2[0][i] = alphablend(bmpImageBuffer2[1][i], charSpriteMem[0][(metalYpos*384)+metalXpos], charSpriteAlpha[0][i]);
		  }
		}
	}

	dmaCopyHalfWordsAsynch(0, bmpImageBuffer[0], bgGetGfxPtr(bg2Main), 0x18000);
	dmaCopyHalfWordsAsynch(1, bmpImageBuffer2[0], bgGetGfxPtr(bg3Main), 0x18000);
	if (!titleBottomLoaded) {
		tonccpy(bgGetGfxPtr(bg3Sub), photo_bgBitmap, photo_bgBitmapLen);
		for (int i = 0; i < 256*192; i+=2) {
			bgGetGfxPtr(bg3Sub)[i/2] += 0x1010;	// Shift pallete 16 colors further
		}
		tonccpy(BG_PALETTE_SUB + 0x10, photo_bgPal, 240*sizeof(u16));
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

	swiWaitForVBlank();	// Prevent screen tearing
	dmaFillHalfWords(0xFFFF, bgGetGfxPtr(bg2Main), 0x18000);
	dmaFillHalfWords(0xFFFF, bgGetGfxPtr(bg3Main), 0x18000);

	timeOutside = 2;	// Default is Nighttime
	int aniFrames = 0;

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
				aniFrames = 2;
			} else if (isEvening(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/Sunset_tropicaBeach_0.png";
				aniFrames = 2;
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
			aniFrames = 3;
			break;
		case 52:
			if (isDaytime(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/Day_outdoorStage.png";
			} else if (isEvening(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/Sunset_outdoorStage.png";
			} else {
				bgPath = "nitro:/graphics/bg/Night_outdoorStage.png";
			}
			break;
		case 53:
			if (isDaytime(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/Day_park4.png";
			} else if (isEvening(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/Sunset_park4.png";
			} else {
				bgPath = "nitro:/graphics/bg/Night_park4.png";
			}
			break;
		case 54:
			if (isDaytime(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/Day_cafe4.png";
			} else if (isEvening(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/Sunset_cafe4.png";
			} else {
				bgPath = "nitro:/graphics/bg/Night_cafe4.png";
			}
			break;
		case 55:
			if (isDaytime(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/Day_seaside.png";
			} else if (isEvening(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/Sunset_seaside.png";
			} else {
				bgPath = "nitro:/graphics/bg/Night_seaside.png";
			}
			break;
		case 56:
			if (isDaytime(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/Day_countrysideSpring.png";
			} else if (isEvening(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/Sunset_countrysideSpring.png";
			} else {
				bgPath = "nitro:/graphics/bg/Night_countrysideSpring.png";
			}
			break;
		case 57:
			if (isDaytime(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/Day_countrysideSummer.png";
			} else if (isEvening(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/Sunset_countrysideSummer.png";
			} else {
				bgPath = "nitro:/graphics/bg/Night_countrysideSummer.png";
			}
			break;
		case 58:
			if (isDaytime(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/Day_countrysideFall.png";
			} else if (isEvening(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/Sunset_countrysideFall.png";
			} else {
				bgPath = "nitro:/graphics/bg/Night_countrysideFall.png";
			}
			break;
		case 59:
			if (isDaytime(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/Day_countrysideWinter.png";
			} else if (isEvening(hour, minutes)) {
				bgPath = "nitro:/graphics/bg/Sunset_countrysideWinter.png";
			} else {
				bgPath = "nitro:/graphics/bg/Night_countrysideWinter.png";
			}
			break;
		case 60:
			bgPath = "nitro:/graphics/bg/lifestyleShop4.png";
			break;
		case 61:
			bgPath = "nitro:/graphics/bg/florist.png";
			break;
		case 62:
			bgPath = "nitro:/graphics/bg/cakeShop4.png";
			break;
		case 63:
			bgPath = "nitro:/graphics/bg/livelyStage_0.png";
			aniFrames = 3;
			break;
		case 64:
			bgPath = "nitro:/graphics/bg/girlyStage_0.png";
			aniFrames = 1;
			break;
		case 65:
			bgPath = "nitro:/graphics/bg/concertHall.png";
			break;
		case 66:
			bgPath = "nitro:/graphics/bg/charityStage.png";
			break;
		case 67:
			bgPath = "nitro:/graphics/bg/RocketRobz.png";
			break;
		case 68:
			bgPath = "nitro:/graphics/bg/Nintendo.png";
			break;
		case 69:
			bgPath = "nitro:/graphics/bg/segaGen.png";
			break;
		case 70:
			bgPath = "nitro:/graphics/bg/twlMenu.png";
			break;
		case 71:
			bgPath = "nitro:/graphics/bg/hiyaCFW.png";
			break;
	}
	std::vector<unsigned char> image;
	unsigned width, height;
	lodepng::decode(image, width, height, bgPath);
	bool alternatePixel = false;
	for(unsigned i=0;i<image.size()/4;i++) {
		image[(i*4)+3] = 0;
		if (alternatePixel) {
			if (image[(i*4)] >= 0x4) {
				image[(i*4)] -= 0x4;
				image[(i*4)+3] |= BIT(0);
			}
			if (image[(i*4)+1] >= 0x4) {
				image[(i*4)+1] -= 0x4;
				image[(i*4)+3] |= BIT(1);
			}
			if (image[(i*4)+2] >= 0x4) {
				image[(i*4)+2] -= 0x4;
				image[(i*4)+3] |= BIT(2);
			}
		}
		bgSpriteMem[i] = image[i*4]>>3 | (image[(i*4)+1]>>3)<<5 | (image[(i*4)+2]>>3)<<10 | BIT(15);
		if (alternatePixel) {
			if (image[(i*4)+3] & BIT(0)) {
				image[(i*4)] += 0x4;
			}
			if (image[(i*4)+3] & BIT(1)) {
				image[(i*4)+1] += 0x4;
			}
			if (image[(i*4)+3] & BIT(2)) {
				image[(i*4)+2] += 0x4;
			}
		} else {
			if (image[(i*4)] >= 0x4) {
				image[(i*4)] -= 0x4;
			}
			if (image[(i*4)+1] >= 0x4) {
				image[(i*4)+1] -= 0x4;
			}
			if (image[(i*4)+2] >= 0x4) {
				image[(i*4)+2] -= 0x4;
			}
		}
		bgSpriteMem2[i] = image[i*4]>>3 | (image[(i*4)+1]>>3)<<5 | (image[(i*4)+2]>>3)<<10 | BIT(15);
		if ((i % 256) == 255) alternatePixel = !alternatePixel;
		alternatePixel = !alternatePixel;
	}

	bgSpriteLoaded = true;
	bgAnimationFrame = 0;
	bgAnimationCurrent = 0;
	bgAnimationTime = 0;
	animateBg = false;

	// Load animated parts
	if (aniFrames > 0) {
		switch (studioBg) {
			case 12:
				if (timeOutside == 0) {
					bgPath = "nitro:/graphics/bg/Day_tropicaBeach_%i.png";
				} else {
					bgPath = "nitro:/graphics/bg/Sunset_tropicaBeach_%i.png";
				}
				break;
			case 51:
				bgPath = "nitro:/graphics/bg/liveMusicClub3_%i.png";
				break;
			case 63:
				bgPath = "nitro:/graphics/bg/livelyStage_%i.png";
				break;
			case 64:
				bgPath = "nitro:/graphics/bg/girlyStage_%i.png";
				break;
		}
		char bgAniPath[64];
		for (int i = 1; i <= aniFrames; i++) {
			snd().updateStream();
			sprintf(bgAniPath, bgPath, i);
			image.clear();
			lodepng::decode(image, width, height, bgAniPath);
			for(unsigned p=0;p<image.size()/4;p++) {
				image[(p*4)+3] = 0;
				if (alternatePixel) {
					if (image[(p*4)] >= 0x4) {
						image[(p*4)] -= 0x4;
						image[(p*4)+3] |= BIT(0);
					}
					if (image[(p*4)+1] >= 0x4) {
						image[(p*4)+1] -= 0x4;
						image[(p*4)+3] |= BIT(1);
					}
					if (image[(p*4)+2] >= 0x4) {
						image[(p*4)+2] -= 0x4;
						image[(p*4)+3] |= BIT(2);
					}
				}
				bgSpriteMemExt[i-1][p] = image[p*4]>>3 | (image[(p*4)+1]>>3)<<5 | (image[(p*4)+2]>>3)<<10 | BIT(15);
				if (alternatePixel) {
					if (image[(p*4)+3] & BIT(0)) {
						image[(p*4)] += 0x4;
					}
					if (image[(p*4)+3] & BIT(1)) {
						image[(p*4)+1] += 0x4;
					}
					if (image[(p*4)+3] & BIT(2)) {
						image[(p*4)+2] += 0x4;
					}
				} else {
					if (image[(p*4)] >= 0x4) {
						image[(p*4)] -= 0x4;
					}
					if (image[(p*4)+1] >= 0x4) {
						image[(p*4)+1] -= 0x4;
					}
					if (image[(p*4)+2] >= 0x4) {
						image[(p*4)+2] -= 0x4;
					}
				}
				bgSpriteMemExt2[i-1][p] = image[p*4]>>3 | (image[(p*4)+1]>>3)<<5 | (image[(p*4)+2]>>3)<<10 | BIT(15);
				if ((p % 256) == 255) alternatePixel = !alternatePixel;
				alternatePixel = !alternatePixel;
			}
		}
		if (studioBg == 64) {
			bgAnimationDelay = iFps/2;
			bgAnimation[0] = 0;
			bgAnimation[1] = 1;
			bgAnimation[2] = 100;
		} else if (studioBg == 51 || studioBg == 63) {
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
	dmaCopyHalfWords(0, bgSpriteMem, bmpImageBuffer[0], 0x18000);
	dmaCopyHalfWords(1, bgSpriteMem2, bmpImageBuffer2[0], 0x18000);
	swiWaitForVBlank();	// Prevent screen tearing
	dmaCopyHalfWordsAsynch(0, bmpImageBuffer[0], bgGetGfxPtr(bg2Main), 0x18000);
	dmaCopyHalfWordsAsynch(1, bmpImageBuffer2[0], bgGetGfxPtr(bg3Main), 0x18000);
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
	bool alternatePixel = false;
	if (num == 4) {
		for(unsigned i=0;i<image.size()/4;i++) {
			//tonccpy(&charSpriteAlpha5[i], &image[(i*4)+3], 1);
			charSpriteAlpha5[i] = image[(i*4)+3];
			image[(i*4)+3] = 0;
			if (alternatePixel) {
				if (image[(i*4)] >= 0x4) {
					image[(i*4)] -= 0x4;
					image[(i*4)+3] |= BIT(0);
				}
				if (image[(i*4)+1] >= 0x4) {
					image[(i*4)+1] -= 0x4;
					image[(i*4)+3] |= BIT(1);
				}
				if (image[(i*4)+2] >= 0x4) {
					image[(i*4)+2] -= 0x4;
					image[(i*4)+3] |= BIT(2);
				}
			}
			charSpriteMem5[i] = image[i*4]>>3 | (image[(i*4)+1]>>3)<<5 | (image[(i*4)+2]>>3)<<10 | BIT(15);
			if (alternatePixel) {
				if (image[(i*4)+3] & BIT(0)) {
					image[(i*4)] += 0x4;
				}
				if (image[(i*4)+3] & BIT(1)) {
					image[(i*4)+1] += 0x4;
				}
				if (image[(i*4)+3] & BIT(2)) {
					image[(i*4)+2] += 0x4;
				}
			} else {
				if (image[(i*4)] >= 0x4) {
					image[(i*4)] -= 0x4;
				}
				if (image[(i*4)+1] >= 0x4) {
					image[(i*4)+1] -= 0x4;
				}
				if (image[(i*4)+2] >= 0x4) {
					image[(i*4)+2] -= 0x4;
				}
			}
			charSpriteMem5_2[i] = image[i*4]>>3 | (image[(i*4)+1]>>3)<<5 | (image[(i*4)+2]>>3)<<10 | BIT(15);
			if ((i % 256) == 255) alternatePixel = !alternatePixel;
			alternatePixel = !alternatePixel;
		}
	} else if (num == 3) {
		for(unsigned i=0;i<image.size()/4;i++) {
			//tonccpy(&charSpriteAlpha4[i], &image[(i*4)+3], 1);
			charSpriteAlpha4[i] = image[(i*4)+3];
			image[(i*4)+3] = 0;
			if (alternatePixel) {
				if (image[(i*4)] >= 0x4) {
					image[(i*4)] -= 0x4;
					image[(i*4)+3] |= BIT(0);
				}
				if (image[(i*4)+1] >= 0x4) {
					image[(i*4)+1] -= 0x4;
					image[(i*4)+3] |= BIT(1);
				}
				if (image[(i*4)+2] >= 0x4) {
					image[(i*4)+2] -= 0x4;
					image[(i*4)+3] |= BIT(2);
				}
			}
			charSpriteMem4[i] = image[i*4]>>3 | (image[(i*4)+1]>>3)<<5 | (image[(i*4)+2]>>3)<<10 | BIT(15);
			if (alternatePixel) {
				if (image[(i*4)+3] & BIT(0)) {
					image[(i*4)] += 0x4;
				}
				if (image[(i*4)+3] & BIT(1)) {
					image[(i*4)+1] += 0x4;
				}
				if (image[(i*4)+3] & BIT(2)) {
					image[(i*4)+2] += 0x4;
				}
			} else {
				if (image[(i*4)] >= 0x4) {
					image[(i*4)] -= 0x4;
				}
				if (image[(i*4)+1] >= 0x4) {
					image[(i*4)+1] -= 0x4;
				}
				if (image[(i*4)+2] >= 0x4) {
					image[(i*4)+2] -= 0x4;
				}
			}
			charSpriteMem4_2[i] = image[i*4]>>3 | (image[(i*4)+1]>>3)<<5 | (image[(i*4)+2]>>3)<<10 | BIT(15);
			if ((i % 256) == 255) alternatePixel = !alternatePixel;
			alternatePixel = !alternatePixel;
		}
	} else if (num == 2) {
		for(unsigned i=0;i<image.size()/4;i++) {
			//tonccpy(&charSpriteAlpha3[i], &image[(i*4)+3], 1);
			charSpriteAlpha3[i] = image[(i*4)+3];
			image[(i*4)+3] = 0;
			if (alternatePixel) {
				if (image[(i*4)] >= 0x4) {
					image[(i*4)] -= 0x4;
					image[(i*4)+3] |= BIT(0);
				}
				if (image[(i*4)+1] >= 0x4) {
					image[(i*4)+1] -= 0x4;
					image[(i*4)+3] |= BIT(1);
				}
				if (image[(i*4)+2] >= 0x4) {
					image[(i*4)+2] -= 0x4;
					image[(i*4)+3] |= BIT(2);
				}
			}
			charSpriteMem3[i] = image[i*4]>>3 | (image[(i*4)+1]>>3)<<5 | (image[(i*4)+2]>>3)<<10 | BIT(15);
			if (alternatePixel) {
				if (image[(i*4)+3] & BIT(0)) {
					image[(i*4)] += 0x4;
				}
				if (image[(i*4)+3] & BIT(1)) {
					image[(i*4)+1] += 0x4;
				}
				if (image[(i*4)+3] & BIT(2)) {
					image[(i*4)+2] += 0x4;
				}
			} else {
				if (image[(i*4)] >= 0x4) {
					image[(i*4)] -= 0x4;
				}
				if (image[(i*4)+1] >= 0x4) {
					image[(i*4)+1] -= 0x4;
				}
				if (image[(i*4)+2] >= 0x4) {
					image[(i*4)+2] -= 0x4;
				}
			}
			charSpriteMem3_2[i] = image[i*4]>>3 | (image[(i*4)+1]>>3)<<5 | (image[(i*4)+2]>>3)<<10 | BIT(15);
			if ((i % 256) == 255) alternatePixel = !alternatePixel;
			alternatePixel = !alternatePixel;
		}
	} else {
		for(unsigned i=0;i<image.size()/4;i++) {
			charSpriteAlpha[num][i] = image[(i*4)+3];
			image[(i*4)+3] = 0;
			if (alternatePixel) {
				if (image[(i*4)] >= 0x4) {
					image[(i*4)] -= 0x4;
					image[(i*4)+3] |= BIT(0);
				}
				if (image[(i*4)+1] >= 0x4) {
					image[(i*4)+1] -= 0x4;
					image[(i*4)+3] |= BIT(1);
				}
				if (image[(i*4)+2] >= 0x4) {
					image[(i*4)+2] -= 0x4;
					image[(i*4)+3] |= BIT(2);
				}
			}
			charSpriteMem[num][i] = image[i*4]>>3 | (image[(i*4)+1]>>3)<<5 | (image[(i*4)+2]>>3)<<10 | BIT(15);
			if (alternatePixel) {
				if (image[(i*4)+3] & BIT(0)) {
					image[(i*4)] += 0x4;
				}
				if (image[(i*4)+3] & BIT(1)) {
					image[(i*4)+1] += 0x4;
				}
				if (image[(i*4)+3] & BIT(2)) {
					image[(i*4)+2] += 0x4;
				}
			} else {
				if (image[(i*4)] >= 0x4) {
					image[(i*4)] -= 0x4;
				}
				if (image[(i*4)+1] >= 0x4) {
					image[(i*4)+1] -= 0x4;
				}
				if (image[(i*4)+2] >= 0x4) {
					image[(i*4)+2] -= 0x4;
				}
			}
			charSpriteMem_2[num][i] = image[i*4]>>3 | (image[(i*4)+1]>>3)<<5 | (image[(i*4)+2]>>3)<<10 | BIT(15);
			if ((i % 256) == 255) alternatePixel = !alternatePixel;
			alternatePixel = !alternatePixel;
		}
	}

	chracterSpriteFound[num] = true;

	return true;
}

ITCM_CODE void GFX::loadCharSpriteMem(int zoomIn, bool* flipH) {
	u16* bgLoc = 0;
	u16* bgLoc2 = 0;
	if (bgAnimationFrame == 0) {
		bgLoc = bgSpriteMem;
		bgLoc2 = bgSpriteMem2;
	} else {
		bgLoc = bgSpriteMemExt[bgAnimationFrame-1];
		bgLoc2 = bgSpriteMemExt2[bgAnimationFrame-1];
	}

	dmaCopyHalfWords(0, bgLoc+((256*192)*zoomIn), bmpImageBuffer[0], 0x18000);
	dmaCopyHalfWords(1, bgLoc2+((256*192)*zoomIn), bmpImageBuffer2[0], 0x18000);
	if (!displayChars || !chracterSpriteFound[0]) {
		swiWaitForVBlank();	// Prevent screen tearing
		dmaCopyHalfWordsAsynch(0, bmpImageBuffer[0], bgGetGfxPtr(bg2Main), 0x18000);
		dmaCopyHalfWordsAsynch(1, bmpImageBuffer2[0], bgGetGfxPtr(bg3Main), 0x18000);
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
		case 52:
		case 53:
		case 55:
		case 56:
		case 57:
		case 58:
		case 59:
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
		case 54:
			// Tint for Cafe 4
			if (timeOutside > 0) {
				blendAlpha = 32;
			}
			if (timeOutside == 1) {
				fg = RGB15(95/8, 47/8, 0);	// Tint for Sunset
			} else if (timeOutside == 2) {
				fg = RGB15(0, 0, 63/8);	// Tint for Nighttime
			}
			break;
	}

	int buffer = 0;
	int x2 = 0;
	u16 color = 0;
	u16 color2 = 0;
	if (chracterSpriteFound[2]) {
		// Character 1
		for (int y = 0; y < 192; y++) {
		  x2 = flipH[0] ? 255 : 0;
		  x2 -= (zoomIn==1 ? 96 : 50);
		  for (int x = 0; x < 256; x++) {
			if (x2 >= 0 && x2 < 256 && charSpriteAlpha[0][((y*256)+x)+((256*192)*zoomIn)] != 0) {
				color = charSpriteMem[0][((y*256)+x)+((256*192)*zoomIn)];
				color2 = charSpriteMem_2[0][((y*256)+x)+((256*192)*zoomIn)];
				if (blendAlpha > 0) {
					color = alphablend(fg, charSpriteMem[0][((y*256)+x)+((256*192)*zoomIn)], blendAlpha);
					color2 = alphablend(fg, charSpriteMem_2[0][((y*256)+x)+((256*192)*zoomIn)], blendAlpha);
				}
				if (charSpriteAlpha[0][((y*256)+x)+((256*192)*zoomIn)] == 255) {
					bmpImageBuffer[0][(y*256)+x2] = color;
					bmpImageBuffer2[0][(y*256)+x2] = color2;
				} else {
					bmpImageBuffer[0][(y*256)+x2] = alphablend(color, bgLoc[((y*256)+x2)+((256*192)*zoomIn)], charSpriteAlpha[0][((y*256)+x)+((256*192)*zoomIn)]);
					bmpImageBuffer2[0][(y*256)+x2] = alphablend(color2, bgLoc2[((y*256)+x2)+((256*192)*zoomIn)], charSpriteAlpha[0][((y*256)+x)+((256*192)*zoomIn)]);
				}
			}
			flipH[0] ? x2-- : x2++;
		  }
		}
		// Character 2
		dmaCopyHalfWordsAsynch(0, bmpImageBuffer[0], bmpImageBuffer[1], 0x18000);
		dmaCopyHalfWords(1, bmpImageBuffer2[0], bmpImageBuffer2[1], 0x18000);
		for (int y = 0; y < 192; y++) {
		  x2 = flipH[1] ? 255 : 0;
		  for (int x = 0; x < 256; x++) {
			if (charSpriteAlpha[1][((y*256)+x)+((256*192)*zoomIn)] != 0) {
				color = charSpriteMem[1][((y*256)+x)+((256*192)*zoomIn)];
				color2 = charSpriteMem_2[1][((y*256)+x)+((256*192)*zoomIn)];
				if (blendAlpha > 0) {
					color = alphablend(fg, charSpriteMem[1][((y*256)+x)+((256*192)*zoomIn)], blendAlpha);
					color2 = alphablend(fg, charSpriteMem_2[1][((y*256)+x)+((256*192)*zoomIn)], blendAlpha);
				}
				if (charSpriteAlpha[1][((y*256)+x)+((256*192)*zoomIn)] == 255) {
					bmpImageBuffer[1][(y*256)+x2] = color;
					bmpImageBuffer2[1][(y*256)+x2] = color2;
				} else {
					bmpImageBuffer[1][(y*256)+x2] = alphablend(color, bmpImageBuffer[0][(y*256)+x2], charSpriteAlpha[1][((y*256)+x)+((256*192)*zoomIn)]);
					bmpImageBuffer2[1][(y*256)+x2] = alphablend(color2, bmpImageBuffer2[0][(y*256)+x2], charSpriteAlpha[1][((y*256)+x)+((256*192)*zoomIn)]);
				}
			}
			flipH[1] ? x2-- : x2++;
		  }
		}
		// Character 3
		dmaCopyHalfWordsAsynch(0, bmpImageBuffer[1], bmpImageBuffer[0], 0x18000);
		dmaCopyHalfWords(1, bmpImageBuffer2[1], bmpImageBuffer2[0], 0x18000);
		for (int y = 0; y < 192; y++) {
		  x2 = flipH[2] ? 255 : 0;
		  x2 += (zoomIn==1 ? 96 : 50);
		  for (int x = 0; x < 256; x++) {
			if (x2 >= 0 && x2 < 256 && charSpriteAlpha3[((y*256)+x)+((256*192)*zoomIn)] != 0) {
				color = charSpriteMem3[((y*256)+x)+((256*192)*zoomIn)];
				color2 = charSpriteMem3_2[((y*256)+x)+((256*192)*zoomIn)];
				if (blendAlpha > 0) {
					color = alphablend(fg, charSpriteMem3[((y*256)+x)+((256*192)*zoomIn)], blendAlpha);
					color2 = alphablend(fg, charSpriteMem3_2[((y*256)+x)+((256*192)*zoomIn)], blendAlpha);
				}
				if (charSpriteAlpha3[((y*256)+x)+((256*192)*zoomIn)] == 255) {
					bmpImageBuffer[0][(y*256)+x2] = color;
					bmpImageBuffer2[0][(y*256)+x2] = color2;
				} else {
					bmpImageBuffer[0][(y*256)+x2] = alphablend(color, bmpImageBuffer[1][(y*256)+x2], charSpriteAlpha3[((y*256)+x)+((256*192)*zoomIn)]);
					bmpImageBuffer2[0][(y*256)+x2] = alphablend(color2, bmpImageBuffer2[1][(y*256)+x2], charSpriteAlpha3[((y*256)+x)+((256*192)*zoomIn)]);
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
				color2 = charSpriteMem_2[0][((y*256)+x)+((256*192)*zoomIn)];
				if (blendAlpha > 0) {
					color = alphablend(fg, charSpriteMem[0][((y*256)+x)+((256*192)*zoomIn)], blendAlpha);
					color2 = alphablend(fg, charSpriteMem_2[0][((y*256)+x)+((256*192)*zoomIn)], blendAlpha);
				}
				if (charSpriteAlpha[0][((y*256)+x)+((256*192)*zoomIn)] == 255) {
					bmpImageBuffer[0][(y*256)+x2] = color;
					bmpImageBuffer2[0][(y*256)+x2] = color2;
				} else {
					bmpImageBuffer[0][(y*256)+x2] = alphablend(color, bgLoc[((y*256)+x2)+((256*192)*zoomIn)], charSpriteAlpha[0][((y*256)+x)+((256*192)*zoomIn)]);
					bmpImageBuffer2[0][(y*256)+x2] = alphablend(color2, bgLoc2[((y*256)+x2)+((256*192)*zoomIn)], charSpriteAlpha[0][((y*256)+x)+((256*192)*zoomIn)]);
				}
			}
			flipH[0] ? x2-- : x2++;
		  }
		}
		// Character 2
		dmaCopyHalfWordsAsynch(0, bmpImageBuffer[0], bmpImageBuffer[1], 0x18000);
		dmaCopyHalfWords(1, bmpImageBuffer2[0], bmpImageBuffer2[1], 0x18000);
		for (int y = 0; y < 192; y++) {
		  x2 = flipH[1] ? 255 : 0;
		  x2 += (zoomIn==1 ? 64 : 26);
		  for (int x = 0; x < 256; x++) {
			if (x2 >= 0 && x2 < 256 && charSpriteAlpha[1][((y*256)+x)+((256*192)*zoomIn)] != 0) {
				color = charSpriteMem[1][((y*256)+x)+((256*192)*zoomIn)];
				color2 = charSpriteMem_2[1][((y*256)+x)+((256*192)*zoomIn)];
				if (blendAlpha > 0) {
					color = alphablend(fg, charSpriteMem[1][((y*256)+x)+((256*192)*zoomIn)], blendAlpha);
					color2 = alphablend(fg, charSpriteMem_2[1][((y*256)+x)+((256*192)*zoomIn)], blendAlpha);
				}
				if (charSpriteAlpha[1][((y*256)+x)+((256*192)*zoomIn)] == 255) {
					bmpImageBuffer[1][(y*256)+x2] = color;
					bmpImageBuffer2[1][(y*256)+x2] = color2;
				} else {
					bmpImageBuffer[1][(y*256)+x2] = alphablend(color, bmpImageBuffer[0][(y*256)+x2], charSpriteAlpha[1][((y*256)+x)+((256*192)*zoomIn)]);
					bmpImageBuffer2[1][(y*256)+x2] = alphablend(color2, bmpImageBuffer2[0][(y*256)+x2], charSpriteAlpha[1][((y*256)+x)+((256*192)*zoomIn)]);
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
			color2 = charSpriteMem_2[0][((y*256)+x)+((256*192)*zoomIn)];
			if (blendAlpha > 0) {
				color = alphablend(fg, charSpriteMem[0][((y*256)+x)+((256*192)*zoomIn)], blendAlpha);
				color2 = alphablend(fg, charSpriteMem_2[0][((y*256)+x)+((256*192)*zoomIn)], blendAlpha);
			}
			if (charSpriteAlpha[0][((y*256)+x)+((256*192)*zoomIn)] == 255) {
				bmpImageBuffer[0][(y*256)+x2] = color;
				bmpImageBuffer2[0][(y*256)+x2] = color2;
			} else {
				bmpImageBuffer[0][(y*256)+x2] = alphablend(color, bgLoc[((y*256)+x2)+((256*192)*zoomIn)], charSpriteAlpha[0][((y*256)+x)+((256*192)*zoomIn)]);
				bmpImageBuffer2[0][(y*256)+x2] = alphablend(color2, bgLoc2[((y*256)+x2)+((256*192)*zoomIn)], charSpriteAlpha[0][((y*256)+x)+((256*192)*zoomIn)]);
			}
		}
		flipH[0] ? x2-- : x2++;
	  }
	}

	if (chracterSpriteFound[3]) {
		// Continued from "if (chracterSpriteFound[2])"
		// Character 4
		dmaCopyHalfWordsAsynch(0, bmpImageBuffer[0], bmpImageBuffer[1], 0x18000);
		dmaCopyHalfWords(1, bmpImageBuffer2[0], bmpImageBuffer2[1], 0x18000);
		int y2 = 72;
		for (int y = 0; y < 120; y++) {
		  x2 = flipH[3] ? 255 : 0;
		  x2 -= (zoomIn==1 ? 64 : 26);
		  for (int x = 0; x < 256; x++) {
			if (x2 >= 0 && x2 < 256 && charSpriteAlpha4[((y*256)+x)+((256*192)*zoomIn)] != 0) {
				color = charSpriteMem4[((y*256)+x)+((256*192)*zoomIn)];
				color2 = charSpriteMem4_2[((y*256)+x)+((256*192)*zoomIn)];
				if (blendAlpha > 0) {
					color = alphablend(fg, charSpriteMem4[((y*256)+x)+((256*192)*zoomIn)], blendAlpha);
					color2 = alphablend(fg, charSpriteMem4_2[((y*256)+x)+((256*192)*zoomIn)], blendAlpha);
				}
				if (charSpriteAlpha4[((y*256)+x)+((256*192)*zoomIn)] == 255) {
					bmpImageBuffer[1][(y2*256)+x2] = color;
					bmpImageBuffer2[1][(y2*256)+x2] = color2;
				} else {
					bmpImageBuffer[1][(y2*256)+x2] = alphablend(color, bmpImageBuffer[0][(y2*256)+x2], charSpriteAlpha4[((y*256)+x)+((256*192)*zoomIn)]);
					bmpImageBuffer2[1][(y2*256)+x2] = alphablend(color2, bmpImageBuffer2[0][(y2*256)+x2], charSpriteAlpha4[((y*256)+x)+((256*192)*zoomIn)]);
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
		dmaCopyHalfWordsAsynch(0, bmpImageBuffer[1], bmpImageBuffer[0], 0x18000);
		dmaCopyHalfWords(1, bmpImageBuffer2[1], bmpImageBuffer2[0], 0x18000);
		int y2 = 72;
		for (int y = 0; y < 120; y++) {
		  x2 = flipH[4] ? 255 : 0;
		  x2 += (zoomIn==1 ? 64 : 26);
		  for (int x = 0; x < 256; x++) {
			if (x2 >= 0 && x2 < 256 && charSpriteAlpha5[((y*256)+x)+((256*192)*zoomIn)] != 0) {
				color = charSpriteMem5[((y*256)+x)+((256*192)*zoomIn)];
				color2 = charSpriteMem5_2[((y*256)+x)+((256*192)*zoomIn)];
				if (blendAlpha > 0) {
					color = alphablend(fg, charSpriteMem5[((y*256)+x)+((256*192)*zoomIn)], blendAlpha);
					color2 = alphablend(fg, charSpriteMem5_2[((y*256)+x)+((256*192)*zoomIn)], blendAlpha);
				}
				if (charSpriteAlpha5[((y*256)+x)+((256*192)*zoomIn)] == 255) {
					bmpImageBuffer[0][(y2*256)+x2] = color;
					bmpImageBuffer2[0][(y2*256)+x2] = color2;
				} else {
					bmpImageBuffer[0][(y2*256)+x2] = alphablend(color, bmpImageBuffer[1][(y2*256)+x2], charSpriteAlpha5[((y*256)+x)+((256*192)*zoomIn)]);
					bmpImageBuffer2[0][(y2*256)+x2] = alphablend(color2, bmpImageBuffer2[1][(y2*256)+x2], charSpriteAlpha5[((y*256)+x)+((256*192)*zoomIn)]);
				}
			}
			flipH[4] ? x2-- : x2++;
		  }
		  y2++;
		}
		buffer--;
	}

	swiWaitForVBlank();	// Prevent screen tearing
	dmaCopyHalfWordsAsynch(0, bmpImageBuffer[buffer], bgGetGfxPtr(bg2Main), 0x18000);
	dmaCopyHalfWordsAsynch(1, bmpImageBuffer2[buffer], bgGetGfxPtr(bg3Main), 0x18000);
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