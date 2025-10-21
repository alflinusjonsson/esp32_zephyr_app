#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/socket.h>
#include <zephyr/drivers/sensor.h>
#include "udp_socket.h"
#include "events.h"

BUILD_ASSERT(sizeof(CONFIG_UDP_SERVER_ADDR) > 0,
             "CONFIG_UDP_SERVER_ADDR is empty. Please provide it to west using "
             "-DCONFIG_UDP_SERVER_ADDR=<your-server-ip> or set it in prj.conf.");
BUILD_ASSERT(sizeof(CONFIG_UDP_SERVER_PORT) > 0,
             "CONFIG_UDP_SERVER_PORT is not set or invalid. Please provide it to west using "
             "-DCONFIG_UDP_SERVER_PORT=<your-server-port> or set it in prj.conf.");

LOG_MODULE_REGISTER(APP_SOCKET, CONFIG_LOG_DEFAULT_LEVEL);

static int udp_sock = -1;
static struct sockaddr_in server_addr = {0};

void udp_socket_open(void) {
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

    const enum socket_status status = SOCKET_OPEN;
    const int ret = zbus_chan_pub(&socket_chan, &status, K_NO_WAIT);
    if (ret < 0) {
        LOG_ERR("Failed to publish socket status, error: %s", strerror(ret));
    }
}

void udp_socket_close(void) {
    if (udp_sock < 0) {
        return;
    }

    const int ret = close(udp_sock);
    if (ret < 0) {
        LOG_ERR("Failed to close UDP socket: %s", strerror(errno));
        return;
    } else {
        LOG_INF("UDP socket closed, clearing server_addr");
        udp_sock = -1;
        memset(&server_addr, 0, sizeof(server_addr));
    }

    const enum socket_status status = SOCKET_CLOSED;
    const int pub_ret = zbus_chan_pub(&socket_chan, &status, K_NO_WAIT);
    if (pub_ret < 0) {
        LOG_ERR("Failed to publish socket status, error: %s", strerror(pub_ret));
    }
}

void on_temp_event(const struct zbus_channel *chan) {
    if (chan != &temp_chan) {
        return;
    }

    const struct sensor_value *temp = zbus_chan_const_msg(chan);
    if (!temp) {
        LOG_WRN("temp_chan returned NULL message pointer");
        return;
    }

    if (udp_sock < 0) {
        LOG_ERR("UDP socket not initialized");
        return;
    }

    const ssize_t sent = sendto(udp_sock,
                                temp,
                                sizeof(struct sensor_value),
                                0,
                                (struct sockaddr *)&server_addr,
                                sizeof(server_addr));
    if (sent < 0) {
        LOG_WRN("Failed to send UDP packet: %s", strerror(errno));
    } else {
        LOG_DBG("Sent %d bytes: val1=%d, val2=%d", (int)sent, temp->val1, temp->val2);
    }
}

void on_network_event(const struct zbus_channel *chan) {
    if (chan != &network_chan) {
        return;
    }

    const enum network_status *status = zbus_chan_const_msg(chan);
    if (!status) {
        LOG_WRN("network_chan returned NULL message pointer");
        return;
    }

    switch (*status) {
    case NETWORK_CONNECTED:
        LOG_INF("Network connected");
        udp_socket_open();
        break;
    case NETWORK_DISCONNECTED:
        LOG_INF("Network disconnected");
        udp_socket_close();
        break;
    default:
        LOG_WRN("Unknown network status: %d", *status);
        break;
    }
}

ZBUS_LISTENER_DEFINE(network_listener, on_network_event);
ZBUS_LISTENER_DEFINE(temp_listener, on_temp_event);
