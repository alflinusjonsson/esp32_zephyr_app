#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/socket.h>
#include <zephyr/drivers/sensor.h>
#include "udp_socket.h"

BUILD_ASSERT(sizeof(CONFIG_UDP_SERVER_ADDR) > 0,
	"CONFIG_UDP_SERVER_ADDR is empty. Please provide it to west using -DCONFIG_UDP_SERVER_ADDR=<your-server-ip> or set it in prj.conf.");
BUILD_ASSERT(sizeof(CONFIG_UDP_SERVER_PORT) > 0,
	"CONFIG_UDP_SERVER_PORT is not set or invalid. Please provide it to west using -DCONFIG_UDP_SERVER_PORT=<your-server-port> or set it in prj.conf.");

LOG_MODULE_REGISTER(APP_SOCKET, CONFIG_LOG_DEFAULT_LEVEL);

#define TEMP_MSG_SIZE 32

static int udp_sock = -1;
static struct sockaddr_in server_addr;

void udp_socket_init(void)
{
    if (udp_sock >= 0) {
        return;
    }

    udp_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udp_sock < 0) {
        LOG_ERR("Failed to create UDP socket: %s", strerror(errno));
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(CONFIG_UDP_SERVER_PORT);
    inet_pton(AF_INET, CONFIG_UDP_SERVER_ADDR, &server_addr.sin_addr);

	LOG_INF("UDP socket created");
}

void udp_socket_send_sensor_data(struct sensor_value *temp)
{
    if (udp_sock < 0) {
        LOG_ERR("UDP socket not initialized");
        return;
    }

    char msg[TEMP_MSG_SIZE];
    int len = snprintf(msg, sizeof(msg), "Temperature: %d.%06d °C", temp->val1, temp->val2);

    const ssize_t sent = sendto(udp_sock, msg, len, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (sent < 0) {
        LOG_WRN("Failed to send UDP packet: %s", strerror(errno));
    }
}
