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

#include "sprites.h"
#include "cursor.h"

#include <nds.h>
#include <gl2d.h>
#include <unistd.h>

std::unique_ptr<Screen> usedScreen, tempScreen; // tempScreen used for "fade" effects.
bool currentScreen = false;
bool fadeout = false;
bool fadein = false;
int fadealpha = 0;
int fadecolor = 0;

int bg3Main;
int bg2Main;
int bg3Sub;

static int cursorTexID;
static glImage cursorImage[(32 / 32) * (32 / 32)];

// Ported from PAlib (obsolete)
void SetBrightness(u8 screen, s8 bright) {
	u16 mode = 1 << 14;

	if (bright < 0) {
		mode = 2 << 14;
		bright = -bright;
	}
	if (bright > 31)
		bright = 31;
	*(u16 *)(0x0400006C + (0x1000 * screen)) = bright + mode;
}

// Clear Text.
void Gui::clearTextBufs(void) {  }

// Draw a sprite from the sheet.
void Gui::DrawSprite(int imgindex, int x, int y) {
	switch (imgindex) {
		case sprites_cursor_idx:
			glSprite(x, y, GL_FLIP_NONE, cursorImage);
			break;
	}
}

// Initialize GUI.
void Gui::init(void) {
	//////////////////////////////////////////////////////////
	videoSetMode(MODE_5_3D | DISPLAY_BG3_ACTIVE);
	videoSetModeSub(MODE_3_2D | DISPLAY_BG3_ACTIVE);

	// Initialize gl2d
	glScreen2D();
	// Make gl2d render on transparent stage.
	glClearColor(31, 31, 31, 0);
	glDisable(GL_CLEAR_BMP);

	// Clear the GL texture state
	glResetTextures();

	// Set up enough texture memory for our textures
	// Bank A is just 128kb and we are using 194 kb of
	// sprites
	vramSetBankA(VRAM_A_TEXTURE);
	vramSetBankB(VRAM_B_MAIN_BG_0x06020000);
	vramSetBankC(VRAM_C_SUB_BG_0x06200000);
	vramSetBankD(VRAM_D_MAIN_BG_0x06000000);
	vramSetBankE(VRAM_E_TEX_PALETTE);
	vramSetBankF(VRAM_F_TEX_PALETTE_SLOT4);
	vramSetBankG(VRAM_G_MAIN_SPRITE);
	vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE);
	vramSetBankI(VRAM_I_SUB_SPRITE_EXT_PALETTE);

	//	vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE); // Not sure this does anything...
	lcdMainOnBottom();

	bg3Main = bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
	bgSetPriority(bg3Main, 3);

	bg2Main = bgInit(2, BgType_Bmp8, BgSize_B8_256x256, 6, 0);
	//nocashMessage(std::to_string(bg2Main).c_str());
	bgSetPriority(bg2Main, 0);

	bg3Sub = bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
	bgSetPriority(bg3Sub, 3);

	bgSetPriority(0, 1); // Set 3D to below text

	cursorTexID = glLoadTileSet(cursorImage, // pointer to glImage array
							32, // sprite width
							32, // sprite height
							32, // bitmap width
							32, // bitmap height
							GL_RGB16, // texture type for glTexImage2D() in videoGL.h
							TEXTURE_SIZE_32, // sizeX for glTexImage2D() in videoGL.h
							TEXTURE_SIZE_32, // sizeY for glTexImage2D() in videoGL.h
							TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
							16, // Length of the palette to use (16 colors)
							(u16*) cursorPal, // Load our 16 color tiles palette
							(u8*) cursorBitmap // image data generated by GRIT
							);

	irqSet(IRQ_VBLANK, DrawScreen);
	irqEnable(IRQ_VBLANK);
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
bool Gui::Draw_Rect(int x, int y, int w, int h, int color) {
	glBoxFilled(x, y, x+w, y+h, color);
}

// Draw's the current screen's draw.
void Gui::DrawScreen() {
	SetBrightness(0, (fadecolor==255 ? fadealpha : -fadealpha)/8);
	SetBrightness(1, (fadecolor==255 ? fadealpha : -fadealpha)/8);
	glBegin2D();

	if (usedScreen != nullptr)	usedScreen->Draw();

	glEnd2D();
	GFX_FLUSH = 0;
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