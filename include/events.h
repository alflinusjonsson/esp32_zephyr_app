#ifndef EVENTS_H_
#define EVENTS_H_

#include <zephyr/zbus/zbus.h>

enum socket_status { SOCKET_OPEN, SOCKET_CLOSED };

ZBUS_CHAN_DECLARE(socket_chan);
ZBUS_CHAN_DECLARE(temp_chan);

#endif /* EVENTS_H_ */
