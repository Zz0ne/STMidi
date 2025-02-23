#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/rcc.h>

void clock_setup(void)
{
    flash_set_ws(FLASH_ACR_LATENCY_2WS);
    flash_prefetch_enable();

    // rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_84MHZ]);
    rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_3V3_84MHZ]);
}
