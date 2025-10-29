#include <zephyr/kernel.h>
#include "ds18b20.h"
#include "ds18b20_events.h"
#include "events.h"
#include "udp_socket.h"
#include "wifi.h"

#define TEMP_READ_INTERVAL_MS 5000
#define TEMP_STACK_SIZE 1024

LOG_MODULE_REGISTER(APP_MAIN, CONFIG_LOG_DEFAULT_LEVEL);
K_THREAD_STACK_DEFINE(temp_stack, TEMP_STACK_SIZE);

static struct k_thread temp_thread_data = {0};
static k_tid_t temp_tid = NULL;
extern struct k_msgq ds18b20_msgq;

static void ds18b20_sampling_thread() {
    while (true) {
        struct ds18b20_event event = {
                .type = REQUEST,
        };
        k_msgq_put(&ds18b20_msgq, &event, K_FOREVER);
        k_msgq_get(&ds18b20_msgq, &event, K_FOREVER);
        if (event.type == RESPONSE) {
            udp_socket_send_data(&event.temperature);
        } else {
            LOG_WRN("Invalid DS18B20 event type: %d", event.type);
        }
        k_sleep(K_MSEC(TEMP_READ_INTERVAL_MS));
    }
}

void on_socket_event(const struct zbus_channel *chan) {
    if (chan != &socket_chan) {
        return;
    }

    const enum socket_status *status = zbus_chan_const_msg(chan);
    if (!status) {
        LOG_WRN("socket_chan returned NULL message pointer");
        return;
    }

    switch (*status) {
    case SOCKET_OPEN:
        if (!temp_tid) {
            LOG_INF("Starting temperature sampling thread");
            temp_tid = k_thread_create(&temp_thread_data,
                                       temp_stack,
                                       K_THREAD_STACK_SIZEOF(temp_stack),
                                       ds18b20_sampling_thread,
                                       NULL,
                                       NULL,
                                       NULL,
                                       5,
                                       0,
                                       K_NO_WAIT);
        }
        break;
    case SOCKET_CLOSED:
        if (temp_tid) {
            LOG_INF("Stopping temperature sampling thread");
            k_thread_abort(temp_tid);
            temp_tid = NULL;
        }
        break;
    default:
        LOG_WRN("Unknown socket status: %d", (int)*status);
        break;
    }
}

ZBUS_LISTENER_DEFINE(socket_listener, on_socket_event);

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

    return 0;
}
