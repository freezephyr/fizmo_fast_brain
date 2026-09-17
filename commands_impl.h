/*void control_led(ParsedCommand parsedCommand);
void walk(ParsedCommand parsedCommand);
void turn(ParsedCommand parsedCommand);
void check_state(ParsedCommand parsedCommand);
void scan_devices(ParsedCommand parsedCommand);

/**
 * @brief Scans the I2C bus for expected devices and reports which responded.
 *
 * Polls each known device address defined in config.h (PCA9685, IMU, etc.)
 * and records whether each one acknowledged. Assumes Wire.begin() has
 * already been called in setup().
 *
 * @return Bitmask of responding devices; bit set = device present.
 */
/*int scan_i2c_devices();
/**
 * @brief scans given response for .
 *
 * Polls each known device address defined in config.h (PCA9685, IMU, etc.)
 * and records whether each one acknowledged. Assumes Wire.begin() has
 * already been called in setup().
 *
 * @return Bitmask of responding devices; bit set = device present.
 */
 /*
int scan_imu();
int scan_pca9685();
int scan_motors();
int scan_camera();
void invalid_command(ParsedCommand parsedCommand);
*/