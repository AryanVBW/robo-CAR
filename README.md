# RC Car Control System

A comprehensive remote-controlled car system with live camera feed and gripper control.

## Components
- Arduino MEGA 2560 R3 (Main Controller)
- OV7670 VGA CMOS Camera
- Seeed Studio XIAO ESP32S3
- MG995 Servo (Gripper Base)
- SG5050 Servo (Gripper Claw)
- L298N Motor Driver Modules (x2)
- DC Motors (x4)

## Features
- Real-time video streaming
- Web-based control interface
- Keyboard controls
- Motor control for movement
- Servo-controlled gripper system
- WiFi connectivity
- Mobile-responsive UI

## Setup Instructions

1. **Hardware Setup**
   - Follow the connection diagram in `CONNECTIONS.md`
   - Double-check all power connections
   - Ensure proper grounding

2. **Software Requirements**
   - Arduino IDE
   - Required Libraries:
     - `Servo.h`
     - `Wire.h`
     - `WiFi.h`
     - `WebServer.h`
     - `DNSServer.h`
     - `ESPmDNS.h`

3. **Installation**
   - Upload `arduino_mega.ino` to Arduino MEGA
   - Upload `esp32.ino` to ESP32S3
   - Power up the system

4. **Accessing the Control Interface**
   - Connect to WiFi network "RC_Car" (password: 12345678)
   - Open browser and navigate to http://rccar.local
   - If DNS resolution fails, use the IP address shown on ESP32's serial monitor

## Controls

### Web Interface
- Arrow buttons for movement
- Dedicated gripper controls
- Live camera feed display

### Keyboard Controls
- Arrow keys: Movement control
- Spacebar: Stop
- 'G': Close gripper
- 'O': Open gripper
- 'U': Move base up
- 'D': Move base down
- 'N': Neutral position

## Troubleshooting

1. **No Video Feed**
   - Check camera connections
   - Verify power supply
   - Refresh browser page

2. **Motors Not Responding**
   - Check L298N connections
   - Verify motor power supply
   - Check serial communication between ESP32 and Arduino

3. **Gripper Issues**
   - Verify servo power supply
   - Check servo signal connections
   - Ensure servos are properly calibrated

## Safety Notes
- Always operate on a stable power supply
- Keep the system away from water
- Monitor motor temperature
- Don't exceed servo limits

## Contributing
Feel free to submit issues and enhancement requests!

## License
MIT License - feel free to use and modify as needed.

## Contact
For support or questions, please open an issue in the repository.
