#ifdef _3DS

#include "photoStudio.hpp"
#include "screenvars.h"

extern bool clearTop;
extern bool renderTop;

static char txt_cinemaWide[24];
static char txt_frameRate[24];

void PhotoStudio::SettingsDraw(void) const {
	cursorY = 64+(48*settings_cursorPositionOnScreen);

	Gui::DrawStringCentered(0, 8, 0.55, WHITE, "Settings");

	int i2 = 48;
	for (int i = settingShownFirst; i < settingShownFirst+3; i++) {
		if (i > numberOfSettings) break;
	
		GFX::DrawSprite(sprites_item_button_idx, 18, i2-20);
		switch (i) {
			case 0:
				sprintf(txt_cinemaWide, "Aspect Ratio: %s", cinemaWide ? "Cinema" : "Normal");
				Gui::DrawString(32, i2, 0.65, WHITE, txt_cinemaWide);
				break;
			case 1:
				sprintf(txt_frameRate, "Frame Rate: %iFPS", iFps);
				Gui::DrawString(32, i2, 0.65, WHITE, txt_frameRate);
				break;
			default:
				break;
		}
		i2 += 48;
	}
}

void PhotoStudio::SettingsLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (showCursor) {
		if (hDown & KEY_UP) {
			sndHighlight();
			settings_cursorPosition--;
			settings_cursorPositionOnScreen--;
			if (settings_cursorPosition < 0) {
				settings_cursorPosition = 0;
				settingShownFirst = 0;
			} else if (settings_cursorPosition < settingShownFirst) {
				settingShownFirst--;
			}
			if (settings_cursorPositionOnScreen < 0) {
				settings_cursorPositionOnScreen = 0;
			}
		}

		if (hDown & KEY_DOWN) {
			sndHighlight();
			settings_cursorPosition++;
			settings_cursorPositionOnScreen++;
			if (settings_cursorPosition > numberOfSettings) {
				settings_cursorPosition = numberOfSettings;
				settingShownFirst = numberOfSettings-2;
				if (settingShownFirst < 0) settingShownFirst = 0;
				if (settings_cursorPositionOnScreen > numberOfSettings) {
					settings_cursorPositionOnScreen = numberOfSettings;
				}
			} else if (settings_cursorPosition > settingShownFirst+2) {
				settingShownFirst++;
			}
			if (settings_cursorPositionOnScreen > 2) {
				settings_cursorPositionOnScreen = 2;
			}
		}
	}

	if ((hDown & KEY_LEFT) && (settings_cursorPosition <= numberOfSettings)) {
		sndSelect();
		switch (settings_cursorPosition) {
			case 0:
				cinemaWide = !cinemaWide;
				renderTop = true;
				break;
			case 1:
					 if (iFps==30) iFps = 24;
				else if (iFps==24) iFps = 60;
				else if (iFps==60) iFps = 30;
				C3D_FrameRate(iFps);
				break;
		}
	}

	if (((hDown & KEY_RIGHT) || (hDown & KEY_A)) && (settings_cursorPosition <= numberOfSettings)) {
		sndSelect();
		switch (settings_cursorPosition) {
			case 0:
				cinemaWide = !cinemaWide;
				renderTop = true;
				break;
			case 1:
					 if (iFps==60) iFps = 24;
				else if (iFps==24) iFps = 30;
				else if (iFps==30) iFps = 60;
				C3D_FrameRate(iFps);
				break;
		}
	}

	if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touchingBackButton())) {
		sndBack();
		subScreenMode = 0;
	}
}

#endif