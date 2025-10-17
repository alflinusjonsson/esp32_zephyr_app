#ifndef EVENTS_H_
#define EVENTS_H_

#include <zephyr/zbus/zbus.h>

enum network_status {
    NETWORK_CONNECTED,
    NETWORK_DISCONNECTED
};

ZBUS_CHAN_DECLARE(network_chan);

#endif /* EVENTS_H_ */
