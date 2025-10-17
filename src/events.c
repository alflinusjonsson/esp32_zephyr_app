#include <zephyr/zbus/zbus.h>
#include "events.h"

ZBUS_CHAN_DEFINE(network_chan,
    enum network_status,
    NULL,
    NULL,
    ZBUS_OBSERVERS(network_listener),
    ZBUS_MSG_INIT(NETWORK_DISCONNECTED)
);
