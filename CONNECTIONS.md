# Car Control System Connections

## Component Overview
1. Arduino MEGA 2560 R3 (Main Controller)
2. OV7670 VGA CMOS Camera
3. Seeed Studio XIAO ESP32S3 (WiFi + Camera Interface)
4. MG995 Servo (Gripper Base)
5. SG5050 Servo (Gripper Claw)
6. L298N Motor Drivers (x2)

## Pin Connections

### OV7670 Camera to ESP32S3
- SIOC (SCL) → GPIO7 (SCL)
- SIOD (SDA) → GPIO6 (SDA)
- VSYNC → GPIO4
- HREF → GPIO5
- PCLK → GPIO8
- XCLK → GPIO9
- D7 → GPIO10
- D6 → GPIO11
- D5 → GPIO12
- D4 → GPIO13
- D3 → GPIO14
- D2 → GPIO15
- D1 → GPIO16
- D0 → GPIO17
- RESET → GPIO18
- PWDN → GPIO19
- 3.3V → 3.3V
- GND → GND

### ESP32S3 to Arduino MEGA
- TX (GPIO43) → RX1 (Pin 19)
- RX (GPIO44) → TX1 (Pin 18)
- 3.3V → 3.3V
- GND → GND

### Servo Motors to Arduino MEGA
- MG995 (Base) Signal → Pin 11
- SG5050 (Claw) Signal → Pin 12
- VCC → 5V
- GND → GND

### L298N Motor Driver #1 (Left Motors)
- ENA → Pin 2
- IN1 → Pin 3
- IN2 → Pin 4
- IN3 → Pin 5
- IN4 → Pin 6
- ENB → Pin 7
- 12V → External Power Supply (+)
- GND → External Power Supply (-)
- GND → Arduino GND

### L298N Motor Driver #2 (Right Motors)
- ENA → Pin 8
- IN1 → Pin 9
- IN2 → Pin 10
- IN3 → Pin 13
- IN4 → Pin 14
- ENB → Pin 15
- 12V → External Power Supply (+)
- GND → External Power Supply (-)
- GND → Arduino GND

## Power Supply Requirements

### Main Power Source
- Battery: Lithium U2 Series
  - Capacity: 3300mAh
  - Voltage: 11.1V (3S LiPo)
  - Burst Rate: 70C
  - Energy: 36.63 Wh
  - Maximum Burst Current: 231A (3.3Ah × 70C)
  - Continuous Discharge: ~35C (115.5A)

### Power Distribution
1. **Arduino MEGA 2560 R3**
   - Input Voltage: 11.1V from battery via voltage regulator
   - Operating Voltage: 5V (internal regulator)
   - Current Draw: ~100mA typical
   - Connection: VIN pin for raw battery voltage

2. **Motor Drivers (L298N)**
   - Input Voltage: 11.1V direct from battery
   - Maximum Current: 2A per channel
   - Logic Voltage: 5V from Arduino
   - Ensure thick wires (16-18 AWG) for battery connection
   - Add 470μF capacitor across power input

3. **Servo Motors**
   - Supply: Dedicated 5V BEC (Battery Elimination Circuit)
   - Current: Up to 1A per servo
   - DO NOT power from Arduino's 5V

4. **ESP32S3**
   - Power via USB-C or
   - 5V from dedicated BEC
   - Current: ~250mA peak

5. **OV7670 Camera**
   - Power: 3.3V from ESP32S3
   - Current: ~25mA typical

### Power Distribution Board Layout
```
Battery 11.1V
    ├── Arduino MEGA (via VIN)
    ├── L298N Motor Drivers (direct)
    └── 5V BEC
         ├── Servo Motors
         └── ESP32S3
```

### Safety Notes
1. **Protection**
   - Add inline fuse (10A) on battery main line
   - Use XT60 connector for main battery
   - Add reverse polarity protection
   - Install power switch with capacity >10A

2. **Wiring**
   - Use 14 AWG for main battery lines
   - Use 18 AWG for motor connections
   - Use 22 AWG for signal and low-power lines
   - Add ferrite beads on signal lines

3. **Monitoring**
   - Add battery voltage monitoring
   - Set low voltage cutoff at 3.3V per cell (9.9V total)
   - Monitor temperature of motor drivers

4. **Grounding**
   - Create star ground configuration
   - Separate power and signal grounds
   - Use ground plane for noise reduction
   - Add bypass capacitors near ICs

### Power Budget
1. **Maximum Current Draw**
   - Motors (peak): 4 × 2A = 8A
   - Servos (peak): 2 × 1A = 2A
   - Electronics: ~0.5A
   - Total Peak: ~10.5A
   - Battery Capacity Sufficient: 3.3Ah × 35C = 115.5A (continuous)

2. **Runtime Estimate**
   - Average Current: ~5A
   - Battery Capacity: 3.3Ah
   - Estimated Runtime: ~40 minutes
   - Recommended Recharge: At 20% capacity

## Notes
1. Use capacitors (100μF) across motor power inputs to reduce noise
2. Keep digital and power ground connections separate
3. Use shielded cables for camera connections to reduce interference
4. Add pull-up resistors (4.7kΩ) on I2C lines (SDA/SCL)
5. For better performance, consider using a separate 5V power supply for servos
