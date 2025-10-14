#include <zephyr/kernel.h>
#include "wifi.h"
#include "udp_socket.h"
#include "ds18b20.h"
#include "led.h"

int main(void)
{
	if (!wifi_init()) {
		return -1;
	}

	if (!ds18b20_init()) {
		return -1;
	}

	if (!led_init()){
		return -1;
	}

	return 0;
}
