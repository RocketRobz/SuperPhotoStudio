/*
*   This file is part of Universal-Core
*   Copyright (C) 2020 DeadPhoenix8091, Epicpkmn11, Flame, RocketRobz, StackZ, TotallyNotGuy
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
*       * Requiring preservation of specified reasonable legal notices or
*         author attributions in that material or in the Appropriate Legal
*         Notices displayed by works containing it.
*       * Prohibiting misrepresentation of the origin of that material,
*         or requiring that modified versions of such material be marked in
*         reasonable ways as different from the original version.
*/

#include "gui.hpp"
#include "screenCommon.hpp"

#include <nds.h>
#include <unistd.h>

std::unique_ptr<Screen> usedScreen, tempScreen; // tempScreen used for "fade" effects.
bool currentScreen = false;
bool fadeout = false;
bool fadein = false;
int fadealpha = 0;
int fadecolor = 0;

// Clear Text.
void Gui::clearTextBufs(void) {  }

// Draw a sprite from the sheet.
void Gui::DrawSprite(int x, int y, float ScaleX, float ScaleY) {
}

// Initialize GUI.
void Gui::init(void) {
}

// Load a Font.
void Gui::loadFont(const char* Path) {
}

// Unload a Font.
void Gui::unloadFont() {
}

// Load a Sheet.
void Gui::loadSheet(const char* Path) {
}

// Unload a Sheet.
void Gui::unloadSheet() {
}

// Exit the GUI.
void Gui::exit(void) {
	if (usedScreen != nullptr)	usedScreen = nullptr;
}

// Draw a Centered String.
void Gui::DrawStringCentered(float x, float y, float size, u32 color, std::string Text, int maxWidth, int maxHeight) {
}

// Draw String or Text.
void Gui::DrawString(float x, float y, float size, u32 color, std::string Text, int maxWidth, int maxHeight) {
}

// Get String or Text Width.
float Gui::GetStringWidth(float size, std::string Text) {
}

// Get String or Text Size.
void Gui::GetStringSize(float size, float *width, float *height, std::string Text) {
}


// Get String or Text Height.
float Gui::GetStringHeight(float size, std::string Text) {
}

// Draw a Rectangle.
bool Gui::Draw_Rect(float x, float y, float w, float h, u32 color) {
}

// Draw's the current screen's draw.
void Gui::DrawScreen() {
	if (usedScreen != nullptr)	usedScreen->Draw();
}

// Do the current screen's logic.
void Gui::ScreenLogic(u32 hDown, u32 hHeld, touchPosition touch, bool waitFade) {
	if (waitFade) {
		if (!fadein && !fadeout) {
			if (usedScreen != nullptr)	usedScreen->Logic(hDown, hHeld, touch);
		}
	} else {
		if (usedScreen != nullptr)	usedScreen->Logic(hDown, hHeld, touch);
	}
}

// Move's the tempScreen to the used one.
void Gui::transferScreen() {
	if (tempScreen != nullptr)	usedScreen = std::move(tempScreen);
}

// Set the current Screen.
void Gui::setScreen(std::unique_ptr<Screen> screen, bool fade) { 
	tempScreen = std::move(screen);
	// Switch screen without fade.
	if (!fade) {
		Gui::transferScreen();
	} else {
		// Fade, then switch.
		fadeout = true;
	}
}

// Fade's the screen in and out and transfer the screen.
// Credits goes to RocketRobz & SavvyManager.
void Gui::fadeEffects(int fadeoutFrames, int fadeinFrames) {
	if (fadein) {
		fadealpha -= fadeinFrames;
		if (fadealpha < 0) {
			fadealpha = 0;
			fadecolor = 255;
			fadein = false;
		}
	}

	if (fadeout) {
		fadealpha += fadeoutFrames;
		if (fadealpha > 255) {
			fadealpha = 255;
			Gui::transferScreen(); // Transfer Temp screen to the used one.
			fadein = true;
			fadeout = false;
		}
	}
}

// Select, on which Screen should be drawn.
void Gui::ScreenDraw() {
}

void Gui::drawGrid(float xPos, float yPos, float Width, float Height, u32 color) {
}

void Gui::drawAnimatedSelector(float xPos, float yPos, float Width, float Height, float speed, u32 SelectorColor, u32 bgColor) {
}