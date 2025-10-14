#ifndef DS18B20_H
#define DS18B20_H

#include <zephyr/drivers/sensor.h>

/**
 * @brief Initialize the DS18B20 temperature sensor
 * @return true if initialization was successful, false otherwise
 */
bool ds18b20_init(void);

/**
 * @brief Read temperature from the DS18B20 sensor
 * @param temp Pointer to store the temperature reading
 * @return 0 on success, negative error code on failure
 */
int ds18b20_read_temperature(struct sensor_value *temp);

/**
 * @brief Monitors temperature
 */
void ds18b20_monitor_temperature(void);

#endif /* DS18B20_H */
