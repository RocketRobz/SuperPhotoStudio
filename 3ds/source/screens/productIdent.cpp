#include "productIdent.hpp"

extern int delay;
extern char verText[32];
extern u8 consoleModel;

void ProductIdent::Draw(void) const {
	fadecolor = 0;	// Always use black color for fading effects

	Gui::ScreenDraw(Top);

	switch (consoleModel) {
		case 0:
		default:
			Gui::DrawString(26, 86-(shiftBySubPixel ? 0.5f : 0), 0.45, LIGHTRED, "Nintendo 3DS Product Identification");
			break;
		case 1:
			Gui::DrawString(26, 86-(shiftBySubPixel ? 0.5f : 0), 0.45, LIGHTRED, "Nintendo 3DS XL Product Identification");
			break;
		case 2:
			Gui::DrawString(26, 86-(shiftBySubPixel ? 0.5f : 0), 0.45, LIGHTRED, "new Nintendo 3DS Product Identification");
			break;
		case 3:
			Gui::DrawString(26, 86-(shiftBySubPixel ? 0.5f : 0), 0.45, LIGHTRED, "Nintendo 2DS Product Identification");
			break;
		case 4:
			Gui::DrawString(26, 86-(shiftBySubPixel ? 0.5f : 0), 0.45, LIGHTRED, "new Nintendo 3DS XL Product Identification");
			break;
		case 5:
			Gui::DrawString(26, 86-(shiftBySubPixel ? 0.5f : 0), 0.45, LIGHTRED, "new Nintendo 2DS XL Product Identification");
			break;
	}
	Gui::Draw_Rect(24, 100-(shiftBySubPixel ? 0.5f : 0), 400-28, 1, LIGHTRED);
	Gui::DrawString(40, 100-(shiftBySubPixel ? 0.5f : 0), 0.45, LIGHTRED, "Title:");
	Gui::DrawString(192, 100-(shiftBySubPixel ? 0.5f : 0), 0.45, LIGHTRED, "SuperPhotoStudio");
	Gui::DrawString(40, 110-(shiftBySubPixel ? 0.5f : 0), 0.45, LIGHTRED, "Product Code:");
	Gui::DrawString(192, 110-(shiftBySubPixel ? 0.5f : 0), 0.45, LIGHTRED, "CTR-H-RPHO");
	Gui::DrawString(40, 120-(shiftBySubPixel ? 0.5f : 0), 0.45, LIGHTRED, "Unique ID:");
	Gui::DrawString(192, 120-(shiftBySubPixel ? 0.5f : 0), 0.45, LIGHTRED, "0x52504");
	Gui::DrawString(40, 130-(shiftBySubPixel ? 0.5f : 0), 0.45, LIGHTRED, "Variant:");
	Gui::DrawString(192, 130-(shiftBySubPixel ? 0.5f : 0), 0.45, LIGHTRED, verText);
	Gui::DrawString(40, 140-(shiftBySubPixel ? 0.5f : 0), 0.45, LIGHTRED, "Developer:");
	Gui::DrawString(192, 140-(shiftBySubPixel ? 0.5f : 0), 0.45, LIGHTRED, "Rocket Robz");
	Gui::Draw_Rect(38, 155-(shiftBySubPixel ? 0.5f : 0), 400-42, 1, LIGHTRED);
	if (gfxIsWide()) {
		Gui::DrawString(40, 176-(shiftBySubPixel ? 0.5f : 0), 0.50, WHITE, "Horizon HD (800px MODE) ACTIVATED");
	}
	Gui::DrawString(40, 198-(shiftBySubPixel ? 0.5f : 0), 0.50, LIGHTRED, "2020-2024 Rocket Robz");
	Gui::Draw_Rect(38, 214-(shiftBySubPixel ? 0.5f : 0), 400-42, 1, LIGHTRED);
	Gui::DrawString(40, 214-(shiftBySubPixel ? 0.5f : 0), 0.45, LIGHTRED, "robz designs on the future <<<");

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect

	Gui::ScreenDraw(Bottom);
}


void ProductIdent::Logic(u32 hDown, u32 hHeld, touchPosition touch) { }