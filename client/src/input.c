#include "input.h"

SwkbdState swkbd;

SwkbdButton fetch_swkbd_input(SwkbdType type, const char* hint, char* out_buf, size_t buf_size) {
    SwkbdState swkbd;
    
    swkbdInit(&swkbd, type, 1, buf_size - 1);
    swkbdSetHintText(&swkbd, hint);
    
    return swkbdInputText(&swkbd, out_buf, buf_size);
}

struct in_addr fetch_ip(void) {
    SwkbdButton status;
    char ip_string[26];
    struct in_addr ip;

    while (1) {
        do {
            status = fetch_swkbd_input(SWKBD_TYPE_NORMAL, "IP Address", ip_string, sizeof(ip_string) - 1);
     
            if (status == SWKBD_BUTTON_LEFT || status == SWKBD_BUTTON_NONE) {
                exit(0);
            }
        } while (status != SWKBD_BUTTON_RIGHT);

        if (inet_aton(ip_string, &ip)) {
            return ip;
        }
    }
}

int fetch_port(void) {
    SwkbdButton status;
    char port_string[10];
    int port;

    while (1) {
        do {
            status = fetch_swkbd_input(SWKBD_TYPE_NUMPAD, "Port", port_string, sizeof(port_string) - 1);
     
            if (status == SWKBD_BUTTON_LEFT || status == SWKBD_BUTTON_NONE) {
                exit(0);
            }
        } while (status != SWKBD_BUTTON_RIGHT);

        port = atoi(port_string);
        if (port != 0) {
            return port;
        }
    }
}


