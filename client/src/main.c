#include <stdio.h>
#include <stdlib.h>
#include <3ds.h>
#include "log.h"
#include "input.h"

void cleanup_services(void) {
	cleanup_logging();
	gfxExit();
}

int main(int argc, char* argv[]) {
	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);
	atexit(cleanup_services);

	printf("--- 3DS Input Streamer ---\n\n");
	printf("Please input your server information...\n");

	struct in_addr server_ip = fetch_ip();
	int server_port = fetch_port();

	init_logging(server_ip, server_port);
	
	consoleClear();
	printf("Streaming inputs to PC!\n");
	printf("Target: %s:%d\n\n", inet_ntoa(server_ip), server_port);
	printf("Press [START] to stop streaming and exit.\n");

	struct InputPacket packet;
	circlePosition circle;

	while (aptMainLoop())
	{
		gspWaitForVBlank();
		gfxSwapBuffers();
		
		hidScanInput();
		u32 k_held = hidKeysHeld();
		hidCircleRead(&circle);

		packet.buttons = k_held;
		packet.circle_x = circle.dx;
		packet.circle_y = circle.dy;

		send_packet(&packet, sizeof(packet));

		if (hidKeysDown() & KEY_START)
			break;
	}

	return 0;
}
