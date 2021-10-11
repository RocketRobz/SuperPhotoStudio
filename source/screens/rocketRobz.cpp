#include "rocketRobz.hpp"
#include "screenvars.h"
#ifdef NDS
#include "lodepng.h"
#include "myDSiMode.h"
#endif

static int subMode = 0;
#ifdef _3DS
static int prevSubMode = -1;
#endif
extern int delay;
static int rr_fadeAlpha = 0;
static int rr_fadeType = true;

#ifdef _3DS
static int robzXpos = 135+220;
static int robzYpos = 19+220;
static int rrFlashFade = 255;
static int rrTextFade = 255;
static int rrTextGlowFade = 127;
static int rrBgYpos = 240;

static int floorLineYpos[36] = {
	0, 0, 1, 1, 2, 2, 3, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 33, 36, 39, 42, 45, 50, 54, 58, 62, 66, 70
};
static int floorLineCurrentYpos[3] = {0, 12, 24};
static bool floorLineMoveOdd[3] = {false, false, false};

#define rrTopGradient C2D_Color32(64, 0, 96, 255)
#define rrBottomGradient C2D_Color32(169, 0, 254, 255)
#define wireframeColor C2D_Color32(0, 255, 0, 255)
#endif

void RocketRobz::Draw(void) const {
	fadecolor = 0;	// Always use black color for fading effects

	#ifdef _3DS
	Gui::ScreenDraw(Top);

	bool robzInPos = (robzXpos == 135 && robzYpos == 19);
	if (robzInPos) {
		if (rrBgYpos > 0) {
			Gui::Draw_Rect(0, 0, 400, rrBgYpos, rrTopGradient);
		}
		// Top half gradient
		C2D_DrawRectangle(
	0, rrBgYpos, 0, 400, 120,
	rrTopGradient, rrTopGradient,
	rrBottomGradient, rrBottomGradient);
		// Bottom half gradient
		C2D_DrawRectangle(
	0, rrBgYpos+120, 0, 400, 120,
	rrBottomGradient, rrBottomGradient,
	rrTopGradient, rrTopGradient);

		// Top half gradient
		C2D_DrawRectangle(
	0, rrBgYpos+240, 0, 400, 120,
	rrTopGradient, rrTopGradient,
	rrBottomGradient, rrBottomGradient);
		// Bottom half gradient
		C2D_DrawRectangle(
	0, rrBgYpos+240+120, 0, 400, 120,
	rrBottomGradient, rrBottomGradient,
	rrTopGradient, rrTopGradient);

	} else {
		// Ceiling
		C2D_DrawLine(0, 64, wireframeColor, 400, 64, wireframeColor, 1, 0);
		C2D_DrawLine(200, 0, wireframeColor, 200, 64, wireframeColor, 1, 0);
		int xTop = 0;
		int xBot = 0;
		for (int i = 0; i < 4; i++) {
			xTop += 80;
			xBot += 48;
			C2D_DrawLine(200-xTop, 0, wireframeColor, 200-xBot, 64, wireframeColor, 1, 0);
			C2D_DrawLine(200+xTop, 0, wireframeColor, 200+xBot, 64, wireframeColor, 1, 0);
		}
		for (int i = 0; i < 3; i++) {
			C2D_DrawLine(0, 64-floorLineYpos[floorLineCurrentYpos[i]], wireframeColor, 400, 64-floorLineYpos[floorLineCurrentYpos[i]], wireframeColor, 1, 0);
		}

		// Floor
		C2D_DrawLine(0, 240-64, wireframeColor, 400, 240-64, wireframeColor, 1, 0);
		C2D_DrawLine(200, 240-64, wireframeColor, 200, 240, wireframeColor, 1, 0);
		xTop = 0;
		xBot = 0;
		for (int i = 0; i < 4; i++) {
			xTop += 48;
			xBot += 80;
			C2D_DrawLine(200-xTop, 240-64, wireframeColor, 200-xBot, 240, wireframeColor, 1, 0);
			C2D_DrawLine(200+xTop, 240-64, wireframeColor, 200+xBot, 240, wireframeColor, 1, 0);
		}
		for (int i = 0; i < 3; i++) {
			C2D_DrawLine(0, (240-64)+floorLineYpos[floorLineCurrentYpos[i]], wireframeColor, 400, (240-64)+floorLineYpos[floorLineCurrentYpos[i]], wireframeColor, 1, 0);
		}

		for (int i = 0; i < 3; i++) {
			switch (iFps) {
				default:
					floorLineCurrentYpos[i]++;
					break;
				case 30:
					floorLineCurrentYpos[i]+=2;
					break;
				case 24:
					floorLineCurrentYpos[i] += 2+floorLineMoveOdd[i];
					floorLineMoveOdd[i] = !floorLineMoveOdd[i];
					break;
			}
			if (floorLineCurrentYpos[i] > 36) floorLineCurrentYpos[i] = floorLineMoveOdd[i];
		}

		// Top half gradient
		C2D_DrawRectangle(
	0, 0, 0, 400, 120,
	0, 0,
	rrTopGradient, rrTopGradient);
		// Bottom half gradient
		C2D_DrawRectangle(
	0, 120, 0, 400, 120,
	rrTopGradient, rrTopGradient,
	0, 0);

	}
	if (robzInPos) {
		GFX::DrawSpriteLinearBlend(sprites_rr_robz_idx, robzXpos-7, robzYpos, C2D_Color32(255, 0, 0, 127), 0.5, 1);
		GFX::DrawSpriteLinearBlend(sprites_rr_robz_idx, robzXpos+7, robzYpos, C2D_Color32(0, 0, 255, 127), 0.5, 1);
		Gui::Draw_Rect(0, 0, 50, 240, C2D_Color32(rrFlashFade, rrFlashFade, rrFlashFade, 255));
		Gui::Draw_Rect(400-50, 0, 50, 240, C2D_Color32(rrFlashFade, rrFlashFade, rrFlashFade, 255));
		GFX::DrawSpriteLinearBlend(sprites_rr_shapeBG_idx, 50, 0, C2D_Color32(rrFlashFade, rrFlashFade, rrFlashFade, 255), 0.5, 1);
		Gui::Draw_Rect(0, 238, 400, 2, C2D_Color32(rrFlashFade, rrFlashFade, rrFlashFade, 255));	// Hide line from other texture(s)
	}
	GFX::DrawSpriteLinear(sprites_rr_robz_idx, robzXpos, robzYpos, 0.5, 1);
	if (robzInPos) {
		if (rrTextFade != 255) GFX::DrawSpriteLinear(sprites_logo_rocketrobz_idx, 54, 74, 0.5, 1);
		if (rrTextFade > 0) {
			if (rrTextGlowFade > 0) {
				GFX::DrawSpriteLinearBlend(sprites_logo_rocketrobz_idx, 39, 64, C2D_Color32(0, 255, 0, rrTextGlowFade), 0.55, 1.2);
			}
			GFX::DrawSpriteLinearBlend(sprites_logo_rocketrobz_idx, 54, 74, C2D_Color32(0, 255, 0, rrTextFade), 0.5, 1);
		}
		switch (iFps) {
			default:
				rrFlashFade -= 16;
				rrBgYpos -= 2;
				break;
			case 30:
				rrFlashFade -= 32;
				rrBgYpos -= 4;
				break;
			case 24:
				rrFlashFade -= 40;
				rrBgYpos -= 6;
				break;
		}
		if (rrFlashFade < 0) {
			rrFlashFade = 0;
			switch (iFps) {
				default:
					rrTextFade -= 4;
					rrTextGlowFade -= 8;
					break;
				case 30:
					rrTextFade -= 8;
					rrTextGlowFade -= 16;
					break;
				case 24:
					rrTextFade -= 10;
					rrTextGlowFade -= 20;
					break;
			}
			if (rrTextFade < 0) rrTextFade = 0;
			if (rrTextGlowFade < 0) rrTextGlowFade = 0;
		}
		if (rrBgYpos < -240) rrBgYpos += 240;
	} else {
		switch (iFps) {
			default:
				robzXpos -= 2;
				robzYpos -= 2;
				break;
			case 30:
				robzXpos -= 4;
				robzYpos -= 4;
				break;
			case 24:
				robzXpos -= 6;
				robzYpos -= 6;
				break;
		}
		if (robzXpos < 135) robzXpos = 135;
		if (robzYpos < 19) robzYpos = 19;
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect

	if (shiftBySubPixel) return;
	Gui::ScreenDraw(Bottom);
	if (subMode == 0) {
		GFX::DrawSprite(sprites_logo_UniversalCore_idx, 0, 26);
	} else {
		Gui::DrawStringCentered(0, 48, 0.75, WHITE, this->presentedText);
		GFX::DrawSprite(sprites_logo_horiHD_idx, 32, 85);
	}
	if (rr_fadeAlpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, rr_fadeAlpha)); // Fade in/out effect
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
	#endif
	
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
	if (subMode == 2) {
		fadeFPS += fadeFPS;
	}
	
	if (rr_fadeType) {
		rr_fadeAlpha -= fadeFPS;
		if (rr_fadeAlpha < 0) rr_fadeAlpha = 0;
	} else {
		rr_fadeAlpha += fadeFPS;
		if (rr_fadeAlpha > 255) rr_fadeAlpha = 255;
	}

	#ifdef _3DS
	if (delay > iFps*10 && gfxIsWide()) {
		if (prevSubMode != 0) {
			rr_fadeType = false;
		}
		if (rr_fadeAlpha == 255) {
			subMode = 1;
			rr_fadeType = true;
			prevSubMode++;
		}
	}
	#endif
}


void RocketRobz::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	#ifdef NDS
	if (!graphicLoaded) {
		extern int bg2Main;
		extern int bg3Main;

		std::vector<unsigned char> image;
		unsigned width, height;
		lodepng::decode(image, width, height, "nitro:/graphics/logos/rocketRobz.png");
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
			bgGetGfxPtr(bg2Main)[i] = image[i*4]>>3 | (image[(i*4)+1]>>3)<<5 | (image[(i*4)+2]>>3)<<10 | BIT(15);
		  if (dsiFeatures()) {
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
			bgGetGfxPtr(bg3Main)[i] = image[i*4]>>3 | (image[(i*4)+1]>>3)<<5 | (image[(i*4)+2]>>3)<<10 | BIT(15);
		  }
			if ((i % 256) == 255) alternatePixel = !alternatePixel;
			alternatePixel = !alternatePixel;
		}
		graphicLoaded = true;
	}
	#endif
}