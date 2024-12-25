#include "common.hpp"
#include "sound.h"

#include <unistd.h>

extern sound *music;
extern sound *music_loop;
extern bool dspfirmfound;
extern bool musicStartLoaded;

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
			startName = "romfs:/music/styleSavvy/wifiMap_start.wav";
			loopName = "romfs:/music/styleSavvy/wifiMap_loop.wav";
			break;
		case 18: // Jim Power: The Lost Dimension in 3D (GEN/MD) - Stage 3: Mutant's Forest
			startName = "romfs:/music/jimPower3D_gen/mutantsForest_start.wav";
			loopName = "romfs:/music/jimPower3D_gen/mutantsForest_loop.wav";
			break;
		case 19: // Savvy Manager - Menu
			startName = "romfs:/music/savvyManager/menu_start.wav";
			loopName = "romfs:/music/savvyManager/menu_loop.wav";
			break;
		case 20: // Style Savvy: Trendsetters - Title Screen
			startName = "romfs:/music/styleSavvy2/title_start.wav";
			loopName = "romfs:/music/styleSavvy2/title_loop.wav";
			break;
		case 21: // Style Savvy: Trendsetters - Map (Max)
			startName = "romfs:/music/styleSavvy2/map_start.wav";
			loopName = "romfs:/music/styleSavvy2/map_loop.wav";
			break;
		case 22: // Style Savvy: Trendsetters - Emmylou's Theme
			loopName = "romfs:/music/styleSavvy2/tetsuko.wav";
			break;
		case 23: // Style Savvy: Trendsetters - Brad's Theme
			loopName = "romfs:/music/styleSavvy2/musashi.wav";
			break;
		case 24: // Style Savvy: Trendsetters - Buyer's Center
			startName = "romfs:/music/styleSavvy2/exhibition_start.wav";
			loopName = "romfs:/music/styleSavvy2/exhibition_loop.wav";
			break;
		case 25: // Style Savvy: Trendsetters - Hair Salon
			startName = "romfs:/music/styleSavvy2/hairSalon_start.wav";
			loopName = "romfs:/music/styleSavvy2/hairSalon_loop.wav";
			break;
		case 26: // Style Savvy: Trendsetters - Makeup Salon
			startName = "romfs:/music/styleSavvy2/makeSalon_start.wav";
			loopName = "romfs:/music/styleSavvy2/makeSalon_loop.wav";
			break;
		case 27: // Style Savvy: Trendsetters - Photo Studio
			loopName = "romfs:/music/styleSavvy2/photoStudio.wav";
			break;
		case 28: // Style Savvy: Trendsetters - Fashion Contest Menu
			loopName = "romfs:/music/styleSavvy2/contest.wav";
			break;
		case 29: // Style Savvy: Trendsetters - MC Mode's Theme
			loopName = "romfs:/music/styleSavvy2/contestDJ.wav";
			break;
		case 30: // Style Savvy: Trendsetters - Cute Runway Theme
			startName = "romfs:/music/styleSavvy2/runway00_start.wav";
			loopName = "romfs:/music/styleSavvy2/runway00_loop.wav";
			break;
		case 31: // Style Savvy: Trendsetters - Pop Runway Theme
			loopName = "romfs:/music/styleSavvy2/runway01.wav";
			break;
		case 32: // Style Savvy: Trendsetters - Runway Theme 02
			startName = "romfs:/music/styleSavvy2/runway02_start.wav";
			loopName = "romfs:/music/styleSavvy2/runway02_loop.wav";
			break;
		case 33: // Style Savvy: Trendsetters - Romantic Runway Theme
			startName = "romfs:/music/styleSavvy2/runway03_start.wav";
			loopName = "romfs:/music/styleSavvy2/runway03_loop.wav";
			break;
		case 34: // Style Savvy: Trendsetters - Boho-Chic Runway Theme
			loopName = "romfs:/music/styleSavvy2/runway04.wav";
			break;
		case 35: // Style Savvy: Trendsetters - Rock Runway Theme
			startName = "romfs:/music/styleSavvy2/runway05_start.wav";
			loopName = "romfs:/music/styleSavvy2/runway05_loop.wav";
			break;
		case 36: // Style Savvy: Trendsetters - Wireless Shop
			loopName = "romfs:/music/styleSavvy2/girlsMall.wav";
			break;
		case 37: // Style Savvy: Trendsetters - Main Shop Data
			loopName = "romfs:/music/styleSavvy2/personalCheck.wav";
			break;
		case 38: // Style Savvy - Hair Salon
			loopName = "romfs:/music/styleSavvy/hairSalon.wav";
			break;
		case 39: // Style Savvy - Makeup Salon
			startName = "romfs:/music/styleSavvy/makeSalon_start.wav";
			loopName = "romfs:/music/styleSavvy/makeSalon_loop.wav";
			break;
		case 40: // Style Savvy - Unused?
			loopName = "romfs:/music/styleSavvy/exhibition.wav";
			break;
		case 41: // Style Savvy - Dominic's Mansion
			loopName = "romfs:/music/styleSavvy/mansion.wav";
			break;
		case 42: // Super Mario Bros. - Overworld
			startName = "romfs:/music/superMarioBros/overworld_start.wav";
			loopName = "romfs:/music/superMarioBros/overworld_loop.wav";
			break;
		case 43: // Sonic Jam - Menu
			startName = "romfs:/music/sonicJam/menu_start.wav";
			loopName = "romfs:/music/sonicJam/menu_loop.wav";
			break;
		case 44: // Sonic Jam - Sonic World
			startName = "romfs:/music/sonicJam/sworld_start.wav";
			loopName = "romfs:/music/sonicJam/sworld_loop.wav";
			break;
		case 45: // Sonic Jam - Gallery
			startName = "romfs:/music/sonicJam/gallery_start.wav";
			loopName = "romfs:/music/sonicJam/gallery_loop.wav";
			break;
		case 46: // Style Savvy: Fashion Forward - Intro Song (Japanese)
			startName = "nitro:/music/styleSavvy3/introJapanese_start.wav";
			loopName = "nitro:/music/styleSavvy3/introJapanese_loop.wav";
			break;
		case 47: // Style Savvy: Fashion Forward - Intro Song (English)
			startName = "nitro:/music/styleSavvy3/introEnglish_start.wav";
			loopName = "nitro:/music/styleSavvy3/introEnglish_loop.wav";
			break;
		case 48: // Style Savvy: Fashion Forward - Title Screen
			loopName = "romfs:/music/styleSavvy3/title.wav";
			break;
		case 49: // Style Savvy: Fashion Forward - My Apartment
			loopName = "romfs:/music/styleSavvy3/apartment.wav";
			break;
		case 50: // Style Savvy: Fashion Forward - Beaumonde City (Morning)
			loopName = "romfs:/music/styleSavvy3/beaumonde6AM.wav";
			break;
		case 51: // Style Savvy: Fashion Forward - Beaumonde City (Noon)
			loopName = "romfs:/music/styleSavvy3/beaumonde1PM.wav";
			break;
		case 52: // Style Savvy: Fashion Forward - Beaumonde City (Sunset)
			loopName = "romfs:/music/styleSavvy3/beaumonde7PM.wav";
			break;
		case 53: // Style Savvy: Fashion Forward - Beaumonde City (Night)
			loopName = "romfs:/music/styleSavvy3/beaumonde9PM.wav";
			break;
		case 54: // Style Savvy: Fashion Forward - Beaumonde City (Midnight)
			loopName = "romfs:/music/styleSavvy3/beaumonde12AM.wav";
			break;
		case 55: // Style Savvy: Fashion Forward - Phone
			startName = "romfs:/music/styleSavvy3/phone_start.wav";
			loopName = "romfs:/music/styleSavvy3/phone_loop.wav";
			break;
		case 56: // Style Savvy: Fashion Forward - Wardrobe
			loopName = "romfs:/music/styleSavvy3/wardrobe.wav";
			break;
	}

	if (access(startName, F_OK) == 0) {
		music = new sound(startName, 0, false);
		musicStartLoaded = true;
	}
	if (access(loopName, F_OK) == 0) {
		music_loop = new sound(loopName, 1, true);
	}
}
