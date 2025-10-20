#include <zephyr/zbus/zbus.h>
#include <zephyr/drivers/sensor.h>
#include "events.h"

ZBUS_CHAN_DEFINE(network_chan,
    enum network_status,
    NULL,
    NULL,
    ZBUS_OBSERVERS(network_listener),
    ZBUS_MSG_INIT(NETWORK_DISCONNECTED)
);

ZBUS_CHAN_DEFINE(socket_chan,
    enum socket_status,
    NULL,
    NULL,
    ZBUS_OBSERVERS(socket_listener),
    ZBUS_MSG_INIT(SOCKET_CLOSED)
);

ZBUS_CHAN_DEFINE(temp_chan,
    struct sensor_value,
    NULL,
    NULL,
    ZBUS_OBSERVERS(temp_listener),
    ZBUS_MSG_INIT(.val1 = 0, .val2 = 0)
);
