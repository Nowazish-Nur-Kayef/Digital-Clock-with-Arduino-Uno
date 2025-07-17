# Digital Clock with Arduino Uno

[![Version](https://img.shields.io/badge/version-2.0.0-blue.svg)](https://github.com/Nowazish-Nur-Kayef/Digital-Clock-with-Arduino-Uno/releases)
[![Arduino](https://img.shields.io/badge/Arduino-Uno-00979D.svg)](https://www.arduino.cc/)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)

A digital clock project built with Arduino Uno featuring both basic display and interactive alarm systems. Available in two versions: V1.0 (basic display) and V2.0 (interactive with button controls).

## 🚀 Available Versions

### 📟 [Version 1.0 - Basic Digital Clock](DigitalClock/)
A simple digital clock with LCD display and predefined alarms. Perfect for beginners learning Arduino basics.

**Features:**
- 16x2 LCD time display
- Static alarm system
- Basic startup animation
- Buzzer notifications

### ⚡ [Version 2.0 - Interactive Alarm Clock](DigitalClockV2/) **← Latest!**
Advanced interactive system with button controls and dynamic alarm management. Ideal for daily use and advanced learning.

**Features:**
- 4-button interactive interface
- Real-time time adjustment
- Quick alarm setting (20-min timer)
- Dynamic alarm management
- Professional user interface
- Comprehensive documentation

**[📖 Read V2.0 Documentation](README_V2.md) | [📋 View Changelog](CHANGELOG.md)**

## 📋 Table of Contents
- [Available Versions](#-available-versions)
- [Quick Start Guide](#-quick-start-guide)
- [Version Comparison](#-version-comparison)
- [Overview](#overview)
- [Features](#features)
- [Components Required](#components-required)
- [Circuit Diagram](#circuit-diagram)
- [Installation](#installation)
- [Setup Instructions](#setup-instructions)
- [Usage](#usage)
- [Code Structure](#code-structure)
- [Library Dependencies](#library-dependencies)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [License](#license)

## 🚀 Quick Start Guide

### For Beginners (Recommended: V1.0)
1. **Get familiar** with basic Arduino and electronics concepts
2. **Build V1.0** to understand the fundamentals
3. **Learn** LCD interfacing and basic alarm logic
4. **Upgrade** to V2.0 when ready for interactive features

### For Experienced Users (Recommended: V2.0)
1. **Jump to V2.0** for full interactive experience
2. **Add 4 buttons** to the basic circuit
3. **Upload V2.0 sketch** for immediate functionality
4. **Customize** alarms and settings as needed

### Migration Path: V1.0 → V2.0
- **Hardware**: Add 4 push buttons to pins 2, 3, 4, 5
- **Software**: Upload V2.0 sketch (backward compatible)
- **No changes** to existing LCD and buzzer connections
- **Optional**: Use example sketches to test new features

## 📊 Version Comparison

| Feature | V1.0 Basic | V2.0 Interactive |
|---------|------------|------------------|
| **Difficulty** | Beginner | Intermediate |
| **Components** | 3 (LCD, Buzzer, Arduino) | 7 (+ 4 Buttons) |
| **Time Control** | Code only | Real-time buttons |
| **Alarms** | Static only | Static + Dynamic |
| **User Interface** | Basic display | Professional messages |
| **Learning Value** | Arduino basics | Advanced programming |
| **Daily Usage** | Display only | Fully functional |
| **Documentation** | Standard | Comprehensive |

**💡 Recommendation:** Start with V1.0 if new to Arduino, or jump to V2.0 for immediate practical use.

## 🔍 Overview

This project creates functional digital clocks using Arduino Uno with alarm capabilities. Available in two versions to suit different skill levels and use cases:

**Version 1.0**: A foundational digital clock with LCD display and static alarms. Perfect for learning Arduino basics, LCD interfacing, and simple alarm logic.

**Version 2.0**: An advanced interactive alarm clock with button controls, dynamic alarms, and professional user interface. Ideal for daily use and advanced Arduino programming education.

Both versions feature I2C LCD displays and buzzer notifications, with comprehensive documentation and example code for educational purposes.


## ✨ Features

### Core Features (Both Versions)
- **Digital Time Display**: Shows current time on 16x2 LCD display
- **Alarm Functionality**: Set and trigger alarms with buzzer notification
- **I2C Communication**: Efficient LCD control using I2C interface
- **Low Power Consumption**: Optimized for continuous operation
- **Educational Value**: Comprehensive documentation and learning materials

### V1.0 Specific Features
- **Static Alarm System**: Predefined alarm times in code
- **Basic Display**: Simple time display with startup animation
- **Beginner Friendly**: Perfect introduction to Arduino programming

### V2.0 Enhanced Features
- **Interactive Controls**: 4-button interface for real-time control
- **Dynamic Alarms**: User-settable alarms with quick 20-minute timer
- **Advanced UI**: Professional messages and status indicators
- **Smart Alarm Logic**: Dual alarm system with intelligent management
- **Real-time Adjustment**: Modify time without code changes
- **Example Sketches**: Button testing and alarm demonstration utilities

## 🛠️ Components Required

| Component | Quantity | Description |
|-----------|----------|-------------|
| Arduino Uno | 1 | Main microcontroller board |
| LCD 16x2 I2C | 1 | Display module with I2C backpack |
| Buzzer (5V) | 1 | Alarm sound output |
| Male to Female Jumper Wires | 6 | For connections |

### Additional Materials
- Breadboard (optional, for prototyping)
- USB cable (for programming and power)
- 9V battery with connector (optional, for portable operation)

## 📐 Circuit Diagram

![Circuit Design](CircuitDesign/Circuit_Design.png)

### Pin Connections

| Arduino Uno Pin | Component | Connection |
|----------------|-----------|------------|
| 5V | LCD VCC | Power supply |
| GND | LCD GND | Ground |
| A4 (SDA) | LCD SDA | I2C Data |
| A5 (SCL) | LCD SCL | I2C Clock |
| Digital Pin 8 | Buzzer Positive | Signal |
| GND | Buzzer Negative | Ground |

## 💾 Installation

### 1. Clone the Repository
```bash
git clone https://github.com/Nowazish-Nur-Kayef/Digital-Clock-with-Arduino-Uno.git
cd Digital-Clock-with-Arduino-Uno
```

### 2. Install Arduino IDE
Download and install the Arduino IDE from [arduino.cc](https://www.arduino.cc/en/software)

### 3. Install Required Libraries
1. Open Arduino IDE
2. Go to **Sketch** > **Include Library** > **Add .ZIP Library**
3. Select the `LiquidCrystal_I2C-1.1.3.zip` file from the repository
4. Alternatively, install via Library Manager:
   - Go to **Tools** > **Manage Libraries**
   - Search for "LiquidCrystal I2C"
   - Install the library by Frank de Brabander

## 🔧 Setup Instructions

### Hardware Setup
1. **Connect the LCD Display**:
   - VCC to Arduino 5V
   - GND to Arduino GND
   - SDA to Arduino A4
   - SCL to Arduino A5

2. **Connect the Buzzer**:
   - Positive terminal to Arduino Digital Pin 8
   - Negative terminal to Arduino GND

3. **Verify Connections**: Double-check all connections match the circuit diagram

### Software Setup
1. Open the `.ino` file in Arduino IDE
2. Select your Arduino board: **Tools** > **Board** > **Arduino Uno**
3. Select the correct port: **Tools** > **Port** > [Your Arduino Port]
4. Upload the code to your Arduino

## 🚀 Usage

### Basic Operation
1. **Power On**: Connect Arduino to power source via USB or external adapter
2. **Initial Display**: The LCD will show the current time (starts from 00:00:00)
3. **Time Setting**: Modify the code to set initial time or implement buttons for time adjustment

### Setting Alarms
- The alarm time can be configured in the code
- When alarm triggers, the buzzer will sound
- Alarm can be dismissed by resetting or through code modification

### Customization
- Modify display format in the code
- Adjust alarm duration and tone
- Add multiple alarm functionality
- Implement snooze feature

## 📁 Code Structure

```
Digital-Clock-with-Arduino-Uno/
├── DigitalClock/                    # V1.0 - Basic Version
│   └── DigitalClock.ino            # Basic digital clock sketch
├── DigitalClockV2/                 # V2.0 - Interactive Version  
│   └── DigitalClockV2.ino          # Advanced interactive sketch
├── examples/                       # Example sketches and demos
│   ├── button_test.ino             # Button connection verification
│   └── alarm_demo.ino              # Feature demonstration
├── Dependencies/                   # Required libraries
│   └── LiquidCrystal_I2C-1.1.3.zip # I2C LCD library
├── CircuitDesign/                  # Circuit diagrams and schematics
│   └── Circuit_Design.png          # Basic circuit design
├── README_V2.md                    # Comprehensive V2.0 documentation
├── CHANGELOG.md                    # Version history and changes
├── LICENSE                         # MIT License
└── README.md                       # Main project documentation
```

### Key Functions (V1.0)
- `setup()`: Initialize LCD, set pins, configure initial settings
- `loop()`: Main program loop handling time updates and alarm checks
- `updateTime()`: Increment time with rollover handling
- `checkAlarms()`: Monitor and trigger static alarms

### Key Functions (V2.0)
- `setup()`: Enhanced initialization with button configuration
- `loop()`: Non-blocking main loop with button handling
- `handleButtons()`: Debounced button input processing
- `handleButtonPress()`: Individual button action processing
- `updateTime()`: Time increment with alarm reset logic
- `checkAlarms()`: Dual alarm system (static + dynamic)
- `showMessage()`: Professional message display system
- `updateDisplay()`: Advanced LCD management with status indicators

## 📚 Library Dependencies

### LiquidCrystal I2C v1.1.3
- **Purpose**: Controls the I2C LCD display
- **Installation**: Included in repository as ZIP file
- **Documentation**: Provides functions for LCD initialization, cursor control, and text display

## 🔧 Troubleshooting

### Common Issues

#### LCD Not Displaying
- **Check Connections**: Verify SDA and SCL connections
- **I2C Address**: Scan for correct I2C address (usually 0x27 or 0x3F)
- **Power Supply**: Ensure 5V power is connected properly

#### Buzzer Not Working
- **Pin Configuration**: Verify buzzer is connected to correct digital pin
- **Code Check**: Ensure buzzer pin is set as OUTPUT in setup()
- **Voltage**: Confirm buzzer is receiving proper voltage

#### Upload Errors
- **Port Selection**: Check correct COM port is selected
- **Board Type**: Verify Arduino Uno is selected as board type
- **Cable Issues**: Try different USB cable

### Debug Tips
- Use Serial Monitor for debugging: `Serial.begin(9600)`
- Add debug prints to track program flow
- Test components individually before integration

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request. For major changes:

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

### Improvement Ideas

#### For V1.0 Enhancements
- Add RTC module for accurate timekeeping
- Improve startup animation and display effects
- Add more predefined alarm options
- Create custom LCD characters

#### For V2.0 Enhancements  
- ✅ **Completed**: Button controls for time/alarm setting
- Add EEPROM storage for persistent settings
- Implement snooze functionality with dedicated button
- Add temperature display with sensor integration
- Create web interface for remote control via WiFi
- Support for multiple user-defined alarms
- Add brightness control for LCD backlight
- Implement different alarm tones/patterns

#### For V3.0 Future Features
- WiFi connectivity and IoT integration
- Mobile app for remote control
- Multiple timezone support
- Voice control integration
- Sleep tracking and smart wake-up
- Integration with home automation systems

## 📄 License

This project is open source and available under the [MIT License](LICENSE).

## 👨‍💻 Author

**Nowazish Nur Kayef**
- GitHub: [@Nowazish-Nur-Kayef](https://github.com/Nowazish-Nur-Kayef)

## 🙏 Acknowledgments

- Arduino community for extensive documentation and examples
- LiquidCrystal library contributors
- Electronics enthusiasts who share their knowledge online

---

### 📞 Support

If you encounter any issues or have questions about this project, please [open an issue](https://github.com/Nowazish-Nur-Kayef/Digital-Clock-with-Arduino-Uno/issues) on GitHub.

**Happy Building! 🔧⚡**
