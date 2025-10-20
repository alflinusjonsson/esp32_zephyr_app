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

#endif /* UDP_SOCKET_H */
