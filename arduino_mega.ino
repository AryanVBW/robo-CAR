#include <Servo.h>

// Motor control pins
const int LEFT_MOTOR_A_EN = 2;
const int LEFT_MOTOR_A_IN1 = 3;
const int LEFT_MOTOR_A_IN2 = 4;
const int LEFT_MOTOR_B_IN1 = 5;
const int LEFT_MOTOR_B_IN2 = 6;
const int LEFT_MOTOR_B_EN = 7;

const int RIGHT_MOTOR_A_EN = 8;
const int RIGHT_MOTOR_A_IN1 = 9;
const int RIGHT_MOTOR_A_IN2 = 10;
const int RIGHT_MOTOR_B_IN1 = 13;
const int RIGHT_MOTOR_B_IN2 = 14;
const int RIGHT_MOTOR_B_EN = 15;

// Servo pins
const int GRIPPER_BASE_PIN = 11;
const int GRIPPER_CLAW_PIN = 12;

// Battery monitoring
const int BATTERY_PIN = A0;
const float BATTERY_DIVIDER_RATIO = 0.2; // Voltage divider ratio (20k/100k)
const float BATTERY_CELL_MIN = 3.3;      // Minimum voltage per cell
const float BATTERY_CELL_COUNT = 3;      // 3S LiPo
const float BATTERY_MIN_VOLTAGE = BATTERY_CELL_MIN * BATTERY_CELL_COUNT;
const unsigned long BATTERY_CHECK_INTERVAL = 10000; // Check every 10 seconds
unsigned long lastBatteryCheck = 0;

// Create servo objects
Servo gripperBase;
Servo gripperClaw;

// Motor speed control (0-255)
const int MOTOR_SPEED = 200;
bool lowBatteryMode = false;

void setup() {
  // Initialize serial communication with ESP32S3
  Serial1.begin(115200);
  
  // Initialize motor pins
  pinMode(LEFT_MOTOR_A_EN, OUTPUT);
  pinMode(LEFT_MOTOR_A_IN1, OUTPUT);
  pinMode(LEFT_MOTOR_A_IN2, OUTPUT);
  pinMode(LEFT_MOTOR_B_IN1, OUTPUT);
  pinMode(LEFT_MOTOR_B_IN2, OUTPUT);
  pinMode(LEFT_MOTOR_B_EN, OUTPUT);
  
  pinMode(RIGHT_MOTOR_A_EN, OUTPUT);
  pinMode(RIGHT_MOTOR_A_IN1, OUTPUT);
  pinMode(RIGHT_MOTOR_A_IN2, OUTPUT);
  pinMode(RIGHT_MOTOR_B_IN1, OUTPUT);
  pinMode(RIGHT_MOTOR_B_IN2, OUTPUT);
  pinMode(RIGHT_MOTOR_B_EN, OUTPUT);
  
  // Attach servos
  gripperBase.attach(GRIPPER_BASE_PIN);
  gripperClaw.attach(GRIPPER_CLAW_PIN);
  
  // Initialize servos to neutral position
  gripperBase.write(90);
  gripperClaw.write(90);
  
  // Initially stop all motors
  stopMotors();
}

void loop() {
  // Check battery voltage periodically
  if (millis() - lastBatteryCheck >= BATTERY_CHECK_INTERVAL) {
    checkBattery();
    lastBatteryCheck = millis();
  }
  
  if (Serial1.available() > 0) {
    char command = Serial1.read();
    executeCommand(command);
  }
}

float getBatteryVoltage() {
  int rawValue = analogRead(BATTERY_PIN);
  float voltage = (rawValue * 5.0 / 1023.0) / BATTERY_DIVIDER_RATIO;
  return voltage;
}

void checkBattery() {
  float voltage = getBatteryVoltage();
  
  // Send battery voltage to ESP32S3
  Serial1.print("B:");
  Serial1.println(voltage);
  
  if (voltage < BATTERY_MIN_VOLTAGE) {
    if (!lowBatteryMode) {
      lowBatteryMode = true;
      // Notify ESP32S3 of low battery
      Serial1.println("L:1");
    }
  } else if (lowBatteryMode && voltage > (BATTERY_MIN_VOLTAGE + 0.3)) {
    // Add hysteresis to prevent oscillation
    lowBatteryMode = false;
    Serial1.println("L:0");
  }
}

void executeCommand(char command) {
  // If in low battery mode, only allow certain commands
  if (lowBatteryMode && (command != 'S' && command != 'B' && command != 'P')) {
    Serial1.println("E:LOW_BATTERY");
    return;
  }
  
  switch (command) {
    case 'F': // Forward
      moveForward();
      break;
    case 'B': // Backward
      moveBackward();
      break;
    case 'L': // Left
      turnLeft();
      break;
    case 'R': // Right
      turnRight();
      break;
    case 'S': // Stop
      stopMotors();
      break;
    case 'G': // Grip (close)
      gripperClaw.write(180);
      break;
    case 'O': // Open grip
      gripperClaw.write(0);
      break;
    case 'U': // Base Up
      gripperBase.write(180);
      break;
    case 'D': // Base Down
      gripperBase.write(0);
      break;
    case 'N': // Neutral position
      gripperBase.write(90);
      gripperClaw.write(90);
      break;
    case 'P': // Ping (connection check)
      // Send battery voltage along with alive signal
      Serial1.print("A:");
      Serial1.println(getBatteryVoltage());
      break;
    case 'V': // Request voltage
      Serial1.print("B:");
      Serial1.println(getBatteryVoltage());
      break;
  }
}

void moveForward() {
  int speed = lowBatteryMode ? MOTOR_SPEED / 2 : MOTOR_SPEED;
  
  // Left motors forward
  analogWrite(LEFT_MOTOR_A_EN, speed);
  analogWrite(LEFT_MOTOR_B_EN, speed);
  digitalWrite(LEFT_MOTOR_A_IN1, HIGH);
  digitalWrite(LEFT_MOTOR_A_IN2, LOW);
  digitalWrite(LEFT_MOTOR_B_IN1, HIGH);
  digitalWrite(LEFT_MOTOR_B_IN2, LOW);
  
  // Right motors forward
  analogWrite(RIGHT_MOTOR_A_EN, speed);
  analogWrite(RIGHT_MOTOR_B_EN, speed);
  digitalWrite(RIGHT_MOTOR_A_IN1, HIGH);
  digitalWrite(RIGHT_MOTOR_A_IN2, LOW);
  digitalWrite(RIGHT_MOTOR_B_IN1, HIGH);
  digitalWrite(RIGHT_MOTOR_B_IN2, LOW);
}

void moveBackward() {
  int speed = lowBatteryMode ? MOTOR_SPEED / 2 : MOTOR_SPEED;
  
  // Left motors backward
  analogWrite(LEFT_MOTOR_A_EN, speed);
  analogWrite(LEFT_MOTOR_B_EN, speed);
  digitalWrite(LEFT_MOTOR_A_IN1, LOW);
  digitalWrite(LEFT_MOTOR_A_IN2, HIGH);
  digitalWrite(LEFT_MOTOR_B_IN1, LOW);
  digitalWrite(LEFT_MOTOR_B_IN2, HIGH);
  
  // Right motors backward
  analogWrite(RIGHT_MOTOR_A_EN, speed);
  analogWrite(RIGHT_MOTOR_B_EN, speed);
  digitalWrite(RIGHT_MOTOR_A_IN1, LOW);
  digitalWrite(RIGHT_MOTOR_A_IN2, HIGH);
  digitalWrite(RIGHT_MOTOR_B_IN1, LOW);
  digitalWrite(RIGHT_MOTOR_B_IN2, HIGH);
}

void turnLeft() {
  int speed = lowBatteryMode ? MOTOR_SPEED / 2 : MOTOR_SPEED;
  
  // Left motors backward
  analogWrite(LEFT_MOTOR_A_EN, speed);
  analogWrite(LEFT_MOTOR_B_EN, speed);
  digitalWrite(LEFT_MOTOR_A_IN1, LOW);
  digitalWrite(LEFT_MOTOR_A_IN2, HIGH);
  digitalWrite(LEFT_MOTOR_B_IN1, LOW);
  digitalWrite(LEFT_MOTOR_B_IN2, HIGH);
  
  // Right motors forward
  analogWrite(RIGHT_MOTOR_A_EN, speed);
  analogWrite(RIGHT_MOTOR_B_EN, speed);
  digitalWrite(RIGHT_MOTOR_A_IN1, HIGH);
  digitalWrite(RIGHT_MOTOR_A_IN2, LOW);
  digitalWrite(RIGHT_MOTOR_B_IN1, HIGH);
  digitalWrite(RIGHT_MOTOR_B_IN2, LOW);
}

void turnRight() {
  int speed = lowBatteryMode ? MOTOR_SPEED / 2 : MOTOR_SPEED;
  
  // Left motors forward
  analogWrite(LEFT_MOTOR_A_EN, speed);
  analogWrite(LEFT_MOTOR_B_EN, speed);
  digitalWrite(LEFT_MOTOR_A_IN1, HIGH);
  digitalWrite(LEFT_MOTOR_A_IN2, LOW);
  digitalWrite(LEFT_MOTOR_B_IN1, HIGH);
  digitalWrite(LEFT_MOTOR_B_IN2, LOW);
  
  // Right motors backward
  analogWrite(RIGHT_MOTOR_A_EN, speed);
  analogWrite(RIGHT_MOTOR_B_EN, speed);
  digitalWrite(RIGHT_MOTOR_A_IN1, LOW);
  digitalWrite(RIGHT_MOTOR_A_IN2, HIGH);
  digitalWrite(RIGHT_MOTOR_B_IN1, LOW);
  digitalWrite(RIGHT_MOTOR_B_IN2, HIGH);
}

void stopMotors() {
  // Disable all motor enables
  analogWrite(LEFT_MOTOR_A_EN, 0);
  analogWrite(LEFT_MOTOR_B_EN, 0);
  analogWrite(RIGHT_MOTOR_A_EN, 0);
  analogWrite(RIGHT_MOTOR_B_EN, 0);
  
  // Set all direction pins LOW
  digitalWrite(LEFT_MOTOR_A_IN1, LOW);
  digitalWrite(LEFT_MOTOR_A_IN2, LOW);
  digitalWrite(LEFT_MOTOR_B_IN1, LOW);
  digitalWrite(LEFT_MOTOR_B_IN2, LOW);
  digitalWrite(RIGHT_MOTOR_A_IN1, LOW);
  digitalWrite(RIGHT_MOTOR_A_IN2, LOW);
  digitalWrite(RIGHT_MOTOR_B_IN1, LOW);
  digitalWrite(RIGHT_MOTOR_B_IN2, LOW);
}
