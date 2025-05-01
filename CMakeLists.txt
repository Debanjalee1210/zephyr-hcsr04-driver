cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(hcsr04_driver)

# Tell Zephyr where to find the custom devicetree bindings
zephyr_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dts/bindings)

# Let Zephyr detect the board overlay inside this application
set(BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(app PRIVATE src/main.c)

