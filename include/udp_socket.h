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

#endif /* UDP_SOCKET_H */
