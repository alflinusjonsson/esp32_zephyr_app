#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "wifi.h"
#include "udp_socket.h"
#include "ds18b20.h"

LOG_MODULE_REGISTER(APP_MAIN, CONFIG_LOG_DEFAULT_LEVEL);

#define TEMP_READ_INTERVAL_MS 5000

int main(void)
{
	wifi_init();

	if (!connect_to_wifi()) {
		return -1;
	}

	if (!ds18b20_init()) {
		return -1;
	}

	ds18b20_monitor_temperature(TEMP_READ_INTERVAL_MS);

	return 0;
}
