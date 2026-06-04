#include <stdio.h>
#include "log.h"
#include "input.h"

void cleanup_services(void) {
	gfxExit();
	socExit();
}

int main(int argc, char* argv[]) {
	gfxInitDefault();
	atexit(cleanup_services);
	consoleInit(GFX_TOP, NULL);

	init_logging(fetch_ip(), fetch_port());
	printf("Successfully initialized network info.\n");

	while (aptMainLoop())
	{
		gspWaitForVBlank();
		gfxSwapBuffers();
		hidScanInput();

		u32 k_down = hidKeysDown();
		if (k_down & KEY_START)
			break;
	}

	gfxExit();
	return 0;
}