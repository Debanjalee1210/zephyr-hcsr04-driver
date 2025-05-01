/**
 * @file    main.c
 * @brief   HC-SR04 Ultrasonic Distance Sensor driver example using Zephyr RTOS
 * @details This program interfaces with the HC-SR04 sensor connected to a
 *          Nucleo-F401RE board via GPIO pins defined in the devicetree overlay.
 *          It sends a 10 µs trigger pulse and measures the echo pulse width to
 *          calculate the distance in centimeters.
 *
 * @author  Debanjalee Roy
 * @date    2025-05-01
 */ 

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

// Get device tree aliases for TRIG and ECHO pins
#define TRIG_NODE DT_ALIAS(trig)
#define ECHO_NODE DT_ALIAS(echo)

// Check if aliases are correctly defined in the overlay
#if !DT_NODE_HAS_STATUS(TRIG_NODE, okay) || !DT_NODE_HAS_STATUS(ECHO_NODE, okay)
#error "Unsupported board: trig or echo devicetree alias is not defined"
#endif

// Configure the GPIO spec structures for TRIG and ECHO
const struct gpio_dt_spec trig = GPIO_DT_SPEC_GET(TRIG_NODE, gpios);
const struct gpio_dt_spec echo = GPIO_DT_SPEC_GET(ECHO_NODE, gpios);

/**
 * @brief Main application entry point.
 *
 * This loop sends a trigger pulse and times the echo pulse duration using
 * cycle counters. The distance is calculated and printed via UART.
 *
 * @return int Return code (not used).
 */

int main(void)
{
    int ret;

    printk("Starting HC-SR04 demo\n");

    // Configure TRIG pin as output and set to low
    ret = gpio_pin_configure_dt(&trig, GPIO_OUTPUT_INACTIVE);
    if (ret < 0) {
        printk("Error configuring trig pin\n");
        return -1;
    }

    // Configure ECHO pin as input
    ret = gpio_pin_configure_dt(&echo, GPIO_INPUT);
    if (ret < 0) {
        printk("Error configuring echo pin\n");
        return -1;
    }

    while (1) {
        uint32_t start = 0, end = 0;

        // Send 10µs pulse on TRIG pin to initiate measurement
        gpio_pin_set_dt(&trig, 1);
        k_busy_wait(10);
        gpio_pin_set_dt(&trig, 0);

        // Wait for ECHO pin to go high and record the start cycle
        while (gpio_pin_get_dt(&echo) == 0) {
            start = k_cycle_get_32();
        }

        // Wait for ECHO pin to go low and record the end cycle
        while (gpio_pin_get_dt(&echo) == 1) {
            end = k_cycle_get_32();
        }

        // Calculate duration and convert to nanoseconds
        uint32_t cycles = end - start;
        uint64_t ns = k_cyc_to_ns_floor64(cycles);

	// Convert time to distance in cm (Speed of sound = 343 m/s)
        float distance_cm = ns / 58000.0f;
	
	// Print distance with two decimal places
	printk("Distance: %d.%02d cm\n", (int)distance_cm, (int)(distance_cm * 100) % 100);

	// Delay before next measurement
        k_sleep(K_MSEC(1000));
    }
    return 0;
}
