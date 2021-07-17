#include "common.hpp"
#include "sound.h"

#include <unistd.h>

void loadMusic(int num) {
	const char* startName = "";
	const char* loopName = "";
	u32 sampleRate = 0;
	bool stereo = true;

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
			sampleRate = 32000;
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
			sampleRate = 22050;
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
		case 10: // Sonic Mania - Special Stage: "Dimension Heist"
			startName = "nitro:/music/sonicMania/specialStage_start.raw";
			loopName = "nitro:/music/sonicMania/specialStage_loop.raw";
			sampleRate = 44100;
			break;
		case 11: // Style Savvy - My Room
			startName = "nitro:/music/styleSavvy/myRoom_start.raw";
			loopName = "nitro:/music/styleSavvy/myRoom_loop.raw";
			sampleRate = 32000;
			break;
		case 12: // Style Savvy - Map (Spring)
			startName = "nitro:/music/styleSavvy/mapSpring_start.raw";
			loopName = "nitro:/music/styleSavvy/mapSpring_loop.raw";
			sampleRate = 32000;
			break;
		case 13: // Style Savvy - Map (Summer)
			startName = "nitro:/music/styleSavvy/mapSummer_start.raw";
			loopName = "nitro:/music/styleSavvy/mapSummer_loop.raw";
			sampleRate = 32000;
			break;
		case 14: // Style Savvy - Map (Fall)
			startName = "nitro:/music/styleSavvy/mapFall_start.raw";
			loopName = "nitro:/music/styleSavvy/mapFall_loop.raw";
			sampleRate = 32000;
			break;
		case 15: // Style Savvy - Map (Winter)
			startName = "nitro:/music/styleSavvy/mapWinter_start.raw";
			loopName = "nitro:/music/styleSavvy/mapWinter_loop.raw";
			sampleRate = 32000;
			break;
		case 16: // Style Savvy - Wireless Menu
			startName = "nitro:/music/styleSavvy/wirelessMenu_start.raw";
			loopName = "nitro:/music/styleSavvy/wirelessMenu_loop.raw";
			sampleRate = 32000;
			break;
		case 17: // Style Savvy - Wireless Map
			startName = "nitro:/music/styleSavvy/wifiMap_start.raw";
			loopName = "nitro:/music/styleSavvy/wifiMap_loop.raw";
			sampleRate = 32000;
			break;
		case 18: // Jim Power: The Lost Dimension in 3D (GEN/MD) - Stage 3: Mutant's Forest
			startName = "nitro:/music/jimPower3D_gen/mutantsForest_start.raw";
			loopName = "nitro:/music/jimPower3D_gen/mutantsForest_loop.raw";
			sampleRate = 48000;
			stereo = false;
			break;
		case 19: // Savvy Manager - Menu
			startName = "nitro:/music/savvyManager/menu_start.raw";
			loopName = "nitro:/music/savvyManager/menu_loop.raw";
			sampleRate = 48000;
			stereo = false;
			break;
		case 20: // Style Savvy: Trendsetters - Title Screen
			startName = "nitro:/music/styleSavvy2/title_start.raw";
			loopName = "nitro:/music/styleSavvy2/title_loop.raw";
			sampleRate = 32728;
			break;
		case 21: // Style Savvy: Trendsetters - Map (Max)
			startName = "nitro:/music/styleSavvy2/map_start.raw";
			loopName = "nitro:/music/styleSavvy2/map_loop.raw";
			sampleRate = 32728;
			break;
		case 22: // Style Savvy: Trendsetters - Emmylou's Theme
			loopName = "nitro:/music/styleSavvy2/tetsuko.raw";
			sampleRate = 32728;
			break;
		case 23: // Style Savvy: Trendsetters - Brad's Theme
			loopName = "nitro:/music/styleSavvy2/musashi.raw";
			sampleRate = 22050;
			break;
		case 24: // Style Savvy: Trendsetters - Buyer's Center
			startName = "nitro:/music/styleSavvy2/exhibition_start.raw";
			loopName = "nitro:/music/styleSavvy2/exhibition_loop.raw";
			sampleRate = 32728;
			break;
		case 25: // Style Savvy: Trendsetters - Hair Salon
			startName = "nitro:/music/styleSavvy2/hairSalon_start.raw";
			loopName = "nitro:/music/styleSavvy2/hairSalon_loop.raw";
			sampleRate = 32728;
			break;
		case 26: // Style Savvy: Trendsetters - Makeup Salon
			startName = "nitro:/music/styleSavvy2/makeSalon_start.raw";
			loopName = "nitro:/music/styleSavvy2/makeSalon_loop.raw";
			sampleRate = 44100;
			break;
		case 27: // Style Savvy: Trendsetters - Photo Studio
			loopName = "nitro:/music/styleSavvy2/photoStudio.raw";
			sampleRate = 32728;
			break;
		case 28: // Style Savvy: Trendsetters - Fashion Contest Menu
			loopName = "nitro:/music/styleSavvy2/contest.raw";
			sampleRate = 22050;
			break;
		case 29: // Style Savvy: Trendsetters - MC Mode's Theme
			loopName = "nitro:/music/styleSavvy2/contestDJ.raw";
			sampleRate = 32728;
			break;
		case 30: // Style Savvy: Trendsetters - Cute Runway Theme
			startName = "nitro:/music/styleSavvy2/runway00_start.raw";
			loopName = "nitro:/music/styleSavvy2/runway00_loop.raw";
			sampleRate = 32728;
			break;
		case 31: // Style Savvy: Trendsetters - Pop Runway Theme
			loopName = "nitro:/music/styleSavvy2/runway01.raw";
			sampleRate = 32728;
			break;
		case 32: // Style Savvy: Trendsetters - Runway Theme 02
			startName = "nitro:/music/styleSavvy2/runway02_start.raw";
			loopName = "nitro:/music/styleSavvy2/runway02_loop.raw";
			sampleRate = 32728;
			break;
		case 33: // Style Savvy: Trendsetters - Romantic Runway Theme
			startName = "nitro:/music/styleSavvy2/runway03_start.raw";
			loopName = "nitro:/music/styleSavvy2/runway03_loop.raw";
			sampleRate = 32728;
			break;
		case 34: // Style Savvy: Trendsetters - Boho-Chic Runway Theme
			loopName = "nitro:/music/styleSavvy2/runway04.raw";
			sampleRate = 32728;
			break;
		case 35: // Style Savvy: Trendsetters - Rock Runway Theme
			startName = "nitro:/music/styleSavvy2/runway05_start.raw";
			loopName = "nitro:/music/styleSavvy2/runway05_loop.raw";
			sampleRate = 22050;
			break;
		case 36: // Style Savvy: Trendsetters - Wireless Shop
			loopName = "nitro:/music/styleSavvy2/girlsMall.raw";
			sampleRate = 22050;
			break;
		case 37: // Style Savvy: Trendsetters - Main Shop Data
			loopName = "nitro:/music/styleSavvy2/personalCheck.raw";
			sampleRate = 22050;
			break;
		case 38: // Style Savvy - Hair Salon
			loopName = "nitro:/music/styleSavvy/hairSalon.raw";
			sampleRate = 32000;
			break;
		case 39: // Style Savvy - Makeup Salon
			startName = "nitro:/music/styleSavvy/makeSalon_start.raw";
			loopName = "nitro:/music/styleSavvy/makeSalon_loop.raw";
			sampleRate = 32000;
			break;
		case 40: // Style Savvy - Unused?
			loopName = "nitro:/music/styleSavvy/exhibition.raw";
			sampleRate = 32000;
			break;
		case 41: // Style Savvy - Dominic's Mansion
			loopName = "nitro:/music/styleSavvy/mansion.raw";
			sampleRate = 32000;
			break;
		case 42: // Super Mario Bros. - Overworld
			startName = "nitro:/music/superMarioBros/overworld_start.raw";
			loopName = "nitro:/music/superMarioBros/overworld_loop.raw";
			sampleRate = 32000;
			stereo = false;
			break;
		case 43: // Sonic Jam - Menu
			startName = "nitro:/music/sonicJam/menu_start.raw";
			loopName = "nitro:/music/sonicJam/menu_loop.raw";
			sampleRate = 32000;
			break;
		case 44: // Sonic Jam - Sonic World
			startName = "nitro:/music/sonicJam/sworld_start.raw";
			loopName = "nitro:/music/sonicJam/sworld_loop.raw";
			sampleRate = 32000;
			break;
		case 45: // Sonic Jam - Gallery
			startName = "nitro:/music/sonicJam/gallery_start.raw";
			loopName = "nitro:/music/sonicJam/gallery_loop.raw";
			sampleRate = 22050;
			break;
	}
	snd().loadStream(startName, loopName, sampleRate, stereo, true);
}
