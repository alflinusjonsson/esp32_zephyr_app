#include "events.h"
#include <zephyr/drivers/sensor.h>
#include <zephyr/zbus/zbus.h>

ZBUS_CHAN_DEFINE(socket_chan,
                 enum socket_status,
                 NULL,
                 NULL,
                 ZBUS_OBSERVERS(socket_listener),
                 ZBUS_MSG_INIT(SOCKET_CLOSED));
