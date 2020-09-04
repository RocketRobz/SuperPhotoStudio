#ifndef _SAVVY_MANAGER_PHOTO_STUDIO_HPP
#define _SAVVY_MANAGER_PHOTO_STUDIO_HPP

#include "common.hpp"

#include <memory>
#include <vector>

class PhotoStudio : public Screen {
public:
	void Draw(void) const override;
	void SettingsDraw(void) const;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
	void SettingsLogic(u32 hDown, u32 hHeld, touchPosition touch);
	PhotoStudio();
private:
	#ifdef NDS
	bool graphicLoaded = false;
	#endif
	void preview() const;
	void getList();
	void getMaxChars();
	const char* seasonName(void) const;
	const char* import_characterName(void) const;
	const char* import_characterFileName(void) const;
	const char* import_SS2CharacterNames(int i) const;
	const char* NESCharacterNames(int i) const;
	const char* ss1Title(void) const;
	const char* ss2Title(void) const;
	const char* ss3Title(void) const;
	const char* ss4Title(void) const;
	const char* nesTitle(void) const;
	const char* bgGameTitle(void) const;
	const char* charGameTitle(void) const;
	bool charGender(int i) const;
	const char* bgName(int i) const;
	const char* charName(int i) const;
	int getBgNum(void) const;
	void drawMsg(void) const;
	void loadChrImage(void);

	int subScreenMode = 0;
	/*
		0: What to change
		1: Studio BG list
		2: Character list
		10: Settings
	*/

	mutable int cursorX, cursorY;
	int photo_highlightedGame = 0;
	int char_highlightedGame[5] = {0};
	int seasonNo[5] = {0};
	int currentCharNum = 0;
	int charsShown = 0;

	char chrFilePath[256];
	char chrFilePath2[256];
	char chrCounter[24];

	bool displayNothing = false;
	bool displayStudioBg = false;
	bool showScrollingBg = true;
	bool characterPicked[5] = {false};
	bool characterFlipH[5] = {false};

	mutable int charFadeAlpha = 0;
	bool previewCharacter = false;
	bool previewCharacterFound[5] = {false};

	bool showMessage = false;
	int messageNo = 0;
	int zoomIn = 0;

	u16 import_totalCharacters = 0;

	int characterChangeMenu_cursorPosition = 0;
	int characterChangeMenu_cursorPositionOnScreen = 0;
	int characterChangeMenu_optionShownFirst = 0;
	int characterChangeMenuOps[4] = {0};
	int characterChangeMenuOptions = 2;

	int importCharacterList_cursorPosition[5] = {0};
	int importCharacterList_cursorPositionOnScreen[5] = {0};

	int bgList_cursorPosition = 0;
	int bgList_cursorPositionOnScreen = 0;

	int import_characterShownFirst[5] = {0};

	int import_bgShownFirst = 0;

	int numberOfSettings = 1;
	int settings_cursorPosition = 0;
	int settings_cursorPositionOnScreen = 0;
	int settingShownFirst = 0;

	bool exportedCharListGotten = false;
};

#endif