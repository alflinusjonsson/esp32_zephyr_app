#include "ds18b20.h"
#include "events.h"
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#define TEMP_READ_INTERVAL_MS 5000
#define TEMP_STACK_SIZE 1024

LOG_MODULE_REGISTER(APP_DS18B20, CONFIG_LOG_DEFAULT_LEVEL);
K_THREAD_STACK_DEFINE(temp_stack, TEMP_STACK_SIZE);

static const struct device *ds18b20;
static struct k_thread temp_thread_data = {0};
static k_tid_t temp_tid = NULL;

static void ds18b20_sampling_thread() {
    while (true) {
        ds18b20_read_temperature();
        k_sleep(K_MSEC(TEMP_READ_INTERVAL_MS));
    }
}

bool ds18b20_init(void) {
    ds18b20 = DEVICE_DT_GET(DT_NODELABEL(ds18b20));

    if (!device_is_ready(ds18b20)) {
        LOG_ERR("DS18B20 device not ready");
        return false;
    }

    LOG_INF("DS18B20 temperature sensor initialized");
    return true;
}

void ds18b20_read_temperature(void) {
    int ret = sensor_sample_fetch(ds18b20);
    if (ret < 0) {
        LOG_ERR("Failed to fetch sensor sample: %s", strerror(errno));
        return;
    }

    struct sensor_value temp;
    ret = sensor_channel_get(ds18b20, SENSOR_CHAN_AMBIENT_TEMP, &temp);
    if (ret < 0) {
        LOG_ERR("Failed to get temperature: %s", strerror(errno));
        return;
    }

    LOG_INF("Temperature: %d.%06d °C", temp.val1, temp.val2);

    ret = zbus_chan_pub(&temp_chan, &temp, K_NO_WAIT);
    if (ret < 0) {
        LOG_ERR("Failed to publish temperature, error: %s", strerror(ret));
        return;
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
