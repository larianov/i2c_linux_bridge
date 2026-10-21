#ifndef TASKMANAGER
#include "tusb.h"
#include <stdbool.h>
typedef struct {
    bool needs_to_handle;
    uint8_t rhport;
    tusb_control_request_t request;
} task_manager;

extern task_manager task;
#endif