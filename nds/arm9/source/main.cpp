#include <maxmod9.h>
#include <nds.h>

#include <fat.h>
#include <limits.h>
#include <stdio.h>
#include <sys/stat.h>

#include <gl2d.h>
#include <string.h>
#include <unistd.h>

#include "inifile.h"
#include "util/stringtool.h"
#include "tonccpy.h"

touchPosition touch;

//---------------------------------------------------------------------------------
void stop(void) {
//---------------------------------------------------------------------------------
	while (1) {
		swiWaitForVBlank();
	}
}

void doPause() {
	while (1) {
		scanKeys();
		if (keysDown() & KEY_START)
			break;
		snd().updateStream();
		swiWaitForVBlank();
	}
	scanKeys();
}

int main(int argc, char **argv) {
	defaultExceptionHandler();

	return 0;
}
