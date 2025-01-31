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

## Connection Diagram

### 1. ESP32 to Arduino MEGA Connection
```
ESP32          Arduino MEGA
-----          ------------
TX (GPIO1) --> RX (Pin 10)
RX (GPIO3) --> TX (Pin 11)
GND ---------> GND
```

### 2. FQ-020 Camera to ESP32
```
Camera    ESP32
------    -----
Red   --> 5V
Black --> GND
Yellow --> GPIO36 (VP)
```

### 3. Motors to Arduino MEGA (via L298N)
```
L298N #1 (Front Motors)    Arduino MEGA
--------------------      ------------
IN1 ------------------>   Pin 2  (Front Right Forward)
IN2 ------------------>   Pin 3  (Front Right Backward)
IN3 ------------------>   Pin 4  (Front Left Forward)
IN4 ------------------>   Pin 5  (Front Left Backward)
ENA ------------------>   5V
ENB ------------------>   5V

L298N #2 (Back Motors)     Arduino MEGA
-------------------       ------------
IN1 ------------------>   Pin 6  (Back Right Forward)
IN2 ------------------>   Pin 7  (Back Right Backward)
IN3 ------------------>   Pin 8  (Back Left Forward)
IN4 ------------------>   Pin 9  (Back Left Backward)
ENA ------------------>   5V
ENB ------------------>   5V
```

### 4. Power Connections
```
Power Supply    Components
-------------   ----------
7.4V-12V -----> L298N #1 (12V)
7.4V-12V -----> L298N #2 (12V)
5V from MEGA --> ESP32 (VIN)
GND ----------> Common Ground (All components)
```

## Software Setup

1. Upload the code to Arduino MEGA:
   - Open `arduino_mega.ino` in Arduino IDE
   - Select "Arduino MEGA 2560" board
   - Select appropriate COM port
   - Click Upload

2. Upload the code to ESP32:
   - Open `esp32.ino` in Arduino IDE
   - Install ESP32 board support
   - Select "ESP32 Dev Module"
   - Select appropriate COM port
   - Click Upload

## Network Configuration

1. Default Wi-Fi Settings:
   - SSID: `RC_Car`
   - Password: `12345678`
   - IP Address: `192.168.4.1`

2. To connect:
   - Connect to the `RC_Car` Wi-Fi network
   - Open browser and navigate to:
     - `http://car.local` or
     - `http://192.168.4.1`

## Controls

- Arrow keys or on-screen buttons for movement
- Special moves:
  - Figure 8
  - Spin
  - Zigzag
- Spacebar for emergency stop

## Troubleshooting

1. If motors don't respond:
   - Check Serial connection between ESP32 and MEGA
   - Verify motor driver connections
   - Check power supply voltage

2. If camera feed is not visible:
   - Verify camera connections
   - Check if GPIO36 is receiving video signal
   - Ensure proper power supply to camera

3. If Wi-Fi connection fails:
   - Reset ESP32
   - Verify correct SSID and password
   - Check if device is within range

## Notes

- Ensure all grounds are connected to a common point
- Use adequate power supply for motors
- Keep wires organized and properly insulated
- Mount camera securely for stable video feed

## Credits

Created by Vivek W (2025)
