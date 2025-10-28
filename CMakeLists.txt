cmake_minimum_required(VERSION 3.22)
set(ZEPHYR_EXTRA_MODULES "$ENV{HOME}/esp32_zephyr_wifi_lib")
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(wifi_sta_mode)

target_include_directories(app PRIVATE
    include
)

target_sources(app PRIVATE
    src/main.c
    src/ds18b20.c
    src/udp_socket.c
    src/led.c
    src/events.c
)
