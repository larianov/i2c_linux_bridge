#include "common/tusb_types.h"
#include "device/usbd.h"
#include "pico/stdlib.h"
#include <hardware/gpio.h>
#include <hardware/i2c.h>
#include <hardware/structs/io_bank0.h>
#include <pico/stdio.h>
#include "tusb.h"
#include <pico/time.h>
#include <stdbool.h>
#include "commands.h"
#include "hardware/sync.h"
#include "task_manager.h"
#include "i2c_handler.h"
#include "tusb_option.h"


int main(void) {
    tusb_rhport_init_t struct_for_initing = {.role = TUSB_ROLE_DEVICE, .speed = TUSB_SPEED_FULL};
    tusb_rhport_init(0, &struct_for_initing);
    sleep_ms(2000);
    i2c_init(i2c_def, I2C_SPEED);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    while (true) {
        tud_task();
        if (task.needs_to_handle) {
            if (!handle_all(task.rhport, &task.request, i2c0)) CMD_STATUS = STATUS_ADDRESS_NAK;
            else CMD_STATUS = STATUS_ADDRESS_ACK;
            task.needs_to_handle = false;
        }
        if (tud_task_event_ready()) continue;
        __wfi();
    }
}