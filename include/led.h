#ifndef LED_H
#define LED_H

#include <stdbool.h>

/**
 * @brief Initialize the LED.
 * @return true if initialization was successful, false otherwise.
 */
bool led_init(void);

/**
 * @brief Toggle the LED state at specified intervals.
 */
void led_toggle();

#endif // LED_H
