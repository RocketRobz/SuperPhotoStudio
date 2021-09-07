#include "common.hpp"
#include "sound.h"

#include <unistd.h>

void loadMusic(int num) {
	const char* startName = "";
	const char* startCache = "";
	const char* loopName = "";
	const char* loopCache = "";
	u32 sampleRate = 0;
	bool stereo = true;

	switch (num) {
		case 0:	// Pokemon Stadium - Stadium Select
		default:
			startName = "nitro:/music/pkmnStadium/stadiumSel_start.wav";
			loopName = "nitro:/music/pkmnStadium/stadiumSel_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/pkmnStadium/stadiumSel_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/pkmnStadium/stadiumSel_loop.raw";
			sampleRate = 32000;
			break;
		case 1:	// MegaMan: Battle and Chase - The Straight Out
			startName = "nitro:/music/mmBattleAndChase/straightOut_start.wav";
			loopName = "nitro:/music/mmBattleAndChase/straightOut_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/mmBattleAndChase/straightOut_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/mmBattleAndChase/straightOut_loop.raw";
			sampleRate = 44100;
			break;
		case 2:	// Mario Golf: Advance Tour - Marion Club
			startName = "nitro:/music/marioGolfAdvanceTour/marionClub_start.wav";
			loopName = "nitro:/music/marioGolfAdvanceTour/marionClub_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/marioGolfAdvanceTour/marionClub_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/marioGolfAdvanceTour/marionClub_loop.raw";
			sampleRate = 48000;
			break;
		case 3:	// Tetris Party - Title Screen
			startName = "nitro:/music/tetrisParty/titleScreen_start.wav";
			loopName = "nitro:/music/tetrisParty/titleScreen_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/tetrisParty/titleScreen_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/tetrisParty/titleScreen_loop.raw";
			sampleRate = 48000;
			break;
		case 4:	// Tetris Party - Main Menu
			loopName = "nitro:/music/tetrisParty/mainMenu.wav";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/tetrisParty/mainMenu.raw";
			sampleRate = 48000;
			break;
		case 5:	// Sonic Mania - Main Menu: "Comfort Zone"
			startName = "nitro:/music/sonicMania/mainMenu_start.wav";
			loopName = "nitro:/music/sonicMania/mainMenu_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/sonicMania/mainMenu_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/sonicMania/mainMenu_loop.raw";
			sampleRate = 44100;
			break;
		case 6:	// Sonic Mania - Angel Island Zone
			loopName = "nitro:/music/sonicMania/angelIslandZone.wav";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/sonicMania/angelIslandZone.raw";
			sampleRate = 44100;
			break;
		case 7:	// Sonic Mania - Green Hill Zone: Act 1
			startName = "nitro:/music/sonicMania/greenHillZoneAct1_start.wav";
			loopName = "nitro:/music/sonicMania/greenHillZoneAct1_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/sonicMania/greenHillZoneAct1_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/sonicMania/greenHillZoneAct1_loop.raw";
			sampleRate = 44100;
			break;
		case 8:	// Sonic Mania - Green Hill Zone: Act 2
			startName = "nitro:/music/sonicMania/greenHillZoneAct2_start.wav";
			loopName = "nitro:/music/sonicMania/greenHillZoneAct2_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/sonicMania/greenHillZoneAct2_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/sonicMania/greenHillZoneAct2_loop.raw";
			sampleRate = 44100;
			break;
		case 9:	// Sonic Mania - Studiopolis Zone: Act 1
			startName = "nitro:/music/sonicMania/studiopolisZoneAct1_start.wav";
			loopName = "nitro:/music/sonicMania/studiopolisZoneAct1_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/sonicMania/studiopolisZoneAct1_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/sonicMania/studiopolisZoneAct1_loop.raw";
			sampleRate = 44100;
			break;
		case 10: // Sonic Mania - Special Stage: "Dimension Heist"
			startName = "nitro:/music/sonicMania/specialStage_start.wav";
			loopName = "nitro:/music/sonicMania/specialStage_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/sonicMania/specialStage_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/sonicMania/specialStage_loop.raw";
			sampleRate = 44100;
			break;
		case 11: // Style Savvy - My Room
			startName = "nitro:/music/styleSavvy/myRoom_start.wav";
			loopName = "nitro:/music/styleSavvy/myRoom_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy/myRoom_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy/myRoom_loop.raw";
			sampleRate = 32000;
			break;
		case 12: // Style Savvy - Map (Spring)
			startName = "nitro:/music/styleSavvy/mapSpring_start.wav";
			loopName = "nitro:/music/styleSavvy/mapSpring_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy/mapSpring_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy/mapSpring_loop.raw";
			sampleRate = 32000;
			break;
		case 13: // Style Savvy - Map (Summer)
			startName = "nitro:/music/styleSavvy/mapSummer_start.wav";
			loopName = "nitro:/music/styleSavvy/mapSummer_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy/mapSummer_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy/mapSummer_loop.raw";
			sampleRate = 32000;
			break;
		case 14: // Style Savvy - Map (Fall)
			startName = "nitro:/music/styleSavvy/mapFall_start.wav";
			loopName = "nitro:/music/styleSavvy/mapFall_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy/mapFall_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy/mapFall_loop.raw";
			sampleRate = 32000;
			break;
		case 15: // Style Savvy - Map (Winter)
			startName = "nitro:/music/styleSavvy/mapWinter_start.wav";
			loopName = "nitro:/music/styleSavvy/mapWinter_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy/mapWinter_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy/mapWinter_loop.raw";
			sampleRate = 32000;
			break;
		case 16: // Style Savvy - Wireless Menu
			startName = "nitro:/music/styleSavvy/wirelessMenu_start.wav";
			loopName = "nitro:/music/styleSavvy/wirelessMenu_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy/wirelessMenu_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy/wirelessMenu_loop.raw";
			sampleRate = 32000;
			break;
		case 17: // Style Savvy - Wireless Map
			startName = "nitro:/music/styleSavvy/wifiMap_start.wav";
			loopName = "nitro:/music/styleSavvy/wifiMap_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy/wifiMap_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy/wifiMap_loop.raw";
			sampleRate = 32000;
			break;
		case 18: // Jim Power: The Lost Dimension in 3D (GEN/MD) - Stage 3: Mutant's Forest
			startName = "nitro:/music/jimPower3D_gen/mutantsForest_start.wav";
			loopName = "nitro:/music/jimPower3D_gen/mutantsForest_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/jimPower3D_gen/mutantsForest_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/jimPower3D_gen/mutantsForest_loop.raw";
			sampleRate = 48000;
			stereo = false;
			break;
		case 19: // Savvy Manager - Menu
			startName = "nitro:/music/savvyManager/menu_start.wav";
			loopName = "nitro:/music/savvyManager/menu_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/savvyManager/menu_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/savvyManager/menu_loop.raw";
			sampleRate = 48000;
			stereo = false;
			break;
		case 20: // Style Savvy: Trendsetters - Title Screen
			startName = "nitro:/music/styleSavvy2/title_start.wav";
			loopName = "nitro:/music/styleSavvy2/title_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy2/title_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy2/title_loop.raw";
			sampleRate = 32728;
			break;
		case 21: // Style Savvy: Trendsetters - Map (Max)
			startName = "nitro:/music/styleSavvy2/map_start.wav";
			loopName = "nitro:/music/styleSavvy2/map_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy2/map_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy2/map_loop.raw";
			sampleRate = 32728;
			break;
		case 22: // Style Savvy: Trendsetters - Emmylou's Theme
			loopName = "nitro:/music/styleSavvy2/tetsuko.wav";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy2/tetsuko.raw";
			sampleRate = 32728;
			break;
		case 23: // Style Savvy: Trendsetters - Brad's Theme
			loopName = "nitro:/music/styleSavvy2/musashi.wav";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy2/musashi.raw";
			sampleRate = 32728;
			break;
		case 24: // Style Savvy: Trendsetters - Buyer's Center
			startName = "nitro:/music/styleSavvy2/exhibition_start.wav";
			loopName = "nitro:/music/styleSavvy2/exhibition_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy2/exhibition_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy2/exhibition_loop.raw";
			sampleRate = 32728;
			break;
		case 25: // Style Savvy: Trendsetters - Hair Salon
			startName = "nitro:/music/styleSavvy2/hairSalon_start.wav";
			loopName = "nitro:/music/styleSavvy2/hairSalon_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy2/hairSalon_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy2/hairSalon_loop.raw";
			sampleRate = 32728;
			break;
		case 26: // Style Savvy: Trendsetters - Makeup Salon
			startName = "nitro:/music/styleSavvy2/makeSalon_start.wav";
			loopName = "nitro:/music/styleSavvy2/makeSalon_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy2/makeSalon_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy2/makeSalon_loop.raw";
			sampleRate = 44100;
			break;
		case 27: // Style Savvy: Trendsetters - Photo Studio
			loopName = "nitro:/music/styleSavvy2/photoStudio.wav";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy2/photoStudio.raw";
			sampleRate = 32728;
			break;
		case 28: // Style Savvy: Trendsetters - Fashion Contest Menu
			loopName = "nitro:/music/styleSavvy2/contest.wav";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy2/contest.raw";
			sampleRate = 32728;
			break;
		case 29: // Style Savvy: Trendsetters - MC Mode's Theme
			loopName = "nitro:/music/styleSavvy2/contestDJ.wav";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy2/contestDJ.raw";
			sampleRate = 32728;
			break;
		case 30: // Style Savvy: Trendsetters - Cute Runway Theme
			startName = "nitro:/music/styleSavvy2/runway00_start.wav";
			loopName = "nitro:/music/styleSavvy2/runway00_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy2/runway00_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy2/runway00_loop.raw";
			sampleRate = 32728;
			break;
		case 31: // Style Savvy: Trendsetters - Pop Runway Theme
			loopName = "nitro:/music/styleSavvy2/runway01.wav";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy2/runway01.raw";
			sampleRate = 32728;
			break;
		case 32: // Style Savvy: Trendsetters - Runway Theme 02
			startName = "nitro:/music/styleSavvy2/runway02_start.wav";
			loopName = "nitro:/music/styleSavvy2/runway02_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy2/runway02_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy2/runway02_loop.raw";
			sampleRate = 32728;
			break;
		case 33: // Style Savvy: Trendsetters - Romantic Runway Theme
			startName = "nitro:/music/styleSavvy2/runway03_start.wav";
			loopName = "nitro:/music/styleSavvy2/runway03_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy2/runway03_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy2/runway03_loop.raw";
			sampleRate = 32728;
			break;
		case 34: // Style Savvy: Trendsetters - Boho-Chic Runway Theme
			loopName = "nitro:/music/styleSavvy2/runway04.wav";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy2/runway04.raw";
			sampleRate = 32728;
			break;
		case 35: // Style Savvy: Trendsetters - Rock Runway Theme
			startName = "nitro:/music/styleSavvy2/runway05_start.wav";
			loopName = "nitro:/music/styleSavvy2/runway05_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy2/runway05_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy2/runway05_loop.raw";
			sampleRate = 32728;
			break;
		case 36: // Style Savvy: Trendsetters - Wireless Shop
			loopName = "nitro:/music/styleSavvy2/girlsMall.wav";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy2/girlsMall.raw";
			sampleRate = 32728;
			break;
		case 37: // Style Savvy: Trendsetters - Main Shop Data
			loopName = "nitro:/music/styleSavvy2/personalCheck.wav";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy2/personalCheck.raw";
			sampleRate = 32728;
			break;
		case 38: // Style Savvy - Hair Salon
			loopName = "nitro:/music/styleSavvy/hairSalon.wav";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy/hairSalon.raw";
			sampleRate = 32000;
			break;
		case 39: // Style Savvy - Makeup Salon
			startName = "nitro:/music/styleSavvy/makeSalon_start.wav";
			loopName = "nitro:/music/styleSavvy/makeSalon_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy/makeSalon_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy/makeSalon_loop.raw";
			sampleRate = 32000;
			break;
		case 40: // Style Savvy - Unused?
			loopName = "nitro:/music/styleSavvy/exhibition.wav";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy/exhibition.raw";
			sampleRate = 32000;
			break;
		case 41: // Style Savvy - Dominic's Mansion
			loopName = "nitro:/music/styleSavvy/mansion.wav";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy/mansion.raw";
			sampleRate = 32000;
			break;
		case 42: // Super Mario Bros. - Overworld
			startName = "nitro:/music/superMarioBros/overworld_start.wav";
			loopName = "nitro:/music/superMarioBros/overworld_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/superMarioBros/overworld_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/superMarioBros/overworld_loop.raw";
			sampleRate = 48000;
			stereo = false;
			break;
		case 43: // Sonic Jam - Menu
			startName = "nitro:/music/sonicJam/menu_start.wav";
			loopName = "nitro:/music/sonicJam/menu_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/sonicJam/menu_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/sonicJam/menu_loop.raw";
			sampleRate = 44100;
			break;
		case 44: // Sonic Jam - Sonic World
			startName = "nitro:/music/sonicJam/sworld_start.wav";
			loopName = "nitro:/music/sonicJam/sworld_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/sonicJam/sworld_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/sonicJam/sworld_loop.raw";
			sampleRate = 44100;
			break;
		case 45: // Sonic Jam - Gallery
			startName = "nitro:/music/sonicJam/gallery_start.wav";
			loopName = "nitro:/music/sonicJam/gallery_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/sonicJam/gallery_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/sonicJam/gallery_loop.raw";
			sampleRate = 44100;
			break;
		case 46: // Style Savvy: Fashion Forward - Intro Song (Japanese)
			startName = "nitro:/music/styleSavvy3/introJapanese_start.wav";
			loopName = "nitro:/music/styleSavvy3/introJapanese_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy3/introJapanese_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy3/introJapanese_loop.raw";
			sampleRate = 32728;
			break;
		case 47: // Style Savvy: Fashion Forward - Intro Song (English)
			startName = "nitro:/music/styleSavvy3/introEnglish_start.wav";
			loopName = "nitro:/music/styleSavvy3/introEnglish_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy3/introEnglish_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy3/introEnglish_loop.raw";
			sampleRate = 32728;
			break;
		case 48: // Style Savvy: Fashion Forward - Title Screen
			loopName = "nitro:/music/styleSavvy3/title.wav";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy3/title.raw";
			sampleRate = 32000;
			break;
		case 49: // Style Savvy: Fashion Forward - My Apartment
			loopName = "nitro:/music/styleSavvy3/apartment.wav";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy3/apartment.raw";
			sampleRate = 32000;
			break;
		case 50: // Style Savvy: Fashion Forward - Beaumonde City (Morning)
			loopName = "nitro:/music/styleSavvy3/beaumonde6AM.wav";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy3/beaumonde6AM.raw";
			sampleRate = 32000;
			break;
		case 51: // Style Savvy: Fashion Forward - Beaumonde City (Noon)
			loopName = "nitro:/music/styleSavvy3/beaumonde1PM.wav";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy3/beaumonde1PM.raw";
			sampleRate = 32000;
			break;
		case 52: // Style Savvy: Fashion Forward - Beaumonde City (Sunset)
			loopName = "nitro:/music/styleSavvy3/beaumonde7PM.wav";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy3/beaumonde7PM.raw";
			sampleRate = 32000;
			break;
		case 53: // Style Savvy: Fashion Forward - Beaumonde City (Night)
			loopName = "nitro:/music/styleSavvy3/beaumonde9PM.wav";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy3/beaumonde9PM.raw";
			sampleRate = 32000;
			break;
		case 54: // Style Savvy: Fashion Forward - Beaumonde City (Midnight)
			loopName = "nitro:/music/styleSavvy3/beaumonde12AM.wav";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy3/beaumonde12AM.raw";
			sampleRate = 32000;
			break;
		case 55: // Style Savvy: Fashion Forward - Phone
			startName = "nitro:/music/styleSavvy3/phone_start.wav";
			loopName = "nitro:/music/styleSavvy3/phone_loop.wav";
			startCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy3/phone_start.raw";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy3/phone_loop.raw";
			sampleRate = 32000;
			break;
		case 56: // Style Savvy: Fashion Forward - Wardrobe
			loopName = "nitro:/music/styleSavvy3/wardrobe.wav";
			loopCache = "/_nds/SuperPhotoStudio/musicCache/styleSavvy3/wardrobe.raw";
			sampleRate = 32000;
			break;
	}
	snd().loadStream(startName, startCache, loopName, loopCache, sampleRate, stereo, true);
}
