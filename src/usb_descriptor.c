#include "tusb_config.h"
#include "common/tusb_types.h"
#include "device/usbd.h"
#include "commands.h"
#include "tusb.h"
#include "task_manager.h"
#include "stdint.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "i2c_handler.h"

static uint32_t I2C_FUNC_I2C = 0x0EFF0009;

uint8_t CMD_STATUS = STATUS_IDLE;
task_manager task = {.needs_to_handle = false};

tusb_desc_device_t const desc_device = {
    .bLength = 18,
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0200,

    .bDeviceClass = 0x00,
    .bDeviceSubClass = 0x00,
    .bDeviceProtocol = 0x00,
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,

    .idVendor = 0x0403,
    .idProduct = 0xc631,
    .bcdDevice = 0x0100,

    .iManufacturer = 0x01,
    .iProduct = 0x02,
    .iSerialNumber = 0x03,

    .bNumConfigurations = 0x01,
};

uint8_t const *tud_descriptor_device_cb() {
    return (const uint8_t *)(&desc_device);
}

uint8_t const desc_configuration[] = {
    TUD_CONFIG_DESCRIPTOR(1, 1, 0, CONFIG_TOTAL_LEN, 0x00, 100),
    TUD_VENDOR_DESCRIPTOR(0, 0, 0x01, 0x81, 64),
};

uint8_t const *tud_descriptor_configuration_cb(uint8_t index) {
    (void)index;
    return desc_configuration;
}

char const *string_desc_arr[] = {
    (const char[]){0x09, 0x04}, // 0: is supported language is English (0x0409)
    "David_Larianovskii",       // 1: Manufacturer
    "USB-I2C Bridge",           // 2: Product
    "24ABC42ABC2442",           // 3: Serials will use unique ID if possible
};

static uint16_t buff[128];

uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
    (void)langid;
    uint32_t count = 1;
    if (index > 3)
        return NULL;
    if (index == 0) {
        buff[count] = (uint16_t)(string_desc_arr[0][1] << 8 | string_desc_arr[0][0]);
        count++;
    } else {
        for (uint32_t i = 0; string_desc_arr[index][i] != '\0'; i++) {
            buff[count++] = string_desc_arr[index][i];
        }
    }
    buff[0] = (uint16_t)((2 + (2 * (count - 1))) | 0x03 << 8);
    return buff;
}

bool tud_vendor_control_xfer_cb(uint8_t rhport, uint8_t stage, tusb_control_request_t const *request) {
    if (stage == CONTROL_STAGE_ACK)
        return true;
    if (stage == CONTROL_STAGE_SETUP) {
        if (request->bRequest == CMD_SET_DELAY) {
            return tud_control_status(rhport, request);
        } else if (request->bRequest == CMD_GET_FUNC) {
            return tud_control_xfer(rhport, request, &I2C_FUNC_I2C, sizeof(uint32_t));
        } else if (request->bRequest == CMD_GET_STATUS) {
            if (task.needs_to_handle == true) return true;
            return tud_control_xfer(rhport, request, &CMD_STATUS, sizeof(uint8_t));
        } else if (request->bmRequestType_bit.direction == TUSB_DIR_IN) {
            if (request->wLength > 2048) return false;
            memcpy(&task.request, request, sizeof(tusb_control_request_t));
            task.needs_to_handle = true;
            task.rhport = rhport;
            return true;
        }else if (request->bmRequestType_bit.direction == TUSB_DIR_OUT) {
            if (request->wLength > 2048) return false;
            return tud_control_xfer(rhport, request, arr, request->wLength);
        } else
            return false;
    } else if (stage == CONTROL_STAGE_DATA) {
        if (request->bmRequestType_bit.direction != TUSB_DIR_OUT)
            return true;
        task.needs_to_handle = true;
        memcpy(&task.request, request, sizeof(tusb_control_request_t));
        task.rhport = rhport;
        return true;
    }
    return false;
}