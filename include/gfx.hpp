#ifndef GFX_HPP
#define GFX_HPP

#ifdef NDS

#include <nds.h>

// Colors.
#define TRANSPARENT RGB15(0, 0, 0, 0)
#define BLACK RGB15(0, 0, 0)
#define WHITE RGB15(255/8, 255/8, 255/8)
#define GRAY RGB15(127/8, 127/8, 127/8)
#define BLUE RGB15(0, 0, 255/8)
#define GREEN RGB15(0, 255/8, 0)
#define RED RGB15(255/8, 0, 0)
#define LIGHTRED RGB15(222/8, 110/8, 110/8)
#define MSG_BUTTONTEXT RGB15(181/8, 48/8, 0)

#define TIME RGB15(16/8, 0, 0)

#else

#include "sprites.h"

#include <3ds.h>
#include <citro2d.h>

// Colors.
#define TRANSPARENT C2D_Color32(0, 0, 0, 0)
#define BLACK C2D_Color32(0, 0, 0, 255)
#define WHITE C2D_Color32(255, 255, 255, 255)
#define GRAY C2D_Color32(127, 127, 127, 255)
#define BLUE C2D_Color32(0, 0, 255, 255)
#define GREEN C2D_Color32(0, 255, 0, 255)
#define RED C2D_Color32(255, 0, 0, 255)
#define LIGHTRED C2D_Color32(222, 110, 110, 255)
#define MSG_BUTTONTEXT C2D_Color32(181, 48, 0, 255)

#define TIME C2D_Color32(16, 0, 0, 223)

#endif

typedef u32 Color;

extern bool animateBg;
extern bool bgCanAnimate;

extern bool shiftBySubPixel;

namespace GFX {
	// Load & Unload default sheets.
	void resetCharStatus(int num = -1);
	#ifdef _3DS
	Result loadSheets();
	Result unloadSheets();
	#else
	void loadSheets();
	void unloadSheets();
	#endif
	void loadGameSelSheets();
	void unloadGameSelSheets();

	// Sprite Functions.
	bool loadCharSprite(int num, const char* t3xPathAllSeasons, const char* t3xPathOneSeason);
	#ifdef _3DS
	void loadCharSpriteMem(int num);
	#else
	void loadCharSpriteMem(int zoomIn, bool* flipH);
	#endif
	void loadBgSprite(void);
	void unloadBgSprite(void);
	void reloadBgSprite(void);
	void showBgSprite(int zoomIn);
	void animateBgSprite(void);
	void showCharSprite(int num, bool flipH, int zoomIn, int fadeAlpha, bool lightingEffects = false);
	void DrawSprite(int img, int x, int y, float ScaleX = 1, float ScaleY = 1);
	void DrawSpriteLinear(int img, int x, int y, float ScaleX = 1, float ScaleY = 1);
	#ifdef _3DS
	void DrawSpriteBlend(int img, float x, float y, u32 color, float ScaleX = 1, float ScaleY = 1, GPU_TEXTURE_FILTER_PARAM filter = GPU_NEAREST);
	#else
	void DrawSpriteBlend(int img, float x, float y, u32 color, float ScaleX = 1, float ScaleY = 1);
	#endif

	// Draw Cursor.
	void drawCursor(int cX, int cY);
}

#endif