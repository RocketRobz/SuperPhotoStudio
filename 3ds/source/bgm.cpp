#include "common.hpp"
#include "sound.h"

#include <unistd.h>

extern sound *music;
extern sound *music_loop;
extern bool dspfirmfound;

void loadMusic(int num) {
	if (!dspfirmfound) return;

	switch (num) {
		case 0:	// Pokemon Stadium - Stadium Select
		default:
			music = new sound("romfs:/music/pkmnStadium/stadiumSel_start.wav", 0, false);
			music_loop = new sound("romfs:/music/pkmnStadium/stadiumSel_loop.wav", 1, true);
			break;
		case 1:	// MegaMan: Battle and Chase - The Straight Out
			music = new sound("romfs:/music/mmBattleAndChase/straightOut_start.wav", 0, false);
			music_loop = new sound("romfs:/music/mmBattleAndChase/straightOut_loop.wav", 1, true);
			break;
		case 2:	// Mario Golf: Advance Tour - Marion Club
			music = new sound("romfs:/music/marioGolfAdvanceTour/marionClub_start.wav", 0, false);
			music_loop = new sound("romfs:/music/marioGolfAdvanceTour/marionClub_loop.wav", 1, true);
			break;
		case 3:	// Tetris Party - Title Screen
			music = new sound("romfs:/music/tetrisParty/titleScreen_start.wav", 0, false);
			music_loop = new sound("romfs:/music/tetrisParty/titleScreen_loop.wav", 1, true);
			break;
		case 4:	// Tetris Party - Main Menu
			music_loop = new sound("romfs:/music/tetrisParty/mainMenu.wav", 1, true);
			break;
	}
}
