#include "ds18b20.h"
#include "led.h"
#include "udp_socket.h"
#include "wifi.h"
#include <zephyr/kernel.h>

int main(void) {
    if (!wifi_init()) {
        return -1;
    }

    if (!udp_socket_init()) {
        return -1;
    }

    if (!ds18b20_init()) {
        return -1;
    }

    if (!led_init()) {
        return -1;
    }

    return 0;
}
