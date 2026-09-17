#pragma once
#include <stdint.h>

enum class CommandLinks{BLE, USB_SERIAL,UART};
constexpr CommandLinks ACTIVE_LINKS[]={CommandLinks::BLE, CommandLinks::USB_SERIAL};
constexpr uint8_t PCA9685_ADDRESS=0x40;
constexpr uint8_t IMU_MPU6050_ADDRESS=0x41;
enum class RegisteredCommand{LED,WALK,TURN,CHECK_STATE, SCAN_DEVICES,SLEEP,SIT, STAND, UNIDENTIFIED,FAILED_PARSING,NA, PAUSE,STOP,RESUME};

enum class DeviceStatus{DEVICE_READY, DEVICE_BUSY, DEVICE_UNAVAILABLE,DEVICE_ERROR};
enum class CommandStatus{READY,INPROGRESS,FAILED,COMPLETED,PAUSED, WAITING};
enum class ReturnCode{ERROR,WARN,SUCCESS};

/*BLE Settings*/

constexpr const char* BLE_SERVER_NAME="FIZMO_FB";
constexpr const char*  BLE_GATT_SERVICE_UUID="8d707e01-adb4-4868-b3c3-f9f65611f14c";
constexpr const char*  BLE_GATT_SERVICE_NAME="Fizmo_Bluetooth_Controller";
/*command is always fizmo slow brain(pi) to fizmo fast brain (s3)*/
constexpr const char*  BLE_GATT_CHARACTERISTIC1_UUID="8d707e02-adb4-4868-b3c3-f9f65611f14c";
constexpr const char*  BLE_GATT_CHARACTERISTIC1_NAME="Fizmo_Commands";
/*status is always fizmo fast brain(s3) to fizmo slowbrain (pi)*/
constexpr const char*  BLE_GATT_CHARACTERISTIC2_UUID="8d707e03-adb4-4868-b3c3-f9f65611f14c";
constexpr const char*  BLE_GATT_CHARACTERISTIC2_NAME="Fizmo_Status";

constexpr char* commandNames []={"LED","WALK","TURN","CHECK_STATE", "SCAN_DEVICES","SLEEP","SIT", "STAND"};
constexpr int numCommands=8;
constexpr int numDelims=5;
constexpr int MAX_I2c_VALUE=127;
constexpr uint8_t NUM_JOINTS = 8;   // channels 0..7: FLH,FLK,FRH,FRK,RLH,RLK,RRH,RRK
constexpr int     FRAME_MS   = 40;  // per-frame delay -> raise to slow the walk

// per-joint calibrated ticks for normalized 0.0 and 1.0. PLACEHOLDERS — calibrate each joint.
constexpr int JOINT_MIN[NUM_JOINTS] = {100,100,100,100,100,100,100,100};
constexpr int JOINT_MAX[NUM_JOINTS] = {500,500,500,500,500,500,500,500};
constexpr uint8_t GAIT_FRAMES = 32;
constexpr float GAIT[GAIT_FRAMES][NUM_JOINTS] = {
  { -0.8,   0.0,   0.4,   0.0,   0.8,  -0.5,  -0.4,  -0.5},
  {-0.74, -0.34,  0.31,   0.0,  0.79,  -0.5, -0.49,  -0.5},
  {-0.57, -0.64,  0.21,   0.0,  0.77,  -0.5, -0.57,  -0.5},
  {-0.31, -0.83,   0.1,   0.0,  0.74,  -0.5, -0.63,  -0.5},
  { -0.0,  -0.9,   0.0,   0.0,  0.69,  -0.5, -0.69,  -0.5},
  { 0.31, -0.83,  -0.1,   0.0,  0.63,  -0.5, -0.74,  -0.5},
  { 0.57, -0.64, -0.21,   0.0,  0.57,  -0.5, -0.77,  -0.5},
  { 0.74, -0.34, -0.31,   0.0,  0.49,  -0.5, -0.79,  -0.5},
  {  0.8,   0.0,  -0.4,   0.0,   0.4,  -0.5,  -0.8,  -0.5},
  { 0.79,   0.0, -0.49,   0.0,  0.31,  -0.5, -0.74, -0.69},
  { 0.77,   0.0, -0.57,   0.0,  0.21,  -0.5, -0.57, -0.85},
  { 0.74,   0.0, -0.63,   0.0,   0.1,  -0.5, -0.31, -0.96},
  { 0.69,   0.0, -0.69,   0.0,   0.0,  -0.5,  -0.0,  -1.0},
  { 0.63,   0.0, -0.74,   0.0,  -0.1,  -0.5,  0.31, -0.96},
  { 0.57,   0.0, -0.77,   0.0, -0.21,  -0.5,  0.57, -0.85},
  { 0.49,   0.0, -0.79,   0.0, -0.31,  -0.5,  0.74, -0.69},
  {  0.4,   0.0,  -0.8,   0.0,  -0.4,  -0.5,   0.8,  -0.5},
  { 0.31,   0.0, -0.74, -0.34, -0.49,  -0.5,  0.79,  -0.5},
  { 0.21,   0.0, -0.57, -0.64, -0.57,  -0.5,  0.77,  -0.5},
  {  0.1,   0.0, -0.31, -0.83, -0.63,  -0.5,  0.74,  -0.5},
  {  0.0,   0.0,  -0.0,  -0.9, -0.69,  -0.5,  0.69,  -0.5},
  { -0.1,   0.0,  0.31, -0.83, -0.74,  -0.5,  0.63,  -0.5},
  {-0.21,   0.0,  0.57, -0.64, -0.77,  -0.5,  0.57,  -0.5},
  {-0.31,   0.0,  0.74, -0.34, -0.79,  -0.5,  0.49,  -0.5},
  { -0.4,   0.0,   0.8,   0.0,  -0.8,  -0.5,   0.4,  -0.5},
  {-0.49,   0.0,  0.79,   0.0, -0.74, -0.69,  0.31,  -0.5},
  {-0.57,   0.0,  0.77,   0.0, -0.57, -0.85,  0.21,  -0.5},
  {-0.63,   0.0,  0.74,   0.0, -0.31, -0.96,   0.1,  -0.5},
  {-0.69,   0.0,  0.69,   0.0,  -0.0,  -1.0,   0.0,  -0.5},
  {-0.74,   0.0,  0.63,   0.0,  0.31, -0.96,  -0.1,  -0.5},
  {-0.77,   0.0,  0.57,   0.0,  0.57, -0.85, -0.21,  -0.5},
  {-0.79,   0.0,  0.49,   0.0,  0.74, -0.69, -0.31,  -0.5},
};