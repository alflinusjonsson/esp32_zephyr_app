cmake_minimum_required(VERSION 3.22)
set(BOARD esp32_devkitc)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(wifi_sta_mode)

target_sources(app PRIVATE src/main.c)
