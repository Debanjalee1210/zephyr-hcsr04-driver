# HC-SR04 Ultrasonic Sensor Driver using Zephyr RTOS

This project demonstrates how to interface the **HC-SR04 ultrasonic distance sensor** with an **STM32 Nucleo-F401RE** board using **Zephyr RTOS**. It leverages Zephyr's GPIO API and provides a basic loop that sends trigger pulses and measures echo duration to compute distance.It measures distances and prints the result over UART every second using `printk()`.

---

## Features

- Measures distance (in cm) using accurate pulse timing
- Uses **Devicetree overlays** to define TRIG and ECHO pins
- Leverages `gpio_dt_spec` abstraction for portability
- Simple single-threaded polling loop
- Outputs readable UART logs using `picocom` or `screen`

---

## Hardware Connections

| HC-SR04 Pin | Connects To     | Function     |
|-------------|------------------|--------------|
| VCC         | 5V (Nucleo)      | Sensor Power |
| GND         | GND (Nucleo)     | Ground       |
| **TRIG**    | **PA9**          | Trigger GPIO |
| **ECHO**    | **PA10**         | Echo GPIO    |

> Pin PA9 and PA10 are **D8** and **D2** on Nucleo-F401RE headers respectively.

---

## Project Structure

```
hcsr04_driver/
├── CMakeLists.txt                  # Zephyr project definition
├── prj.conf                        # Kernel and driver config
├── src/
│   └── main.c                      # Core logic for pulse timing
├── boards/
│   └── nucleo_f401re.overlay       # Devicetree pin mapping
├── dts/
│   └── bindings/
│       └── custom/
│           └── gpio-node.yaml     # Dummy node to bind gpios
```
---

## Build and Flash
```
# Build the application
west build -b nucleo_f401re hcsr04_driver -p

# Flash to board using OpenOCD or STM32CubeProgrammer
west flash

```
---
### **Serial Output (via UART)**

Use `picocom`:

```
sudo picocom /dev/ttyACM0 -b 115200

```
---

### **Sample Output**

```
*** Booting Zephyr OS build ***
Starting HC-SR04 demo
Distance: 5.27 cm
Distance: 6.91 cm
Distance: 13.22 cm
...
```