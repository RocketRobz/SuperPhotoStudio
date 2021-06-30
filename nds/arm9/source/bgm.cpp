#include "common.hpp"
#include "sound.h"

#include <unistd.h>

void loadMusic(int num) {
	const char* startName = "";
	const char* loopName = "";
	u32 sampleRate = 0;

	switch (num) {
		case 0:	// Pokemon Stadium - Stadium Select
		default:
			startName = "nitro:/music/pkmnStadium/stadiumSel_start.raw";
			loopName = "nitro:/music/pkmnStadium/stadiumSel_loop.raw";
			sampleRate = 32000;
			break;
		case 1:	// MegaMan: Battle and Chase - The Straight Out
			startName = "nitro:/music/mmBattleAndChase/straightOut_start.raw";
			loopName = "nitro:/music/mmBattleAndChase/straightOut_loop.raw";
			sampleRate = 44100;
			break;
		case 2:	// Mario Golf: Advance Tour - Marion Club
			startName = "nitro:/music/marioGolfAdvanceTour/marionClub_start.raw";
			loopName = "nitro:/music/marioGolfAdvanceTour/marionClub_loop.raw";
			sampleRate = 48000;
			break;
		case 3:	// Tetris Party - Title Screen
			startName = "nitro:/music/tetrisParty/titleScreen_start.raw";
			loopName = "nitro:/music/tetrisParty/titleScreen_loop.raw";
			sampleRate = 48000;
			break;
		case 4:	// Tetris Party - Main Menu
			loopName = "nitro:/music/tetrisParty/mainMenu.raw";
			sampleRate = 48000;
			break;
		case 5:	// Sonic Mania - Main Menu: "Comfort Zone"
			startName = "nitro:/music/sonicMania/mainMenu_start.raw";
			loopName = "nitro:/music/sonicMania/mainMenu_loop.raw";
			sampleRate = 44100;
			break;
		case 6:	// Sonic Mania - Angel Island Zone
			loopName = "nitro:/music/sonicMania/angelIslandZone.raw";
			sampleRate = 44100;
			break;
		case 7:	// Sonic Mania - Green Hill Zone: Act 1
			startName = "nitro:/music/sonicMania/greenHillZoneAct1_start.raw";
			loopName = "nitro:/music/sonicMania/greenHillZoneAct1_loop.raw";
			sampleRate = 44100;
			break;
		case 8:	// Sonic Mania - Green Hill Zone: Act 2
			startName = "nitro:/music/sonicMania/greenHillZoneAct2_start.raw";
			loopName = "nitro:/music/sonicMania/greenHillZoneAct2_loop.raw";
			sampleRate = 44100;
			break;
		case 9:	// Sonic Mania - Studiopolis Zone: Act 1
			startName = "nitro:/music/sonicMania/studiopolisZoneAct1_start.raw";
			loopName = "nitro:/music/sonicMania/studiopolisZoneAct1_loop.raw";
			sampleRate = 44100;
			break;
		case 10:	// Sonic Mania - Special Stage: "Dimension Heist"
			startName = "nitro:/music/sonicMania/specialStage_start.raw";
			loopName = "nitro:/music/sonicMania/specialStage_loop.raw";
			sampleRate = 44100;
			break;
	}
	snd().loadStream(startName, loopName, sampleRate, true);
}
