#ifndef UDP_SOCKET_H
#define UDP_SOCKET_H

#include <zephyr/drivers/sensor.h>

/**
 * @brief Initialize UDP socket module (register dynamic zbus observer)
 * @return true on success, false on failure
 */
bool udp_socket_init(void);

/**
 * @brief Open UDP socket
 */
void udp_socket_open(void);

/**
 * @brief Close UDP socket
 */
void udp_socket_close(void);

/**
 * @brief Send temperature data via UDP socket
 * @param temp Pointer to sensor_value struct containing temperature data
 */
void udp_socket_send_data(const struct sensor_value *temp);

#endif /* UDP_SOCKET_H */
