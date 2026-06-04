#ifndef INPUT_H
#define INPUT_H

#include <3ds.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

SwkbdButton fetch_swkbd_input(SwkbdType type, const char* hint, char* out_buf, size_t buf_size);
struct in_addr fetch_ip(void);
int fetch_port(void);

#endif