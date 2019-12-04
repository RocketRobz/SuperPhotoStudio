﻿#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <3ds.h>
#include <malloc.h>
#include <unistd.h>		// access
#include <sys/stat.h>

#include "gui.hpp"
#include "savedata.h"

#include "ss3charnames.h"
#include "ss4charnames.h"
#include "import_ss1charnames.h"
#include "import_ss2charnames.h"
#include "import_ss3charnames.h"
#include "import_ss4charnames.h"

extern C3D_RenderTarget* top;
extern C3D_RenderTarget* bottom;

extern void sndSelect(void);
extern void sndBack(void);
extern void sndHighlight(void);

// Current screen mode.
enum ScreenMode {
	SCREEN_MODE_ROCKETROBZ = 0,			// RocketRobz logo
	SCREEN_MODE_GAME_SELECT = 1,		// Game select
	SCREEN_MODE_WHAT_TO_DO = 2,			// What to do?
	SCREEN_MODE_CHANGE_CHARACTER = 3,	// Change character
};
//static int screenmode = 0;
extern int screenmodebuffer;

static int subScreenMode = 0;
/*
	0: Character list
	1: What to change
	2: Body change
	3: Outfit change (Currently bow placement change)
	4: Import character
*/

static int import_highlightedGame = 0;

static int seasonNo = 0;

static const char* seasonName(void) {
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

static char chrFilePath[256];

extern int highlightedGame;
extern bool saveWritten;

extern int fadealpha;
extern int fadecolor;
extern bool fadein;
extern bool fadeout;

extern float bg_xPos;
extern float bg_yPos;

extern bool showCursor;
extern int cursorX;
extern int cursorY;
extern int cursorAlpha;

extern void drawCursor(void);

extern u32 hDown;

static int cheatKeys[10] = {0};
static int cheatKeyPosition = 0;

static u16 totalCharacters = 0;
static u16 import_totalCharacters = 0;

static int characterList_cursorPosition = 0;
static int characterList_cursorPositionOnScreen = 0;

static int characterChangeMenu_cursorPosition = 0;
static int characterChangeMenu_cursorPositionOnScreen = 0;
static int characterChangeMenu_optionShownFirst = 0;
static int characterChangeMenuOps[4] = {0};
static int characterChangeMenuOptions = 2;

static int importCharacterList_cursorPosition = 0;
static int importCharacterList_cursorPositionOnScreen = 0;

static int characterShownFirst = 0;
static int import_characterShownFirst = 0;

static const char* characterName(bool showPlayerName) {
	if (characterList_cursorPosition == 0) {
		if (showPlayerName) {
			switch (highlightedGame) {
				case 1:
					return ss2PlayerName;
				case 2:
					return ss3PlayerName;
				case 3:
					return ss4PlayerName;
			}
		} else {
			return "Player";
		}
	}

	switch (highlightedGame) {
		case 2:
			return ss3CharacterNames[characterList_cursorPosition];
		case 3:
			return ss4CharacterNames[characterList_cursorPosition];
	}
	return "null";
}

static const char* import_characterName(void) {
	switch (import_highlightedGame) {
		case 0:
			return import_ss1CharacterNames[importCharacterList_cursorPosition];
		case 1:
			return import_ss2CharacterNames[importCharacterList_cursorPosition];
		case 2:
			return import_ss3CharacterNames[importCharacterList_cursorPosition];
		case 3:
			return import_ss4CharacterNames[importCharacterList_cursorPosition];
	}
	return "null";
}

static bool showMessage = false;
static int messageNo = 0;

static char chararacterImported[48];

static void drawMsg(void) {
	Gui::spriteScale(sprites_msg_idx, 0, 0, 2, 1);
	Gui::spriteScale(sprites_msg_idx, 160, 0, -2, 1);
	if (messageNo == 3) {
		Draw_Text(32, 84, 0.60, BLACK, "Exported character does not exist.");
	} else if (messageNo == 2) {
		Draw_Text(32, 48, 0.60, BLACK, "Character exported successfully.");
		Draw_Text(32, 84, 0.60, BLACK, "You can go to \"Import Characters\"");
		Draw_Text(32, 104, 0.60, BLACK, "and restore the exported character");
		Draw_Text(32, 124, 0.60, BLACK, "at any time.");
	} else if (messageNo == 1) {
		Draw_Text(32, 48, 0.60, BLACK, chararacterImported);
		Draw_Text(32, 84, 0.60, BLACK, "Please restore \"SavvyManager\"");
		Draw_Text(32, 104, 0.60, BLACK, "data for your game in Checkpoint,");
		Draw_Text(32, 124, 0.60, BLACK, "for the change to take effect.");
	} else {
		Draw_Text(32, 84, 0.60, BLACK, "This feature is not available yet.");
		//Draw_Text(32, 104, 0.60, BLACK, "yet.");
	}
	Draw_Text(32, 160, 0.65, BLACK, " OK");
}

void changeCharacter(void) {
	if (highlightedGame == 3) {
		characterChangeMenuOps[0] = 0;
		characterChangeMenuOps[1] = 0;
		characterChangeMenuOps[2] = 4;
		characterChangeMenuOps[3] = 10;
		characterChangeMenuOptions = 3;
		totalCharacters = 0x20;
		readSS4Save();
	} else if (highlightedGame == 2) {
		characterChangeMenuOps[0] = 0;
		characterChangeMenuOps[1] = 4;
		characterChangeMenuOps[2] = 10;
		characterChangeMenuOptions = 2;
		totalCharacters = 0xE;
		readSS3Save();
	} else if (highlightedGame == 1) {
		characterChangeMenuOps[0] = 0;
		characterChangeMenuOps[1] = 4;
		characterChangeMenuOps[2] = 10;
		characterChangeMenuOptions = 2;
		totalCharacters = 0;
		readSS2Save();
	}

	if (import_highlightedGame == 4) {
		import_totalCharacters = 0;
	} else if (import_highlightedGame == 3) {
		import_totalCharacters = 0xC;
	} else if (import_highlightedGame == 2) {
		import_totalCharacters = (highlightedGame==3 ? 0x10 : 0x11);
	} else if (import_highlightedGame == 1) {
		import_totalCharacters = 0xF;
	} else if (import_highlightedGame == 0) {
		import_totalCharacters = 0x7;
	}

	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(top, TRANSPARENT);
	C2D_TargetClear(bottom, TRANSPARENT);
	Gui::clearTextBufs();
	set_screen(top);

	Gui::sprite(sprites_blue_bg_idx, 0, 0);
	if (fadealpha > 0) Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect

	set_screen(bottom);
	Draw_Rect(0, 0, 320, 240, WHITE);	// Fill gaps of BG
	for(int w = 0; w < 7; w++) {
		for(int h = 0; h < 3; h++) {
			Gui::sprite(sprites_phone_bg_idx, -76+bg_xPos+w*72, bg_yPos+h*136);
		}
	}

	if (subScreenMode == 4) {
		cursorX = 256;
		cursorY = 64+(48*importCharacterList_cursorPositionOnScreen);

		// Game name
		switch (import_highlightedGame) {
			case 4:
				Draw_Text(32, 8, 0.50, BLACK, "Savvy Manager");
				break;
			case 3:
				Draw_Text(32, 8, 0.50, BLACK, "Style Savvy: Styling Star");
				break;
			case 2:
				Draw_Text(32, 8, 0.50, BLACK, "Style Savvy: Fashion Forward");
				break;
			case 1:
				Draw_Text(32, 8, 0.50, BLACK, "Style Savvy: Trendsetters");
				break;
			case 0:
				Draw_Text(32, 8, 0.50, BLACK, "Style Savvy");
				break;
		}
		Draw_Text(8, 8, 0.50, BLACK, "<");
		Draw_Text(304, 8, 0.50, BLACK, ">");

		if (import_highlightedGame != 4) {
			// Selected season
			Draw_Text(116, 208, 0.65, BLACK, "L");
			Draw_Text(132, 210, 0.50, BLACK, seasonName());
			Draw_Text(192, 208, 0.65, BLACK, "R");
		}

		int i2 = 48;
		for (int i = import_characterShownFirst; i < import_characterShownFirst+3; i++) {
			if (import_highlightedGame == 4) {
				//Gui::sprite((import_ss4CharacterGenders[i] ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
				Draw_Text(64, i2, 0.65, BLACK, "Exported character");
				break;
			} else if (import_highlightedGame == 3) {
				Gui::sprite((import_ss4CharacterGenders[i] ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
				Draw_Text(64, i2, 0.65, BLACK, import_ss4CharacterNames[i]);
			} else if (import_highlightedGame == 2) {
				Gui::sprite((import_ss3CharacterGenders[i] ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
				Draw_Text(64, i2, 0.65, BLACK, import_ss3CharacterNames[i]);
			} else if (import_highlightedGame == 1) {
				Gui::sprite((import_ss2CharacterGenders[i] ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
				Draw_Text(64, i2, 0.65, BLACK, import_ss2CharacterNames[i]);
			} else if (import_highlightedGame == 0) {
				Gui::sprite((import_ss1CharacterGenders[i] ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
				Draw_Text(64, i2, 0.65, BLACK, import_ss1CharacterNames[i]);
			}
			i2 += 48;
		}
	} else if (subScreenMode == 1) {
		cursorX = 256;
		cursorY = 64+(48*characterChangeMenu_cursorPositionOnScreen);

		Draw_Text(8, 8, 0.50, BLACK, characterName(true));

		int i2 = 0;
		if (characterChangeMenu_optionShownFirst == 0) {
			i2 += 48;
			Draw_Text(64, i2, 0.65, BLACK, "Change attributes");
		}
		if (highlightedGame == 3) {
			i2 += 48;
			Draw_Text(64, i2, 0.65, BLACK, "Change bow placement");
		}
		i2 += 48;
		Draw_Text(64, i2, 0.65, BLACK, "Import character");
		if (highlightedGame < 3 || characterChangeMenu_optionShownFirst == 1) {
			i2 += 48;
			Draw_Text(64, i2, 0.65, BLACK, "Export character");
		}
	} else {
		cursorX = 256;
		cursorY = 64+(48*characterList_cursorPositionOnScreen);

		Draw_Text(8, 8, 0.50, BLACK, "Select the character you want to change.");

		int i2 = 48;
		for (int i = characterShownFirst; i < characterShownFirst+3; i++) {
			if (highlightedGame == 3) {
				if (i==0) {
					Gui::sprite((getSS4CharacterGender(i) ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
					Draw_Text(64, i2, 0.65, BLACK, ss4PlayerName);
				} else {
					Gui::sprite((getSS4CharacterGender(i) ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
					Draw_Text(64, i2, 0.65, BLACK, ss4CharacterNames[i]);
				}
			} else if (highlightedGame == 2) {
				if (i==0) {
					Gui::sprite((getSS3CharacterGender(i) ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
					Draw_Text(64, i2, 0.65, BLACK, ss3PlayerName);
				} else {
					Gui::sprite((getSS3CharacterGender(i) ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
					Draw_Text(64, i2, 0.65, BLACK, ss3CharacterNames[i]);
				}
			} else if (highlightedGame == 1) {
				Gui::sprite((getSS2CharacterGender() ? sprites_icon_male_idx : sprites_icon_female_idx), 12, i2-8);
				Draw_Text(64, i2, 0.65, BLACK, ss2PlayerName);
				break;
			}
			i2 += 48;
		}
	}

	Gui::sprite(sprites_button_shadow_idx, 5, 199);
	Gui::sprite(sprites_button_red_idx, 5, 195);
	Gui::sprite(sprites_arrow_back_idx, 19, 195);
	Gui::sprite(sprites_button_b_idx, 44, 218);

	drawCursor();

	if (showMessage) {
		drawMsg();
	}

	if (fadealpha > 0) Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
	Draw_EndFrame();

	if (!fadein && !fadeout) {
		if (showMessage) {
			if (hDown & KEY_A) {
				sndSelect();
				showMessage = false;
			}
		} else if (subScreenMode == 4) {
			bool robzAction = false;
			if (hDown) {
				cheatKeys[cheatKeyPosition] = hDown;
				cheatKeyPosition++;
			}
			robzAction =  ((cheatKeys[0] & KEY_UP)
						&& (cheatKeys[1] & KEY_UP)
						&& (cheatKeys[2] & KEY_DOWN)
						&& (cheatKeys[3] & KEY_DOWN)
						&& (cheatKeys[4] & KEY_LEFT)
						&& (cheatKeys[5] & KEY_RIGHT)
						&& (cheatKeys[6] & KEY_LEFT)
						&& (cheatKeys[7] & KEY_RIGHT)
						&& (cheatKeys[8] & KEY_B)
						&& (cheatKeys[9] & KEY_A));
			if (cheatKeyPosition==10
			|| ((cheatKeys[0] != 0) && !(cheatKeys[0] & KEY_UP))
			|| ((cheatKeys[1] != 0) && !(cheatKeys[1] & KEY_UP))
			|| ((cheatKeys[2] != 0) && !(cheatKeys[2] & KEY_DOWN))
			|| ((cheatKeys[3] != 0) && !(cheatKeys[3] & KEY_DOWN))
			|| ((cheatKeys[4] != 0) && !(cheatKeys[4] & KEY_LEFT))
			|| ((cheatKeys[5] != 0) && !(cheatKeys[5] & KEY_RIGHT))
			|| ((cheatKeys[6] != 0) && !(cheatKeys[6] & KEY_LEFT))
			|| ((cheatKeys[7] != 0) && !(cheatKeys[7] & KEY_RIGHT))
			|| ((cheatKeys[8] != 0) && !(cheatKeys[8] & KEY_B))
			|| ((cheatKeys[9] != 0) && !(cheatKeys[9] & KEY_A))) {
				for (int i= 0; i < 10; i++) {
					cheatKeys[i] = 0;
				}
				cheatKeyPosition = 0;
			}
			if (showCursor) {
				if ((hDown & KEY_UP) && import_highlightedGame != 4) {
					sndHighlight();
					importCharacterList_cursorPosition--;
					importCharacterList_cursorPositionOnScreen--;
					if (importCharacterList_cursorPosition < 0) {
						importCharacterList_cursorPosition = 0;
						import_characterShownFirst = 0;
					} else if (importCharacterList_cursorPosition < import_characterShownFirst) {
						import_characterShownFirst--;
					}
					if (importCharacterList_cursorPositionOnScreen < 0) {
						importCharacterList_cursorPositionOnScreen = 0;
					}
				}
				if ((hDown & KEY_DOWN) && import_highlightedGame != 4) {
					sndHighlight();
					importCharacterList_cursorPosition++;
					importCharacterList_cursorPositionOnScreen++;
					if (importCharacterList_cursorPosition > import_totalCharacters) {
						importCharacterList_cursorPosition = import_totalCharacters;
						import_characterShownFirst = import_totalCharacters-2;
					} else if (importCharacterList_cursorPosition > import_characterShownFirst+2) {
						import_characterShownFirst++;
					}
					if (importCharacterList_cursorPositionOnScreen > 2) {
						importCharacterList_cursorPositionOnScreen = 2;
					}
				}
			}
			if (hDown & KEY_A) {
				if (robzAction) {
					sndSelect();
					switch (highlightedGame) {
						case 3:
							sprintf(chrFilePath, "romfs:/character/Styling Star/All Seasons/%s.chr", "Robz");
							if (access(chrFilePath, F_OK) != 0) {
								sprintf(chrFilePath, "romfs:/character/Styling Star/%s/%s.chr", seasonName(), "Robz");
							}
							readSS4CharacterFile(characterList_cursorPosition, chrFilePath);
							writeSS4Save();
							break;
						case 2:
							sprintf(chrFilePath, "romfs:/character/Fashion Forward/All Seasons/%s.chr", "Robz");
							if (access(chrFilePath, F_OK) != 0) {
								sprintf(chrFilePath, "romfs:/character/Fashion Forward/%s/%s.chr", seasonName(), "Robz");
							}
							readSS3CharacterFile(characterList_cursorPosition, chrFilePath);
							writeSS3Save();
							break;
						case 1:
							sprintf(chrFilePath, "romfs:/character/Trendsetters/All Seasons/%s.chr", "Robz");
							if (access(chrFilePath, F_OK) != 0) {
								sprintf(chrFilePath, "romfs:/character/Trendsetters/%s/%s.chr", seasonName(), "Robz");
							}
							readSS2CharacterFile(chrFilePath);
							writeSS2Save();
							break;
					}
					sprintf(chararacterImported, "Imported %s successfully.", "Robz");
					messageNo = 1;
					subScreenMode = 1;
					showMessage = true;
				} else if (import_highlightedGame == 4) {
					bool exportFound = false;
					switch (highlightedGame) {
						case 3:
							sprintf(chrFilePath, "sdmc:/3ds/SavvyManager/SS4/characters/%s.chr", characterName(false));
							if (access(chrFilePath, F_OK) == 0) {
								sndSelect();
								readSS4CharacterFile(characterList_cursorPosition, chrFilePath);
								writeSS4Save();
								exportFound = true;
							}
							break;
						case 2:
							sprintf(chrFilePath, "sdmc:/3ds/SavvyManager/SS3/characters/%s.chr", characterName(false));
							if (access(chrFilePath, F_OK) == 0) {
								sndSelect();
								readSS3CharacterFile(characterList_cursorPosition, chrFilePath);
								writeSS3Save();
								exportFound = true;
							}
							break;
						case 1:
							sprintf(chrFilePath, "sdmc:/3ds/SavvyManager/SS2/characters/%s.chr", characterName(false));
							if (access(chrFilePath, F_OK) == 0) {
								sndSelect();
								readSS2CharacterFile(chrFilePath);
								writeSS2Save();
								exportFound = true;
							}
							break;
					}
					if (exportFound) {
						sprintf(chararacterImported, "Imported %s successfully.", characterName(true));
						messageNo = 1;
						subScreenMode = 1;
					} else {
						sndBack();
						messageNo = 3;
					}
				} else {
				sndSelect();
				switch (highlightedGame) {
					case 3:
						sprintf(chrFilePath, "romfs:/character/Styling Star/All Seasons/%s.chr", import_characterName());
						if (access(chrFilePath, F_OK) != 0) {
							sprintf(chrFilePath, "romfs:/character/Styling Star/%s/%s.chr", seasonName(), import_characterName());
						}
						readSS4CharacterFile(characterList_cursorPosition, chrFilePath);
						writeSS4Save();
						break;
					case 2:
						sprintf(chrFilePath, "romfs:/character/Fashion Forward/All Seasons/%s.chr", import_characterName());
						if (access(chrFilePath, F_OK) != 0) {
							sprintf(chrFilePath, "romfs:/character/Fashion Forward/%s/%s.chr", seasonName(), import_characterName());
						}
						readSS3CharacterFile(characterList_cursorPosition, chrFilePath);
						writeSS3Save();
						break;
					case 1:
						sprintf(chrFilePath, "romfs:/character/Trendsetters/All Seasons/%s.chr", import_characterName());
						if (access(chrFilePath, F_OK) != 0) {
							sprintf(chrFilePath, "romfs:/character/Trendsetters/%s/%s.chr", seasonName(), import_characterName());
						}
						readSS2CharacterFile(chrFilePath);
						writeSS2Save();
						break;
				}
				sprintf(chararacterImported, "Imported %s successfully.", import_characterName());
				messageNo = 1;
				subScreenMode = 1;
				}
				showMessage = true;
			}
			if (hDown & KEY_LEFT) {
				sndHighlight();
				import_highlightedGame--;
				if (import_highlightedGame < 0) import_highlightedGame = 4;
				importCharacterList_cursorPosition = 0;
				importCharacterList_cursorPositionOnScreen = 0;
				import_characterShownFirst = 0;
			}
			if (hDown & KEY_RIGHT) {
				sndHighlight();
				import_highlightedGame++;
				if (import_highlightedGame > 4) import_highlightedGame = 0;
				importCharacterList_cursorPosition = 0;
				importCharacterList_cursorPositionOnScreen = 0;
				import_characterShownFirst = 0;
			}
			if (import_highlightedGame != 4) {
				if (hDown & KEY_L) {
					sndHighlight();
					seasonNo--;
					if (seasonNo < 0) seasonNo = 3;
				}
				if (hDown & KEY_R) {
					sndHighlight();
					seasonNo++;
					if (seasonNo > 3) seasonNo = 0;
				}
			}
			if ((hDown & KEY_B) && !(cheatKeys[8] & KEY_B)) {
				sndBack();
				subScreenMode = 1;
			}
		} else if (subScreenMode == 1) {
			if (showCursor) {
				if (hDown & KEY_UP) {
					sndHighlight();
					characterChangeMenu_cursorPosition--;
					characterChangeMenu_cursorPositionOnScreen--;
					if (characterChangeMenu_cursorPosition < 0) {
						characterChangeMenu_cursorPosition = 0;
						characterChangeMenu_optionShownFirst = 0;
					} else if (characterChangeMenu_cursorPosition < characterChangeMenu_optionShownFirst) {
						characterChangeMenu_optionShownFirst--;
					}
					if (characterChangeMenu_cursorPositionOnScreen < 0) {
						characterChangeMenu_cursorPositionOnScreen = 0;
					}
				}
				if (hDown & KEY_DOWN) {
					sndHighlight();
					characterChangeMenu_cursorPosition++;
					characterChangeMenu_cursorPositionOnScreen++;
					if (characterChangeMenu_cursorPosition > characterChangeMenuOptions) {
						characterChangeMenu_cursorPosition = characterChangeMenuOptions;
						characterChangeMenu_optionShownFirst = characterChangeMenuOptions-2;
					} else if (characterChangeMenu_cursorPosition > characterChangeMenu_optionShownFirst+2) {
						characterChangeMenu_optionShownFirst++;
					}
					if (characterChangeMenu_cursorPositionOnScreen > 2) {
						characterChangeMenu_cursorPositionOnScreen = 2;
					}
				}
			}
			if (hDown & KEY_A) {
				if (characterChangeMenuOps[characterChangeMenu_cursorPosition] == 0) {
					sndBack();
					messageNo = 0;
					showMessage = true;
				} else if (characterChangeMenuOps[characterChangeMenu_cursorPosition] == 10) {
					sndSelect();
					switch (highlightedGame) {
						case 3:
							sprintf(chrFilePath, "sdmc:/3ds/SavvyManager/SS4/characters/%s.chr", characterName(false));
							writeSS4CharacterFile(characterList_cursorPosition, chrFilePath);
							break;
						case 2:
							sprintf(chrFilePath, "sdmc:/3ds/SavvyManager/SS3/characters/%s.chr", characterName(false));
							writeSS3CharacterFile(characterList_cursorPosition, chrFilePath);
							break;
						case 1:
							sprintf(chrFilePath, "sdmc:/3ds/SavvyManager/SS2/characters/%s.chr", characterName(false));
							writeSS2CharacterFile(chrFilePath);
							break;
					}
					messageNo = 2;
					showMessage = true;
				} else {
					sndSelect();
					subScreenMode = characterChangeMenuOps[characterChangeMenu_cursorPosition];
				}
			}
			if (hDown & KEY_B) {
				sndBack();
				subScreenMode = 0;
			}
		} else {
			if (showCursor) {
				if ((hDown & KEY_UP) && (highlightedGame > 1)) {
					sndHighlight();
					characterList_cursorPosition--;
					characterList_cursorPositionOnScreen--;
					if (characterList_cursorPosition < 0) {
						characterList_cursorPosition = 0;
						characterShownFirst = 0;
					} else if (characterList_cursorPosition < characterShownFirst) {
						characterShownFirst--;
					}
					if (characterList_cursorPositionOnScreen < 0) {
						characterList_cursorPositionOnScreen = 0;
					}
				}
				if ((hDown & KEY_DOWN) && (highlightedGame > 1)) {
					sndHighlight();
					characterList_cursorPosition++;
					characterList_cursorPositionOnScreen++;
					if (characterList_cursorPosition > totalCharacters) {
						characterList_cursorPosition = totalCharacters;
						characterShownFirst = totalCharacters-2;
					} else if (characterList_cursorPosition > characterShownFirst+2) {
						characterShownFirst++;
					}
					if (characterList_cursorPositionOnScreen > 2) {
						characterList_cursorPositionOnScreen = 2;
					}
				}
			}
			if (hDown & KEY_A) {
				sndSelect();
				subScreenMode = 1;
			}
			if (hDown & KEY_B) {
				sndBack();
				characterList_cursorPosition = 0;
				characterList_cursorPositionOnScreen = 0;
				characterShownFirst = 0;
				characterChangeMenu_cursorPosition = 0;
				characterChangeMenu_cursorPositionOnScreen = 0;
				characterChangeMenu_optionShownFirst = 0;
				screenmodebuffer = SCREEN_MODE_WHAT_TO_DO;
				fadeout = true;
			}
		}
	}
}
