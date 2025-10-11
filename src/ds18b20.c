#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>
#include "ds18b20.h"
#include "udp_socket.h"

LOG_MODULE_REGISTER(APP_DS18B20, CONFIG_LOG_DEFAULT_LEVEL);

static const struct device *ds18b20_dev;

bool ds18b20_init(void)
{
    ds18b20_dev = DEVICE_DT_GET(DT_NODELABEL(ds18b20));

    if (!device_is_ready(ds18b20_dev)) {
        LOG_ERR("DS18B20 device not ready");
        return false;
    }

    LOG_INF("DS18B20 temperature sensor initialized");
    return true;
}

int ds18b20_read_temperature(struct sensor_value *temp)
{
    if (!ds18b20_dev) {
        LOG_ERR("DS18B20 not initialized");
        return -ENODEV;
    }

    if (!temp) {
        LOG_ERR("Invalid temperature pointer");
        return -EINVAL;
    }

    int ret = sensor_sample_fetch(ds18b20_dev);
    if (ret < 0) {
        LOG_ERR("Failed to fetch sensor sample: %d", ret);
        return ret;
    }

    ret = sensor_channel_get(ds18b20_dev, SENSOR_CHAN_AMBIENT_TEMP, temp);
    if (ret < 0) {
        LOG_ERR("Failed to get temperature: %d", ret);
        return ret;
    }

    LOG_DBG("Temperature: %d.%06d °C", temp->val1, temp->val2);
    return 0;
}

bool ds18b20_is_ready(void)
{
    return ds18b20_dev != NULL && device_is_ready(ds18b20_dev);
}

void ds18b20_monitor_temperature(const uint16_t read_interval_ms)
{
    LOG_INF("Starting temperature monitoring...");

    while (true) {
        struct sensor_value temp;

        const int ret = ds18b20_read_temperature(&temp);
        if (ret < 0) {
            LOG_ERR("Failed to read temperature: %d", ret);
            continue;
        }

        udp_socket_send_sensor_data(&temp);
        k_sleep(K_MSEC(read_interval_ms));
    }
}
