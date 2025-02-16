#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

void clock_setup(void)
{
    flash_set_ws(FLASH_ACR_LATENCY_2WS);
    flash_prefetch_enable();

    // rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_84MHZ]);
    rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_3V3_84MHZ]);
}

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
    clock_setup();
    // Enable GPIOA clock (for LED)
    rcc_periph_clock_enable(RCC_GPIOA);

    // Set GPIO5 (LED) to 'output push-pull'
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO5);

    while (1)
    {
        // Toggle LED on GPIO5
        gpio_toggle(GPIOA, GPIO5);
        delay_ms(1000);
    }

    return 0;
}
