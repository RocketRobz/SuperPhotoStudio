#include "rocketRobz.hpp"
#include "screenvars.h"

static int subMode = 0;
static int prevSubMode = -1;
extern int delay;
static int rr_fadeAlpha = 0;
static int rr_fadeType = true;

void RocketRobz::Draw(void) const {
	fadecolor = 0;	// Always use black color for fading effects

	Gui::ScreenDraw(Top);

	if (cinemaWide) {
		GFX::DrawSpriteLinear(sprites_logo_rocketrobz_idx, 60, 36, 0.35f, 0.7f);
	} else {
		GFX::DrawSpriteLinear(sprites_logo_rocketrobz_idx, 0, 0, 0.5, 1);
	}
	Gui::Draw_Rect(0, 238, 400, 2, BLACK);	// Hide line from other texture(s)

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect

	if (cinemaWide) {
		Gui::Draw_Rect(0, 0, 400, 36, BLACK);
		Gui::Draw_Rect(0, 204, 400, 36, BLACK);
	}

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
	if (delay > iFps*9 && gfxIsWide()) {
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


void RocketRobz::Logic(u32 hDown, u32 hHeld, touchPosition touch) { }