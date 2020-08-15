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

#ifndef _UNIVERSAL_CORE_GUI_HPP
#define _UNIVERSAL_CORE_GUI_HPP

#include "screen.hpp"

#include <nds.h>

namespace Gui {
	// Clear the Text Buffer.
	void clearTextBufs(void);

	/*	Draw a sprite from a SpriteSheet.
	*	imgIndex: The index of the sprite from the sheet which should be drawn.
	*	x: The X Position where the sprite should be drawn.
	*	y: The Y Position where the sprite should be drawn.
	*/
	void DrawSprite(int imgindex, int x, int y);

	// Initialize the GUI with Citro2D & Citro3D and initialize the Textbuffer. (call this when initializing.)
	void init(void);

	/*	Load a Font. (BCFNT)
	*	fnt: The C2D_Font variable which should be initialized.
	*	Path: Path to the BCFNT file.
	*	if you're unsure, just call 'Gui::init();' and it will load the system font.
	*/
	void loadFont(const char * Path = "");

	/*	Unload a Font. (BCFNT)
	*	fnt: The C2D_Font variable which should be unloaded.
	*/
	void unloadFont();

	/*	Load a spritesheet.
	*	Path: Path to the SpriteSheet file. (T3X)
	*	sheet: Reference to the C2D_SpriteSheet declaration.
	*/
	void loadSheet(const char* Path);

	/*	Unload a spritesheet.
	*	sheet: Reference to the C2D_SpriteSheet which should be free'd.
	*/
	void unloadSheet();

	// Exit the GUI. (Call this at exit.)
	void exit(void);
	
	/*	Draws a centered String.
	*	x: The X Offset from center. (Center: 200 px on top, 160 px on Bottom.)
	*	y: The Y Position of the Text.
	*	size: The size of the Text.
	*	color: The Color of the Text.
	*	Text: The Text which should be displayed.
	*	maxWidth: The maxWidth for the Text. (Optional!)
	*	maxHeight: The maxHeight of the Text. (Optional!)
	*	fnt: The Font which should be used. Uses SystemFont by default. (Optional!)
	*/
	void DrawStringCentered(float x, float y, float size, u32 color, std::string Text, int maxWidth = 0, int maxHeight = 0);

	/*	Draws a String.
	*	x: The X Position where the Text should be drawn.
	*	y: The Y Position where the Text should be drawn.
	*	size: The size of the Text.
	*	color: The Color of the Text.
	*	Text: The Text which should be displayed.
	*	maxWidth: The maxWidth for the Text. (Optional!)
	*	maxHeight: The maxHeight of the Text. (Optional!)
	*	fnt: The Font which should be used. Uses SystemFont by default. (Optional!)
	*/
	void DrawString(float x, float y, float size, u32 color, std::string Text, int maxWidth = 0, int maxHeight = 0);

	/*	Get the width of a String.
	*	size: The size of the Text.
	*	Text: The Text where the width should be getted from.
	*	fnt: The Font which should be used. Uses SystemFont by default. (Optional!)
	*/
	float GetStringWidth(float size, std::string Text);

	/*	Get the size of a String.
	*	size: The size of the Text.
	*	width: The width of the Text.
	*	height: The height of the Text.
	*	Text: The Text where the size should be getted from.
	*	fnt: The Font which should be used. Uses SystemFont by default. (Optional!)
	*/
	void GetStringSize(float size, float *width, float *height, std::string Text);

	/*	Get the height of a String.
	*	size: The size of the Text.
	*	Text: The Text where the height should be getted from.
	*	fnt: The Font which should be used. Uses SystemFont by default. (Optional!)
	*/
	float GetStringHeight(float size, std::string Text);

	/*	Draw a Rectangle.
	*	x: X Position of the Rectangle.
	*	y: Y Position of the Rectangle.
	*	w: The width of the rectangle.
	*	h: The height of the rectangle.
	*	color: The color of the rectangle.
	*/
	bool Draw_Rect(int x, int y, int w, int h, int color);
	
	//	Used for the current Screen's Draw. (Optional!)
	void DrawScreen();

	/*	Used for the current Screen's Logic. (Optional!)
	*	hDown: the hidKeysDown() variable.
	*	hHeld: the HidKeysHeld() variable.
	*	touch: The TouchPosition variable.
	*	waitFade: Wheter to wait until the fade ends.
	*/
	void ScreenLogic(u32 hDown, u32 hHeld, touchPosition touch, bool waitFade = true);
	
	/* Transfer the Temp Screen to the used one. (Optional!)
	*	It will check, if the tempScreen variable is not nullptr, so don't worry.
	*/
	void transferScreen();

	/*	Set a specific Screen with switch function. (Optional!)
	*	screen: unique_ptr of the screen. (Optional by using the screen class.)
	*	screenSwitch: Wheter to switch to the current screen.
	*/
	void setScreen(std::unique_ptr<Screen> screen, bool fade = false);

	/* Fades into screens and calls the constructor after it. (Optional!)
	*	fadeoutFrames: Amount of frames for fadeout.
	*	fadeinFrames: Amount of frames for fadein.
	*/
	void fadeEffects(int fadeoutFrames = 6, int fadeinFrames = 6);

	/*	Set on which screen to draw.
	*	screen: The render target. (Targets are inside the screenCommon.hpp file.)
	*/
	void ScreenDraw();

	/*	Draws a grid.
	*	xPos: X Position of the grid.
	*	yPos: Y Position of the grid.
	*	Width: Width of the grid.
	*	Height: Height of the grid.
	*	color: Color of the grid.
	*/
	void drawGrid(float xPos, float yPos, float Width, float Height, u32 color);

	/*	Draws an animated selector.
	*	xPos: X Position of the selector.
	*	yPos: Y Position of the Selector.
	*	Width: Width of the Selector.
	*	Height: Height of the Selector.
	*	speed: The speed of the animation. (Use .030 or something by default.)
	*	SelectorColor: The Color of the Selector.
	*	bgColor: The Color from the middle of the Selector. (Optional! It's transparent by default.)
	*/
	void drawAnimatedSelector(float xPos, float yPos, float Width, float Height, float speed, u32 SelectorColor, u32 bgColor = 0);
}

#endif