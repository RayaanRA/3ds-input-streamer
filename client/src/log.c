#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include "log.h"
#include "input.h"

#define SOC_ALIGN 0x1000
#define SOC_BUFFERSIZE 0x100000

u32* soc_buffer = NULL;
int log_socket = -1;
struct sockaddr_in pc_addr;

void init_logging(struct in_addr pc_ip,int port) {
	soc_buffer = (u32*) memalign(SOC_ALIGN, SOC_BUFFERSIZE);
	if (!soc_buffer) return;

	Result rc = socInit(soc_buffer, SOC_BUFFERSIZE);
	if (R_FAILED(rc)) return;

	log_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (log_socket < 0) return;

	int flags = fcntl(log_socket, F_GETFL, 0);
	fcntl(log_socket, F_SETFL, flags | O_NONBLOCK);

	pc_addr.sin_family = AF_INET;
	pc_addr.sin_port = htons(port);
	pc_addr.sin_addr = pc_ip;
}

void send_log(const char* message) {
	ssize_t bytes_sent = sendto(
		log_socket, 
		message, 
		strlen(message), 
		0, 
		(struct sockaddr *)&pc_addr, 
		sizeof(pc_addr)
	);
}

void cleanup_logging(void) {
	close(log_socket);
	socExit();
}