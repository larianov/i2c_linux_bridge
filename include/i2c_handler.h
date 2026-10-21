#include "tusb.h"
#include <hardware/i2c.h>
#include <stdbool.h>
#include <stdint.h>
extern uint8_t arr[2048];
#define I2C_SPEED 100000
extern i2c_inst_t *i2c_def;
#define I2C_SDA 16
#define I2C_SCL 17
bool handle_all(const uint8_t rhport, const tusb_control_request_t *request, i2c_inst_t *i2c);
bool handle_reading(const uint8_t rhport, const tusb_control_request_t *request, i2c_inst_t *i2c);
bool handle_writig(const uint8_t rhport, const tusb_control_request_t *request, i2c_inst_t *i2c);
