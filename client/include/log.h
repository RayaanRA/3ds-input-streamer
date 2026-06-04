#ifndef LOG_H
#define LOG_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void init_logging(struct in_addr pc_ip,int port);
void send_log(const char* message);
void cleanup_logging(void);

#endif