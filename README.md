# Wi-Fi Controlled RC Car with Camera

A remote-controlled car project using Arduino MEGA 2560 R3 and ESP32, featuring live camera streaming and a sci-fi themed web interface.

## Hardware Requirements

- Arduino MEGA 2560 R3
- ESP32 Development Board
- FQ-020 Analog Camera
- 4x DC Motors with wheels
- 2x L298N Motor Driver Modules
- Power Supply (7.4V-12V)
- Jumper Wires
- Chassis (with motor mounts)

## Pin Configurations

### 1. ESP32 to Arduino MEGA Connection
```
ESP32          Arduino MEGA    Description
-----          ------------    -----------
TX2 (GPIO17) -> RX (Pin 10)   Commands from ESP32 to MEGA
RX2 (GPIO16) -> TX (Pin 11)   Feedback from MEGA to ESP32 (if needed)
GND ---------> GND            Common ground
```

### 2. FQ-020 Camera to ESP32
```
Camera    ESP32
------    -----
Red   --> 5V
Black --> GND
Yellow --> GPIO36 (VP)    Analog video input
```

### 3. Arduino MEGA Motor Control Pins
```
Arduino MEGA Pin    Connection    Description
----------------    ----------    -----------
Pin 2 ----------->  L298N #1 IN1  Front Right Forward
Pin 3 ----------->  L298N #1 IN2  Front Right Backward
Pin 4 ----------->  L298N #1 IN3  Front Left Forward
Pin 5 ----------->  L298N #1 IN4  Front Left Backward
Pin 6 ----------->  L298N #2 IN1  Back Right Forward
Pin 7 ----------->  L298N #2 IN2  Back Right Backward
Pin 8 ----------->  L298N #2 IN3  Back Left Forward
Pin 9 ----------->  L298N #2 IN4  Back Left Backward
```

### 4. L298N Motor Driver Configuration
```
L298N          Power/Control
-----          -------------
12V   -------> 7.4V-12V Power Supply
GND   -------> Common Ground
ENA   -------> 5V (or PWM for speed control)
ENB   -------> 5V (or PWM for speed control)
```

## Software Setup

### ESP32 Configuration
1. Install Required Libraries:
   - ESP32 Board Support
   - WebServer
   - WiFi

2. Upload Settings:
   - Board: "ESP32 Dev Module"
   - Upload Speed: "115200"
   - Flash Frequency: "80MHz"
   - Core Debug Level: "None"

### Arduino MEGA Configuration
1. Upload Settings:
   - Board: "Arduino MEGA 2560"
   - Processor: "ATmega2560"
   - Port: Select appropriate COM port
   - Programmer: "AVRISP mkII"

## Communication Protocol

### Serial Communication
- Baud Rate: 115200
- Hardware Serial used on both boards
- ESP32: Using Serial2 (TX2/RX2)
- Arduino MEGA: Using Hardware Serial

### Command Codes
```
Command    Function
-------    --------
'F'        Forward
'B'        Backward
'L'        Left Turn
'R'        Right Turn
'S'        Stop
'8'        Figure 8 Pattern
'P'        Spin
'Z'        Zigzag Pattern
```

## Network Configuration

1. Default Wi-Fi Settings:
   - SSID: `RC_Car`
   - Password: `12345678`
   - IP Address: `192.168.4.1`

2. Web Interface:
   - URL: `http://192.168.4.1`
   - Mobile-responsive design
   - Camera feed with sci-fi overlay
   - Touch-friendly controls

## Troubleshooting

1. Communication Issues:
   - Verify TX2/RX2 connections
   - Check baud rate (115200)
   - Ensure common ground connection

2. Motor Control:
   - Verify L298N power connections
   - Check motor pin assignments
   - Test each motor individually

3. Camera Feed:
   - Check analog input connection
   - Verify power supply
   - Ensure proper ground connection

## Operation Instructions

1. Power up sequence:
   - Connect power supply
   - Power on Arduino MEGA
   - ESP32 will create Wi-Fi network

2. Connect to RC Car:
   - Join `RC_Car` Wi-Fi network
   - Open web browser
   - Navigate to `http://192.168.4.1`

3. Controls:
   - Use on-screen buttons
   - Arrow keys on keyboard
   - Special move buttons
   - Emergency stop (Spacebar)

## Credits

Created by Vivek W (2025)
Version: 1.0
