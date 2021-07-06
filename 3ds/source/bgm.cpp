#include "common.hpp"
#include "sound.h"

#include <unistd.h>

extern sound *music;
extern sound *music_loop;
extern bool dspfirmfound;

void loadMusic(int num) {
	if (!dspfirmfound) return;

	const char* startName = "";
	const char* loopName = "";

	switch (num) {
		case 0:	// Pokemon Stadium - Stadium Select
		default:
			startName = "romfs:/music/pkmnStadium/stadiumSel_start.wav";
			loopName = "romfs:/music/pkmnStadium/stadiumSel_loop.wav";
			break;
		case 1:	// MegaMan: Battle and Chase - The Straight Out
			startName = "romfs:/music/mmBattleAndChase/straightOut_start.wav";
			loopName = "romfs:/music/mmBattleAndChase/straightOut_loop.wav";
			break;
		case 2:	// Mario Golf: Advance Tour - Marion Club
			startName = "romfs:/music/marioGolfAdvanceTour/marionClub_start.wav";
			loopName = "romfs:/music/marioGolfAdvanceTour/marionClub_loop.wav";
			break;
		case 3:	// Tetris Party - Title Screen
			startName = "romfs:/music/tetrisParty/titleScreen_start.wav";
			loopName = "romfs:/music/tetrisParty/titleScreen_loop.wav";
			break;
		case 4:	// Tetris Party - Main Menu
			loopName = "romfs:/music/tetrisParty/mainMenu.wav";
			break;
		case 5:	// Sonic Mania - Main Menu: "Comfort Zone"
			startName = "romfs:/music/sonicMania/mainMenu_start.wav";
			loopName = "romfs:/music/sonicMania/mainMenu_loop.wav";
			break;
		case 6:	// Sonic Mania - Angel Island Zone
			loopName = "romfs:/music/sonicMania/angelIslandZone.wav";
			break;
		case 7:	// Sonic Mania - Green Hill Zone: Act 1
			startName = "romfs:/music/sonicMania/greenHillZoneAct1_start.wav";
			loopName = "romfs:/music/sonicMania/greenHillZoneAct1_loop.wav";
			break;
		case 8:	// Sonic Mania - Green Hill Zone: Act 2
			startName = "romfs:/music/sonicMania/greenHillZoneAct2_start.wav";
			loopName = "romfs:/music/sonicMania/greenHillZoneAct2_loop.wav";
			break;
		case 9:	// Sonic Mania - Studiopolis Zone: Act 1
			startName = "romfs:/music/sonicMania/studiopolisZoneAct1_start.wav";
			loopName = "romfs:/music/sonicMania/studiopolisZoneAct1_loop.wav";
			break;
		case 10: // Sonic Mania - Special Stage: "Dimension Heist"
			startName = "romfs:/music/sonicMania/specialStage_start.wav";
			loopName = "romfs:/music/sonicMania/specialStage_loop.wav";
			break;
		case 11: // Style Savvy - My Room
			startName = "romfs:/music/styleSavvy/myRoom_start.wav";
			loopName = "romfs:/music/styleSavvy/myRoom_loop.wav";
			break;
		case 12: // Style Savvy - Map (Spring)
			startName = "romfs:/music/styleSavvy/mapSpring_start.wav";
			loopName = "romfs:/music/styleSavvy/mapSpring_loop.wav";
			break;
		case 13: // Style Savvy - Map (Summer)
			startName = "romfs:/music/styleSavvy/mapSummer_start.wav";
			loopName = "romfs:/music/styleSavvy/mapSummer_loop.wav";
			break;
		case 14: // Style Savvy - Map (Fall)
			startName = "romfs:/music/styleSavvy/mapFall_start.wav";
			loopName = "romfs:/music/styleSavvy/mapFall_loop.wav";
			break;
		case 15: // Style Savvy - Map (Winter)
			startName = "romfs:/music/styleSavvy/mapWinter_start.wav";
			loopName = "romfs:/music/styleSavvy/mapWinter_loop.wav";
			break;
		case 16: // Style Savvy - Wireless Menu
			startName = "romfs:/music/styleSavvy/wirelessMenu_start.wav";
			loopName = "romfs:/music/styleSavvy/wirelessMenu_loop.wav";
			break;
		case 17: // Style Savvy - Wireless Map
			startName = "romfs:/music/styleSavvy/wirelessMap_start.wav";
			loopName = "romfs:/music/styleSavvy/wirelessMap_loop.wav";
			break;
		case 18: // Jim Power: The Lost Dimension in 3D (GEN/MD) - Stage 3: Mutant's Forest
			startName = "romfs:/music/jimPower3D_gen/mutantsForest_start.wav";
			loopName = "romfs:/music/jimPower3D_gen/mutantsForest_loop.wav";
			break;
	}

	if (access(startName, F_OK) == 0) {
		music = new sound(startName, 0, false);
	}
	if (access(loopName, F_OK) == 0) {
		music_loop = new sound(loopName, 1, true);
	}
}
