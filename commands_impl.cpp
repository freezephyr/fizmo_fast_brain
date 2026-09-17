#pragma once
#include <Wire.h>
#include <config.h>
#include<commands.h>



int scan_i2c_devices(uint8_t deviceAddress){
  Wire.beginTransmission(deviceAddress);
  if (Wire.endTransmission()==0){
    return 0;
  }
  return -1;
}

  

int scan_pca9685(){
  if (scan_i2c_devices(PCA9685_ADDRESS)==0){
    return 0;

  }

  return -1;

}

int scan_imu_mpu6050(){
  if (scan_i2c_devices(IMU_MPU6050_ADDRESS)==0){
    return 0;

  }

  return -1;

}



int scan_devices(ParsedCommand parsedCommand){
  //This function is called on device start by default and can be called as part of debug to check if all devices and motors attached are functioning.
  return 0;
  
}
int invalid_command(ParsedCommand parsedCommand){
  //This function is called whenever either a command is not found or does not meet standard format
  return 0;

}