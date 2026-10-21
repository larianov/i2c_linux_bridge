#include "tusb.h"
#include <hardware/i2c.h>
#include <stdbool.h>
#include <stdint.h>
extern uint8_t arr[2048];
bool handle_all(const uint8_t rhport, const tusb_control_request_t *request, i2c_inst_t *i2c);
bool handle_reading(const uint8_t rhport, const tusb_control_request_t *request, i2c_inst_t *i2c);
bool handle_writig(const uint8_t rhport, const tusb_control_request_t *request, i2c_inst_t *i2c);
