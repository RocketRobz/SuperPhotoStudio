#include "common.hpp"
#include "sound.h"

#include <unistd.h>

extern sound *music;
extern sound *music_loop;

static bool music_loaded = false;
static bool music_loop_loaded = false;

void loadMusic(int num) {
	if (music_loaded) {
		music->stop();
		delete music;
	}
	if (music_loop_loaded) {
		music_loop->stop();
		delete music_loop;
	}
	switch (num) {
		case 0:
		default:
			music = new sound("romfs:/music/pkmnStadium/stadiumSel_start.wav", 0, false);
			music_loop = new sound("romfs:/music/pkmnStadium/stadiumSel_loop.wav", 1, true);
			break;
	}
	music_loaded = true;
	music_loop_loaded = true;
}
