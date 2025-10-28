#ifndef DS18B20_H
#define DS18B20_H

#include <stdbool.h>

/**
 * @brief Initialize the DS18B20 temperature sensor
 * @return true if initialization was successful, false otherwise
 */
bool ds18b20_init(void);

/**
 * @brief Read temperature from the DS18B20 sensor
 */
void ds18b20_read_temperature(void);

#endif /* DS18B20_H */
