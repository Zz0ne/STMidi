#include "../platform/platform.h"
#include <printf.h>
#include <stdint.h>

void delay_ms(uint32_t ms)
{
    const uint32_t loop_count =
        21000; // This value can be adjusted based on the actual clock frequency.

    for (uint32_t i = 0; i < ms; i++)
    {
        for (uint32_t j = 0; j < loop_count; j++)
        {
            __asm__("nop");
        }
    }
}

int main(void)
{
    platform_init();

    while (1)
    {
        // Toggle LED on GPIO5
        printf("hello from stm32\n");
        delay_ms(200);
    }

    return 0;
}
