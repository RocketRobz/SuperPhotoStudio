#include "rocketRobz.hpp"
#include "screenvars.h"
#ifdef NDS
#include "lodepng.h"
#endif

static int subMode = 0;
#ifdef _3DS
static int prevSubMode = -1;
#endif
extern int delay;
static int rr_fadeAlpha = 0;
static int rr_fadeType = true;

void RocketRobz::Draw(void) const {
	fadecolor = 0;	// Always use black color for fading effects

	#ifdef _3DS
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
			alternatePixel = !alternatePixel;
		}
		graphicLoaded = true;
	}
	#endif
}