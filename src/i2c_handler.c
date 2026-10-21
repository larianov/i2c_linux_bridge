#include "i2c_handler.h"
#include "common/tusb_types.h"
#include "device/usbd.h"
#include "hardware/i2c.h"
#include <stdbool.h>
#include <stdint.h>
bool handle_all(const uint8_t rhport, const tusb_control_request_t *request, i2c_inst_t *i2c){
    if (request->bmRequestType_bit.direction == TUSB_DIR_IN) {
        return handle_reading(rhport, request, i2c);
    }
    else {
        return handle_writig(rhport, request, i2c);
    }
}

uint8_t arr[2048];

bool handle_reading(const uint8_t rhport, const tusb_control_request_t *request, i2c_inst_t *i2c){
    memset(arr, 0, request->wLength);
    int res_i2c = i2c_read_timeout_us(i2c, request->wIndex, arr, request->wLength, (request->bRequest == 4 || request->bRequest == 5) ? true : false, 100000);
    tud_control_xfer(rhport, request, arr, request->wLength);
    return res_i2c >= 0 ? true : false;
}

bool handle_writig(const uint8_t rhport, const tusb_control_request_t *request, i2c_inst_t *i2c){
    int res_i2c = i2c_write_timeout_us(i2c, request->wIndex, arr, request->wLength > 0 ? request->wLength : 1, (request->bRequest == 4 || request->bRequest == 5) ? true : false, 100000);
    return res_i2c >= 0 ? true : false;
}