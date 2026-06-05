#ifndef LOG_H
#define LOG_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

struct __attribute__((__packed__)) InputPacket {
    u32 buttons;
    s16 circle_x;
    s16 circle_y;
};

void init_logging(struct in_addr pc_ip,int port);
void send_packet(const void* data, size_t size);
void cleanup_logging(void);

#endif