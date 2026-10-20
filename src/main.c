#include "common/tusb_types.h"
#include "device/usbd.h"
#include "pico/stdlib.h"
#include <pico/stdio.h>
#include "tusb.h"
#include <pico/time.h>
#include "hardware/sync.h"
#include "task_manager.h"

int main(void){
    tusb_rhport_init_t struct_for_initing = {.role = TUSB_ROLE_DEVICE, .speed = TUSB_SPEED_FULL};
    tusb_rhport_init(0, &struct_for_initing);
    sleep_ms(2000);

    while (true) {
        tud_task();
        if (task.needs_to_handle) {
            //
        }
        __wfi();
    }
    
}