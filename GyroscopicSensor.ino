#include <Wire.h>

const int MPU_ADDR = 0x68; 
float accelX, accelY, accelZ;
float gyroX, gyroY, gyroZ;
float accelAngleX, accelAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
float rollAngle, pitchAngle, yawAngle;
float accelBiasX, accelBiasY, gyroBiasX, gyroBiasY, gyroBiasZ;
float deltaTime, currTime, prevTime;
int sampleCount = 0;

void setup() {
  Serial.begin(19200);
  Wire.begin();                      
  Wire.beginTransmission(MPU_ADDR);       
  Wire.write(0x6B);                  
  Wire.write(0x00);                  
  Wire.endTransmission(true);        
  delay(20);
}

void loop() {
  // Reading data from accelerometer
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); 
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6, true); 
  
  // Dividing by 16384 according to datasheet
  accelX = (Wire.read() << 8 | Wire.read()) / 16384.0; 
  accelY = (Wire.read() << 8 | Wire.read()) / 16384.0; 
  accelZ = (Wire.read() << 8 | Wire.read()) / 16384.0; 

  // Calculating Roll and Pitch from the accelerometer data
  accelAngleX = (atan(accelY / sqrt(pow(accelX, 2) + pow(accelZ, 2))) * 180 / PI) - 0.58; 
  accelAngleY = (atan(-1 * accelX / sqrt(pow(accelY, 2) + pow(accelZ, 2))) * 180 / PI) + 1.58; 
  
  // Reading gyroscope data
  prevTime = currTime;        
  currTime = millis();            
  deltaTime = (currTime - prevTime) / 1000.0; 
  
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x43); 
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6, true); 

  gyroX = (Wire.read() << 8 | Wire.read()) / 131.0; // Dividing by 131.0 according to datasheet
  gyroY = (Wire.read() << 8 | Wire.read()) / 131.0;
  gyroZ = (Wire.read() << 8 | Wire.read()) / 131.0;

  // Calibration
  gyroX = gyroX + 0.56; // Gyro Bias X ~(-0.56)
  gyroY = gyroY - 2.0;  // Gyro Bias Y ~(2)
  gyroZ = gyroZ + 0.79; // Gyro Bias Z ~(-0.8)

  // Getting the angles in degrees
  gyroAngleX += gyroX * deltaTime; 
  gyroAngleY += gyroY * deltaTime;
  yawAngle   += gyroZ * deltaTime;

  // Combining accelerometer and gyro angle values
  rollAngle  = 0.96 * gyroAngleX + 0.04 * accelAngleX;
  pitchAngle = 0.96 * gyroAngleY + 0.04 * accelAngleY;
  
  // Print the values on the serial monitor
  Serial.print(rollAngle);
  Serial.print("/");
  Serial.print(pitchAngle);
  Serial.print("/");
  Serial.println(yawAngle);
}
