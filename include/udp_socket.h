#ifndef UDP_SOCKET_H
#define UDP_SOCKET_H

#include <zephyr/drivers/sensor.h>

/**
 * @brief Open UDP socket
 */
void udp_socket_open(void);

/**
 * @brief Close UDP socket
 */
void udp_socket_close(void);

/**
 * @brief Send sensor data over UDP
 * @param temp Pointer to the sensor_value structure containing temperature data
 */
void udp_socket_send_sensor_data(struct sensor_value *temp);

#endif /* UDP_SOCKET_H */
