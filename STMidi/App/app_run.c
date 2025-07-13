#include "app_run.h"
#include "stm32f4xx.h"

void app_run(void)
{
    while (1)
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        HAL_Delay(1000);
    }
}
