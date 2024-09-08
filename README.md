# Energy Meter with PZEM-004T

Welcome to the Energy Meter project! This repository contains an Arduino-based energy meter using the PZEM-004T v3.0 power monitoring module. This project allows you to measure voltage, current, power, energy consumption, and more, making it an ideal solution for home automation and energy monitoring applications.

## Features

- Measure voltage, current, power, energy, frequency, and power factor.
- Supports multiple devices on the same bus.
- Easy integration with ESP32 and other Arduino-compatible boards.
- Example sketches provided to get you started quickly.

## Getting Started

### Prerequisites

- Arduino IDE or PlatformIO
- ESP32 or compatible microcontroller
- PZEM-004T v3.0 module
- Jumper wires for connections

### Installation

1. Clone this repository:
   ```bash
   git clone https://github.com/yourusername/energy-meter-esp32-hlw8032.git
   cd energy-meter-esp32-hlw8032
   ```

2. Install the PZEM-004T library:
   - You can find the library [here](https://github.com/mandulaj/PZEM-004T-v30) and include it in your Arduino IDE or PlatformIO project.

3. Open the example sketch of your choice from the `vendor/PZEM004Tv30/examples` directory in the Arduino IDE or PlatformIO.

4. Upload the sketch to your ESP32 board.

### Wiring Diagram

Connect the PZEM-004T module to your ESP32 as follows:

- **PZEM-004T VCC** to **ESP32 5V**
- **PZEM-004T GND** to **ESP32 GND**
- **PZEM-004T TX** to **ESP32 RX (GPIO 16 by default)**
- **PZEM-004T RX** to **ESP32 TX (GPIO 17 by default)**

### Example Usage

Refer to the example sketches provided in the `vendor/PZEM004Tv30/examples` directory for various use cases, including:

- **PZEMChangeAddress**: Change the address of the PZEM-004T module.
- **PZEMHardSerial**: Use hardware serial for communication.
- **PZEMSoftwareSerial**: Use software serial for communication.
- **PZEMMultiDevice**: Communicate with multiple PZEM-004T modules.

## License

This project is licensed under the MIT License. See the [LICENSE](vendor/PZEM004Tv30/LICENSE) file for details.

## Contributing

Contributions are welcome! Please feel free to submit a pull request or open an issue for any enhancements or bug fixes.

## Acknowledgments

- [Jakub Mandula](https://github.com/mandulaj) for the PZEM-004T library.
- The open-source community for their contributions and support.

## Contact

For any inquiries or support, please reach out via the issues section of the repository.
