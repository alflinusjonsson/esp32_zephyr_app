#include "led.h"
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#define STACKSIZE 1024
#define THREAD0_PRIORITY 7
#define LED_TOGGLE_INTERVAL_MS 2000

LOG_MODULE_REGISTER(APP_LED, CONFIG_LOG_DEFAULT_LEVEL);

K_THREAD_DEFINE(led_thread_id, STACKSIZE, led_toggle, NULL, NULL, NULL, THREAD0_PRIORITY, 0, 0);

static const struct gpio_dt_spec red_led = GPIO_DT_SPEC_GET(DT_NODELABEL(red_led), gpios);

bool led_init(void) {
    if (!device_is_ready(red_led.port)) {
        LOG_ERR("LED device not ready");
        return false;
    }

    const int ret = gpio_pin_configure_dt(&red_led, GPIO_OUTPUT_HIGH);
    if (ret < 0) {
        return false;
    }

    LOG_INF("LED device initialized");

    return true;
}

void led_toggle(void) {
    while (true) {
        const int ret = gpio_pin_toggle_dt(&red_led);
        if (ret < 0) {
            LOG_ERR("Failed to toggle LED: %s", strerror(errno));
        }

        k_sleep(K_MSEC(LED_TOGGLE_INTERVAL_MS));
    }
}
