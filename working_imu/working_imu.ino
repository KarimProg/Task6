/*
 * File: working_imu.ino
 * Author: Karim Abogharbia
 * Description: This Arduino code communicates with the MPU-6050 IMU sensor's registries
 *              to directly receive the angle about the Z-axis.
 * Date: [5/9/2023]
 */

/* Header Files */
#include <Wire.h>  // I2C communication

/* Macros */
#define MPU6050_ADDRESS 0x68          // MPU-6050 I2C address
#define MPU6050_REG_GYRO_ZOUT_H 0x47  // Gyro Z-axis high byte register
#define MPU6050_REG_GYRO_ZOUT_L 0x48  // Gyro Z-axis low byte register
#define sample_num 9000               // Number of samples for calibration
#define threshold 0.001               // Threshold to accept change in Z angle

int16_t gyroZ;             // Change in Z angle
int16_t gyro_Zoffset = 0;  // 0 Error of IMU Z-axis gyro sensor

float gyroScale = 16.375;  // Sensitivity scale factor for gyro (from +-32,786 to +-2000deg/s)
float angleZ = 0.0;        // Angle of rotation about Z-axis

unsigned long previousMillis = 0;   // Stores the previous millisecond value
const unsigned long interval = 10;  // Interval in milliseconds

void setup() {
  // Initialize I2C communication as master
  Wire.begin();
  // Initialize serial communication at default baud rate (9600)
  Serial.begin(9600);

  // Initialize MPU-6050
  MPU6050_Init();
  Serial.println("\nCalibrating, don't move IMU!");
  // Calibrate MPU-6050
  MPU6050_calibration();
}

void loop() {
  unsigned long currentMillis = millis();  // Get the current millisecond value

  // Calculate the time elapsed since the previous loop iteration
  unsigned long elapsedTime = currentMillis - previousMillis;

  // Read gyro Z-axis data
  gyroZ = (readRegister(MPU6050_ADDRESS, MPU6050_REG_GYRO_ZOUT_H) / gyroScale) - gyro_Zoffset;

  // Calculate angle change using gyro data and elapsed time
  float gyroAngleChange = gyroZ * (elapsedTime / 1000.0);  // Convert elapsed time to seconds

  // Update the angle if it exceeds threshold
  if (gyroAngleChange > threshold || gyroAngleChange < -threshold)
    angleZ += gyroAngleChange;

  // Store the current time as the previous time for the next iteration
  previousMillis = currentMillis;

  // Print the angle of rotation about Z-axis (in degrees)
  Serial.print("Angle (Z-axis): ");
  Serial.println(angleZ);

  delay(interval);
}

/* Function Definitions */

/**
 * @brief Initializes the IMU sensor and sets its full scale range.
 */
void MPU6050_Init() {
  // Wake up MPU-6050
  writeRegister(MPU6050_ADDRESS, 0x6B, 0x00);  // Power Management 1 register
  // Set full scale range to +-2000deg/s
  writeRegister(MPU6050_ADDRESS, 0x1B, 0x18);
}

/**
 * @brief Calibrates the IMU sensor's readings.
 */
void MPU6050_calibration() {
  for (int i = 0; i < sample_num; i++) {
    // Reading requested from Z-axis gyro register
    gyroZ = readRegister(MPU6050_ADDRESS, MPU6050_REG_GYRO_ZOUT_H) / gyroScale;
    // Offset is added to Offset sum
    gyro_Zoffset += gyroZ;
  }
  // Calculate mean offset
  gyro_Zoffset /= sample_num;
}


/**
 * @brief Writes data to the IMU over the I2C bus.
 *
 * @param deviceAddress Address of the IMU sensor.
 * @param registerAddress Address of the register on the IMU sensor.
 */
void writeRegister(uint8_t deviceAddress, uint8_t registerAddress, uint8_t data) {
  Wire.beginTransmission(deviceAddress);
  Wire.write(registerAddress);
  Wire.write(data);
  Wire.endTransmission();
}

/**
 * @brief Reads data from IMU over the I2C bus.
 *
 * @param deviceAddress Address of the IMU sensor.
 * @param registerAddress Address of the register on the IMU sensor.
 * @return Value of reading from the IMU sensor.
 */
int16_t readRegister(uint8_t deviceAddress, uint8_t registerAddress) {
  Wire.beginTransmission(deviceAddress);
  Wire.write(registerAddress);
  Wire.endTransmission();
  // 2 bytes are requested from device
  Wire.requestFrom(deviceAddress, 2);
  // Reading is taken in as 8 bits which are then shifted to read the remaining 8 bits
  int16_t value = Wire.read() << 8 | Wire.read();
  return value;
}