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

#ifdef _3DS
static int robzXpos = 135+220;
static int robzYpos = 19+220;
static int rrTextFade = 255;
#endif

void RocketRobz::Draw(void) const {
	fadecolor = 0;	// Always use black color for fading effects

	#ifdef _3DS
	Gui::ScreenDraw(Top);

	// Top half gradient
	C2D_DrawRectangle(
		64, 42, 0, 280, 76,
		C2D_Color32(82, 0, 121, 255), C2D_Color32(82, 0, 121, 255),
		C2D_Color32(169, 0, 254, 255), C2D_Color32(169, 0, 254, 255));
	// Bottom half gradient
	C2D_DrawRectangle(
		64, 118, 0, 280, 86,
		C2D_Color32(169, 0, 254, 255), C2D_Color32(169, 0, 254, 255),
		C2D_Color32(71, 0, 104, 255), C2D_Color32(71, 0, 104, 255));

	GFX::DrawSpriteLinearBlend(sprites_rr_robz_idx, robzXpos-7, robzYpos, C2D_Color32(255, 0, 0, 127), 0.5, 1);
	GFX::DrawSpriteLinearBlend(sprites_rr_robz_idx, robzXpos+7, robzYpos, C2D_Color32(0, 0, 255, 127), 0.5, 1);
	Gui::Draw_Rect(400-50, 0, 50, 240, C2D_Color32(0, 0, 0, 255));
	GFX::DrawSpriteLinear(sprites_rr_shapeBG_idx, 50, 0, 0.5, 1);
	Gui::Draw_Rect(0, 238, 400, 2, C2D_Color32(0, 0, 0, 255));	// Hide line from other texture(s)
	GFX::DrawSpriteLinear(sprites_rr_robz_idx, robzXpos, robzYpos, 0.5, 1);
	if (robzXpos == 135 && robzYpos == 19) {
		GFX::DrawSpriteLinear(sprites_logo_rocketrobz_idx, 54, 74, 0.5, 1);
		if (rrTextFade > 0) GFX::DrawSpriteLinearBlend(sprites_logo_rocketrobz_idx, 54, 74, C2D_Color32(255, 255, 255, rrTextFade), 0.5, 1);
		rrTextFade -= 16;
		if (rrTextFade < 0) rrTextFade = 0;
	} else {
		robzXpos -= 4;
		robzYpos -= 4;
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
			if ((i % 256) == 255) alternatePixel = !alternatePixel;
			alternatePixel = !alternatePixel;
		}
		graphicLoaded = true;
	}
	#endif
}