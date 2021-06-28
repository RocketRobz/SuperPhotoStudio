#include "common.hpp"
#include "sound.h"

#include <unistd.h>

void loadMusic(int num) {
	switch (num) {
		case 0:
		default:
			snd().loadStream("nitro:/music/pkmnStadium/stadiumSel_start.raw", "nitro:/music/pkmnStadium/stadiumSel_loop.raw", 22050, true);
			break;
	}
}
