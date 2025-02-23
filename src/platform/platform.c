#include "platform.h"
#include "clock_setup.h"
#include "uart_serial_comms.h"

void platform_init(void)
{
    clock_setup();
    uart_serial_comms_init();
}
