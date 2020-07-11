#include "photoStudio.hpp"
#include "screenvars.h"
#include "settings.hpp"

#include "file_browse.h"

#include "import_ss1charnames.h"
#include "import_ss2charnames.h"
#include "import_ss3charnames.h"
#include "import_ss4charnames.h"

#include "import_ss1bgnames.h"
#include "import_ss2bgnames.h"
#include "import_ss3bgnames.h"
#include "import_ss4bgnames.h"

#include <unistd.h>

static int currentCharacterRendered = 0;
extern bool musicPlayStarted;
extern bool clearTop;
extern bool renderTop;

PhotoStudio::PhotoStudio() {
	currentCharacterRendered = 0;
	GFX::resetCharStatus();
	this->getList();
}

void PhotoStudio::getList() {
	this->getMaxChars();
}

void PhotoStudio::getMaxChars() {
	if (this->subScreenMode == 1) {
		if (photo_highlightedGame == 3) {
			import_totalCharacters = 11;
		} else if (photo_highlightedGame == 2) {
			import_totalCharacters = 8;
		} else if (photo_highlightedGame == 1) {
			import_totalCharacters = 27;
		} else if (photo_highlightedGame == 0) {
			import_totalCharacters = 2;
		}
	} else {
		if (char_highlightedGame[currentCharNum] == 4) {
			import_totalCharacters = numberOfExportedCharacters-1;
		} else if (char_highlightedGame[currentCharNum] == 3) {
			import_totalCharacters = 0xD;
		} else if (char_highlightedGame[currentCharNum] == 2) {
			import_totalCharacters = 0x10;
		} else if (char_highlightedGame[currentCharNum] == 1) {
			import_totalCharacters = 0x12;
		} else if (char_highlightedGame[currentCharNum] == 0) {
			import_totalCharacters = 0x7;
		}
	}
}

const char* PhotoStudio::seasonName(void) const {
	switch (seasonNo) {
		case 0:
			return "Spring";
		case 1:
			return "Summer";
		case 2:
			return "Fall";
		case 3:
			return "Winter";
	}
	
	return "null";
}

const char* PhotoStudio::import_characterName(void) const {
	switch (char_highlightedGame[currentCharNum]) {
		case 0:
			return import_ss1CharacterNames[importCharacterList_cursorPosition[currentCharNum]];
		case 1:
			return import_ss2CharacterNames[importCharacterList_cursorPosition[currentCharNum]];
		case 2:
			return import_ss3CharacterNames[importCharacterList_cursorPosition[currentCharNum]];
		case 3:
			return import_ss4CharacterNames[importCharacterList_cursorPosition[currentCharNum]];
	}
	return "null";
}

const char* PhotoStudio::import_SS2CharacterNames(int i) const {
	switch (sysRegion) {
		default:
			return import_ss2CharacterNames[i];
		case CFG_REGION_EUR:
		case CFG_REGION_AUS:
			return import_nsbCharacterNames[i];
	}
}

const char* PhotoStudio::import_characterNameDisplay(void) const {
	switch (char_highlightedGame[currentCharNum]) {
		case 0:
			return import_ss1CharacterNames[importCharacterList_cursorPosition[currentCharNum]];
		case 1:
			return import_SS2CharacterNames(importCharacterList_cursorPosition[currentCharNum]);
		case 2:
			return import_ss3CharacterNames[importCharacterList_cursorPosition[currentCharNum]];
		case 3:
			return import_ss4CharacterNames[importCharacterList_cursorPosition[currentCharNum]];
	}
	return "null";
}

const char* PhotoStudio::ss1Title(void) const {
	switch (sysRegion) {
		default:
			return "Style Savvy";
		case CFG_REGION_EUR:
		case CFG_REGION_AUS:
			return "Style Boutique";
		case CFG_REGION_JPN:
			return "Wagamama Fashion: Girls Mode";
		case CFG_REGION_KOR:
			return "Namanui Collection: Girls Style";
	}
}

const char* PhotoStudio::ss2Title(void) const {
	switch (sysRegion) {
		default:
			return "Style Savvy: Trendsetters";
		case CFG_REGION_EUR:
		case CFG_REGION_AUS:
			return "New Style Boutique";
		case CFG_REGION_JPN:
			return "Girls Mode: Yokubari Sengen";
		case CFG_REGION_KOR:
			return "Girls Style: Paesyeon Lideo Seon-eon!";
	}
}

const char* PhotoStudio::ss3Title(void) const {
	switch (sysRegion) {
		default:
			return "Style Savvy: Fashion Forward";
		case CFG_REGION_EUR:
		case CFG_REGION_AUS:
			return "New Style Boutique 2: Fashion Forward";
		case CFG_REGION_JPN:
			return "Girls Mode 3: Kirakira * Code";
		case CFG_REGION_KOR:
			return "Girls Style: Kirakira * Code";
	}
}

const char* PhotoStudio::ss4Title(void) const {
	switch (sysRegion) {
		default:
			return "Style Savvy: Styling Star";
		case CFG_REGION_EUR:
		case CFG_REGION_AUS:
			return "New Style Boutique 3: Styling Star";
		case CFG_REGION_JPN:
			return "Girls Mode 4: Star Stylist";
		case CFG_REGION_KOR:
			return "Girls Style: Star Stylist";
	}
}

void PhotoStudio::drawMsg(void) const {
	GFX::DrawSprite(sprites_msg_idx, 0, 8, 1, 1);
	GFX::DrawSprite(sprites_msg_idx, 160, 8, -1, 1);
	GFX::DrawSprite(messageNo == 4 ? sprites_icon_question_idx : sprites_icon_msg_idx, 132, -2);

	Gui::DrawStringCentered(0, 84, 0.60, BLACK, "This feature is not available yet.");

	GFX::DrawSprite(sprites_button_msg_shadow_idx, 114, 197);
	GFX::DrawSprite(sprites_button_msg_idx, 115, 188);
	Gui::DrawString(134, 196, 0.70, MSG_BUTTONTEXT, " OK!");
}

void PhotoStudio::loadChrImage(bool Robz) {
	this->previewCharacter = false;
	gspWaitForVBlank();
	if (char_highlightedGame[currentCharNum] == 4) {
		if (numberOfExportedCharacters > 0) {
			sprintf(this->chrFilePath, "sdmc:/3ds/SavvyManager/SS%i/characters/previews/%s.t3x", 4, getExportedCharacterName(this->importCharacterList_cursorPosition[currentCharNum]));	// All Seasons
		} else {
			sprintf(this->chrFilePath, "romfs:/gfx/null.t3x");	// All Seasons
		}
		this->previewCharacterFound[currentCharNum] = GFX::loadCharSprite(currentCharNum, this->chrFilePath, this->chrFilePath);
	} else {
		sprintf(this->chrFilePath, "romfs:/gfx/ss%i_%s.t3x", 4, (Robz ? "Robz" : import_characterName()));				// All Seasons
		sprintf(this->chrFilePath2, "romfs:/gfx/ss%i_%s%i.t3x", 4, (Robz ? "Robz" : import_characterName()), this->seasonNo);	// One Season
		this->previewCharacterFound[currentCharNum] = GFX::loadCharSprite(currentCharNum, this->chrFilePath, this->chrFilePath2);
	}
	if (previewCharacterFound[0] && !characterPicked[1]) {
		GFX::loadCharSpriteMem(0);
	}
	this->previewCharacter = true;
}


void PhotoStudio::Draw(void) const {
	animateBg = bgCanAnimate;

	if (!musicPlayStarted) {
		extern void Play_Music();
		Play_Music();
		musicPlayStarted = true;
	}

  if (renderTop) {
	Gui::ScreenDraw(Top);

	this->preview();

	if (currentCharacterRendered == 0) {
		if (this->displayStudioBg) {
			GFX::showBgSprite(zoomIn);
		} else if (this->showScrollingBg) {
			GFX::DrawSprite(sprites_title_idx, 0, 0, 0.5);
		} else {
			Gui::Draw_Rect(0, 0, 400, 240, WHITE);
		}
		if (this->previewCharacter) {
			if (this->previewCharacterFound[0]) {
				if (characterPicked[1]) {
					GFX::loadCharSpriteMem(0);
				}
				GFX::showCharSprite(0, this->zoomIn, this->charFadeAlpha, this->displayStudioBg);
			} else {
				Gui::DrawStringCentered(0, 104, 0.65, WHITE, (this->char_highlightedGame[currentCharNum]==4 ? "Preview not found." : "Preview unavailable."));
			}
		}
	} else if (currentCharacterRendered == 1) {
		if (characterPicked[1] && this->previewCharacterFound[1]) {
			GFX::loadCharSpriteMem(1);
			GFX::showCharSprite(1, this->zoomIn, this->charFadeAlpha, this->displayStudioBg);
		}
	}

	if (renderTop && subScreenMode!=1 && previewCharacter && previewCharacterFound[0] && previewCharacterFound[1]) {
		currentCharacterRendered++;
		if (currentCharacterRendered > 1) {
			currentCharacterRendered = 0;
			renderTop = false;
		}
		clearTop = (currentCharacterRendered == 0);
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
  }

	if (cinemaWide) {
		Gui::Draw_Rect(0, 0, 400, 36, C2D_Color32(0, 0, 0, 255));
		Gui::Draw_Rect(0, 204, 400, 36, C2D_Color32(0, 0, 0, 255));
	}

	if (shiftBySubPixel) {
		Gui::Draw_Rect(0, 239, 400, 1, C2D_Color32(0, 0, 0, 255));
		return;
	}
	Gui::ScreenDraw(Bottom);
	GFX::DrawSprite(sprites_photo_bg_idx, 0, 0);

	this->cursorX = 248;
	if (this->subScreenMode == 2) {
		this->cursorY = 64+(48*this->importCharacterList_cursorPositionOnScreen[currentCharNum]);

		// Game name
		switch (this->char_highlightedGame[currentCharNum]) {
			case 4:
				Gui::DrawStringCentered(0, 8, 0.50, WHITE, "Your character files");
				break;
			case 3:
				Gui::DrawStringCentered(0, 8, 0.50, WHITE, ss4Title());
				break;
			case 2:
				Gui::DrawStringCentered(0, 8, 0.50, WHITE, ss3Title());
				break;
			case 1:
				Gui::DrawStringCentered(0, 8, 0.50, WHITE, ss2Title());
				break;
			case 0:
				Gui::DrawStringCentered(0, 8, 0.50, WHITE, ss1Title());
				break;
		}
		Gui::DrawString(8, 8, 0.50, WHITE, "<");
		Gui::DrawString(304, 8, 0.50, WHITE, ">");

		if (char_highlightedGame[currentCharNum] != 4) {
			// Selected season
			Gui::DrawString(120-32, 208, 0.65, WHITE, "L");
			Gui::DrawStringCentered(-32, 210, 0.50, WHITE, this->seasonName());
			Gui::DrawString(192-32, 208, 0.65, WHITE, "R");
		}

		Gui::DrawString(192, 208, 0.65, BLUE, "SELECT: Robz");

	  if (!displayNothing) {
		int i2 = 48;
		for (int i = import_characterShownFirst[currentCharNum]; i < import_characterShownFirst[currentCharNum]+3; i++) {
			if (char_highlightedGame[currentCharNum] == 4) {
				if (i >= numberOfExportedCharacters) break;
				GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
				GFX::DrawSprite((getExportedCharacterGender(i) ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
				Gui::DrawString(64, i2, 0.65, WHITE, getExportedCharacterName(i));
			} else if (char_highlightedGame[currentCharNum] == 3) {
				GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
				GFX::DrawSprite((import_ss4CharacterGenders[i] ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
				Gui::DrawString(64, i2, 0.65, WHITE, import_ss4CharacterNames[i]);
			} else if (char_highlightedGame[currentCharNum] == 2) {
				GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
				GFX::DrawSprite((import_ss3CharacterGenders[i] ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
				Gui::DrawString(64, i2, 0.65, WHITE, import_ss3CharacterNames[i]);
			} else if (char_highlightedGame[currentCharNum] == 1) {
				GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
				GFX::DrawSprite((import_ss2CharacterGenders[i] ? sprites_icon_male_idx : sprites_icon_female_idx)/*+import_ss2CharacterTieColors[i]*/, 12, i2-8);
				Gui::DrawString(64, i2, 0.65, WHITE, import_SS2CharacterNames(i));
			} else if (char_highlightedGame[currentCharNum] == 0) {
				GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
				GFX::DrawSprite((import_ss1CharacterGenders[i] ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
				Gui::DrawString(64, i2, 0.65, WHITE, import_ss1CharacterNames[i]);
			}
			i2 += 48;
		}
	  }
	} else if (this->subScreenMode == 1) {
		this->cursorY = 64+(48*this->bgList_cursorPositionOnScreen);

		// Game name
		switch (this->photo_highlightedGame) {
			case 4:
				Gui::DrawStringCentered(0, 8, 0.50, WHITE, "Your character files");
				break;
			case 3:
				Gui::DrawStringCentered(0, 8, 0.50, WHITE, ss4Title());
				break;
			case 2:
				Gui::DrawStringCentered(0, 8, 0.50, WHITE, ss3Title());
				break;
			case 1:
				Gui::DrawStringCentered(0, 8, 0.50, WHITE, ss2Title());
				break;
			case 0:
				Gui::DrawStringCentered(0, 8, 0.50, WHITE, ss1Title());
				break;
		}
		Gui::DrawString(8, 8, 0.50, WHITE, "<");
		Gui::DrawString(304, 8, 0.50, WHITE, ">");

		/*if (photo_highlightedGame != 4) {
			// Selected season
			Gui::DrawString(120, 208, 0.65, BLACK, "L");
			Gui::DrawStringCentered(0, 210, 0.50, BLACK, this->seasonName());
			Gui::DrawString(192, 208, 0.65, BLACK, "R");
		}*/

	  if (!displayNothing) {
		int i2 = 48;
		for (int i = import_bgShownFirst; i < import_bgShownFirst+3; i++) {
			if (photo_highlightedGame == 3) {
				GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
				Gui::DrawString(32, i2, 0.65, WHITE, import_ss4BgNames[i]);
			} else if (photo_highlightedGame == 2) {
				GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
				Gui::DrawString(32, i2, 0.65, WHITE, import_ss3BgNames[i]);
			} else if (photo_highlightedGame == 1) {
				GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
				Gui::DrawString(32, i2, 0.65, WHITE, import_ss2BgNames[i]);
			} else if (photo_highlightedGame == 0) {
				GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
				Gui::DrawString(32, i2, 0.65, WHITE, import_ss1BgNames[i]);
			}
			i2 += 48;
		}
	  }
	} else {
		this->cursorY = 64+(48*this->characterChangeMenu_cursorPositionOnScreen);

		Gui::DrawString(8, 8, 0.50, WHITE, "What do you want to do?");

		Gui::DrawString(192, 206, 0.65, WHITE, ": Take photo");

		int i2 = 0;
		i2 += 48;
		GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
		Gui::DrawString(32, i2, 0.65, WHITE, "Change location");
		i2 += 48;
		GFX::DrawSprite(sprites_item_button_idx, 16, i2-20);
		if (currentCharNum==1) {
			Gui::DrawString(32, i2, 0.65, WHITE, "Change character < 2 >");
		} else {
			Gui::DrawString(32, i2, 0.65, WHITE, "Change character < 1 >");
		}
	}

	if (this->subScreenMode != 0) {
		GFX::DrawSprite(sprites_button_shadow_idx, 5, 199);
		GFX::DrawSprite(sprites_button_red_idx, 5, 195);
		GFX::DrawSprite(sprites_arrow_back_idx, 19, 195);
		GFX::DrawSprite(sprites_button_b_idx, 44, 218);
	}

	GFX::drawCursor(this->cursorX, this->cursorY);

	if (showMessage) {
		drawMsg();
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
}

void PhotoStudio::preview() const {
	if (characterPicked[1]) {
		this->charFadeAlpha = 255;
	} else if (this->previewCharacter) {
		switch (iFps) {
			default:
				this->charFadeAlpha += 20;
				break;
			case 30:
				this->charFadeAlpha += 40;
				break;
			case 24:
				this->charFadeAlpha += 55;
				break;
		}
		if (this->charFadeAlpha > 255) this->charFadeAlpha = 255;
	} else {
		this->charFadeAlpha = 0;
	}
}



void PhotoStudio::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if ((this->subScreenMode==0 || this->subScreenMode==2)) {
		int zoomLimit = characterPicked[1] ? 1 : 2;
		if (hDown & KEY_CPAD_UP) {
			this->zoomIn++;
			if (this->zoomIn > zoomLimit) {
				this->zoomIn = zoomLimit;
			} else {
				renderTop = true;
			}
		}
		if (hDown & KEY_CPAD_DOWN) {
			this->zoomIn--;
			if (this->zoomIn < 0) {
				this->zoomIn = 0;
			} else {
				renderTop = true;
			}
		}
	}

	if (this->showMessage) {
		if ((hDown & KEY_A) || ((hDown & KEY_TOUCH) && touch.px >= 115 && touch.px < 115+90 && touch.py >= 188 && touch.py < 188+47)) {
			sndSelect();
			this->showMessage = false;
		}
	} else if (this->subScreenMode == 2) {
		if (showCursor) {
			if (hDown & KEY_DUP) {
				sndHighlight();
				this->importCharacterList_cursorPosition[currentCharNum]--;
				this->importCharacterList_cursorPositionOnScreen[currentCharNum]--;
				if (this->importCharacterList_cursorPosition[currentCharNum] < 0) {
					this->importCharacterList_cursorPosition[currentCharNum] = 0;
					this->import_characterShownFirst[currentCharNum] = 0;
				} else if (this->importCharacterList_cursorPosition[currentCharNum] < this->import_characterShownFirst[currentCharNum]) {
					this->import_characterShownFirst[currentCharNum]--;
				}
				if (this->importCharacterList_cursorPositionOnScreen[currentCharNum] < 0) {
					this->importCharacterList_cursorPositionOnScreen[currentCharNum] = 0;
				}
				renderTop = true;
				this->loadChrImage(false);
			}

			if (hDown & KEY_DDOWN) {
				sndHighlight();
				this->importCharacterList_cursorPosition[currentCharNum]++;
				this->importCharacterList_cursorPositionOnScreen[currentCharNum]++;
				if (this->importCharacterList_cursorPosition[currentCharNum] > this->import_totalCharacters) {
					this->importCharacterList_cursorPosition[currentCharNum] = this->import_totalCharacters;
					this->import_characterShownFirst[currentCharNum] = this->import_totalCharacters-2;
					if (this->import_characterShownFirst[currentCharNum] < 0) this->import_characterShownFirst[currentCharNum] = 0;
					if (this->importCharacterList_cursorPositionOnScreen[currentCharNum] > this->import_totalCharacters) {
						this->importCharacterList_cursorPositionOnScreen[currentCharNum] = this->import_totalCharacters;
					}
				} else if (this->importCharacterList_cursorPosition[currentCharNum] > this->import_characterShownFirst[currentCharNum]+2) {
					this->import_characterShownFirst[currentCharNum]++;
				}
				if (this->importCharacterList_cursorPositionOnScreen[currentCharNum] > 2) {
					this->importCharacterList_cursorPositionOnScreen[currentCharNum] = 2;
				}
				renderTop = true;
				this->loadChrImage(false);
			}
		}

		if (hDown & KEY_A) {
			sndSelect();
			this->subScreenMode = 0;
		} else if (hDown & KEY_SELECT) {
			sndSelect();
			this->subScreenMode = 0;
			renderTop = true;
			this->loadChrImage(true);	// Load Robz
		}

		if (hDown & KEY_DLEFT) {
			sndHighlight();
			this->char_highlightedGame[currentCharNum]--;
			if (this->char_highlightedGame[currentCharNum] < 0) this->char_highlightedGame[currentCharNum] = 4;
			this->getMaxChars();
			renderTop = true;
		}

		if (hDown & KEY_DRIGHT) {
			sndHighlight();
			this->char_highlightedGame[currentCharNum]++;
			if (this->char_highlightedGame[currentCharNum] > 4) this->char_highlightedGame[currentCharNum] = 0;
			this->getMaxChars();
			renderTop = true;
		}

		if ((hDown & KEY_DLEFT) || (hDown & KEY_DRIGHT)) {
			this->importCharacterList_cursorPosition[currentCharNum] = 0;
			this->importCharacterList_cursorPositionOnScreen[currentCharNum] = 0;
			this->import_characterShownFirst[currentCharNum] = 0;
			if (this->char_highlightedGame[currentCharNum] == 4) {
				this->previewCharacter = false;
				if (!this->exportedCharListGotten) {
					this->displayNothing = true;
					gspWaitForVBlank();
					getExportedCharacterContents();
					this->exportedCharListGotten = true;
					this->displayNothing = false;
				}
			}
			this->getMaxChars();
			renderTop = true;
			this->loadChrImage(false);
		}

		if (this->char_highlightedGame[currentCharNum] != 4) {
			if ((hDown & KEY_L) || (hDown & KEY_ZL)) {
				sndHighlight();
				this->seasonNo--;
				if (this->seasonNo < 0) this->seasonNo = 3;
				this->loadChrImage(false);
				renderTop = true;
			}

			if ((hDown & KEY_R) || (hDown & KEY_ZR)) {
				sndHighlight();
				this->seasonNo++;
				if (this->seasonNo > 3) this->seasonNo = 0;
				this->loadChrImage(false);
				renderTop = true;
			}
		}

		if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touchingBackButton())) {
			sndBack();
			this->subScreenMode = 0;
		}

	} else if (this->subScreenMode == 1) {
		if (showCursor) {
			if (hDown & KEY_DUP) {
				sndHighlight();
				this->bgList_cursorPosition--;
				this->bgList_cursorPositionOnScreen--;
				if (this->bgList_cursorPosition < 0) {
					this->bgList_cursorPosition = 0;
					this->import_bgShownFirst = 0;
				} else if (this->bgList_cursorPosition < this->import_bgShownFirst) {
					this->import_bgShownFirst--;
				}
				if (this->bgList_cursorPositionOnScreen < 0) {
					this->bgList_cursorPositionOnScreen = 0;
				}
				switch (this->photo_highlightedGame) {
					case 0:
						studioBg = import_ss1BgNums[this->bgList_cursorPosition];
						break;
					case 1:
						studioBg = import_ss2BgNums[this->bgList_cursorPosition];
						break;
					case 2:
						studioBg = import_ss3BgNums[this->bgList_cursorPosition];
						break;
					case 3:
						studioBg = import_ss4BgNums[this->bgList_cursorPosition];
						break;
				}
				this->displayStudioBg = false;
				gspWaitForVBlank();
				GFX::reloadBgSprite();
				this->displayStudioBg = true;
			}

			if (hDown & KEY_DDOWN) {
				sndHighlight();
				this->bgList_cursorPosition++;
				this->bgList_cursorPositionOnScreen++;
				if (this->bgList_cursorPosition > this->import_totalCharacters) {
					this->bgList_cursorPosition = this->import_totalCharacters;
					this->import_bgShownFirst = this->import_totalCharacters-2;
					if (this->import_bgShownFirst < 0) this->import_bgShownFirst = 0;
					if (this->bgList_cursorPositionOnScreen > this->import_totalCharacters) {
						this->bgList_cursorPositionOnScreen = this->import_totalCharacters;
					}
				} else if (this->bgList_cursorPosition > this->import_bgShownFirst+2) {
					this->import_bgShownFirst++;
				}
				if (this->bgList_cursorPositionOnScreen > 2) {
					this->bgList_cursorPositionOnScreen = 2;
				}
				switch (this->photo_highlightedGame) {
					case 0:
						studioBg = import_ss1BgNums[this->bgList_cursorPosition];
						break;
					case 1:
						studioBg = import_ss2BgNums[this->bgList_cursorPosition];
						break;
					case 2:
						studioBg = import_ss3BgNums[this->bgList_cursorPosition];
						break;
					case 3:
						studioBg = import_ss4BgNums[this->bgList_cursorPosition];
						break;
				}
				this->displayStudioBg = false;
				gspWaitForVBlank();
				GFX::reloadBgSprite();
				this->displayStudioBg = true;
			}
		}

		if (hDown & KEY_A) {
			sndSelect();
			this->subScreenMode = 0;
			this->previewCharacter = this->characterPicked[currentCharNum];
		}

		if (hDown & KEY_DLEFT) {
			sndHighlight();
			this->photo_highlightedGame--;
			if (this->photo_highlightedGame < 0) this->photo_highlightedGame = 3;
			this->getMaxChars();
		}

		if (hDown & KEY_DRIGHT) {
			sndHighlight();
			this->photo_highlightedGame++;
			if (this->photo_highlightedGame > 3) this->photo_highlightedGame = 0;
			this->getMaxChars();
		}

		if ((hDown & KEY_DLEFT) || (hDown & KEY_DRIGHT)) {
			this->bgList_cursorPosition = 0;
			this->bgList_cursorPositionOnScreen = 0;
			this->import_bgShownFirst = 0;
			this->getMaxChars();
			switch (this->photo_highlightedGame) {
				case 0:
					studioBg = import_ss1BgNums[this->bgList_cursorPosition];
					break;
				case 1:
					studioBg = import_ss2BgNums[this->bgList_cursorPosition];
					break;
				case 2:
					studioBg = import_ss3BgNums[this->bgList_cursorPosition];
					break;
				case 3:
					studioBg = import_ss4BgNums[this->bgList_cursorPosition];
					break;
			}
			this->displayStudioBg = false;
			gspWaitForVBlank();
			GFX::reloadBgSprite();
			this->displayStudioBg = true;
		}

		if ((hDown & KEY_B) || ((hDown & KEY_TOUCH) && touchingBackButton())) {
			sndBack();
			this->subScreenMode = 0;
			this->previewCharacter = this->characterPicked[currentCharNum];
		}

	} else {
		if (showCursor) {
			if (hDown & KEY_DUP) {
				sndHighlight();
				this->characterChangeMenu_cursorPosition--;
				this->characterChangeMenu_cursorPositionOnScreen--;
				if (this->characterChangeMenu_cursorPosition < 0) {
					this->characterChangeMenu_cursorPosition = 0;
				}
				if (this->characterChangeMenu_cursorPositionOnScreen < 0) {
					this->characterChangeMenu_cursorPositionOnScreen = 0;
				}
			}

			if (hDown & KEY_DDOWN) {
				sndHighlight();
				this->characterChangeMenu_cursorPosition++;
				this->characterChangeMenu_cursorPositionOnScreen++;
				if (this->characterChangeMenu_cursorPosition > 1) {
					this->characterChangeMenu_cursorPosition = 1;
				}
				if (this->characterChangeMenu_cursorPositionOnScreen > 1) {
					this->characterChangeMenu_cursorPositionOnScreen = 1;
				}
			}

			if (this->characterChangeMenu_cursorPosition == 1) {
				if (hDown & KEY_DLEFT) {
					sndHighlight();
					currentCharNum--;
					if (currentCharNum < 0) {
						currentCharNum = 0;
					}
				}
				if (hDown & KEY_DRIGHT) {
					sndHighlight();
					currentCharNum++;
					if (currentCharNum > 1) {
						currentCharNum = 1;
					}
				}
			}
		}

		if (hDown & KEY_A) {
			if (this->characterChangeMenu_cursorPosition == 0) {
				sndSelect();
				this->subScreenMode = 1;
				this->getMaxChars();
				this->previewCharacter = false;
				int bgNum = 0;
				switch (this->photo_highlightedGame) {
					case 0:
						bgNum = import_ss1BgNums[this->bgList_cursorPosition];
						break;
					case 1:
						bgNum = import_ss2BgNums[this->bgList_cursorPosition];
						break;
					case 2:
						bgNum = import_ss3BgNums[this->bgList_cursorPosition];
						break;
					case 3:
						bgNum = import_ss4BgNums[this->bgList_cursorPosition];
						break;
				}
				//if (studioBg != bgNum) {
					this->showScrollingBg = false;
					this->displayStudioBg = false;
					gspWaitForVBlank();
					studioBg = bgNum;
					GFX::reloadBgSprite();
					this->displayStudioBg = true;
				//}
				this->zoomIn = 0;
				renderTop = true;
			} else if (this->characterChangeMenu_cursorPosition == 1) {
				sndSelect();
				this->displayNothing = true;
				this->subScreenMode = 2;
				if ((this->subScreenMode == 2) && (this->char_highlightedGame[currentCharNum] == 4) && !this->exportedCharListGotten) {
					gspWaitForVBlank();
					getExportedCharacterContents();
					this->exportedCharListGotten = true;
				}
				this->getMaxChars();
				this->displayNothing = false;
				this->characterPicked[currentCharNum] = true;
				if (characterPicked[0] && characterPicked[1] && this->zoomIn > 1) {
					this->zoomIn = 1;
				}
				renderTop = true;
				this->loadChrImage(false);
			}
		}
	
		if (hDown & KEY_SELECT) {
			sndSelect();
			Gui::setScreen(std::make_unique<Settings>(), true);
		}
	}
}