#include "productIdent.hpp"

extern char verText[32];

void ProductIdent::Draw(void) const {
	/*fadecolor = 0;	// Always use black color for fading effects

	Gui::DrawString(26, 86, 0.45, LIGHTRED, "Nintendo DS Product Identification");
	Gui::Draw_Rect(24, 100, 400-28, 1, LIGHTRED);
	Gui::DrawString(40, 100, 0.45, LIGHTRED, "Title:");
	Gui::DrawString(192, 100, 0.45, LIGHTRED, "SUPERPHOTO");
	Gui::DrawString(40, 110, 0.45, LIGHTRED, "Product Code:");
	Gui::DrawString(192, 110, 0.45, LIGHTRED, "TWL-HPHA");
	Gui::DrawString(40, 130, 0.45, LIGHTRED, "Variant:");
	Gui::DrawString(192, 130, 0.45, LIGHTRED, verText);
	Gui::DrawString(40, 140, 0.45, LIGHTRED, "Developer:");
	Gui::DrawString(192, 140, 0.45, LIGHTRED, "RocketRobz");
	Gui::Draw_Rect(38, 155, 400-42, 1, LIGHTRED);
	Gui::DrawString(40, 198, 0.50, LIGHTRED, "2020 RocketRobz");
	Gui::Draw_Rect(38, 214, 400-42, 1, LIGHTRED);
	Gui::DrawString(40, 214, 0.45, LIGHTRED, "robz designs on the future <<<");*/
}


void ProductIdent::Logic(u32 hDown, u32 hHeld, touchPosition touch) { }