#include "app_run.h"
#include "button/button.h"
#include "stm32f4xx_hal.h"
#include <stdio.h>

void long_press_event(void)
{
    printf("long_press_event\n");
}

void double_press_event(void)
{
    printf("double_press_event\n");
}

void press_event(void)
{
    printf("press_event\n");
}
void release_event(void)
{
    printf("release_event\n");
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    button_interrupt_handler(GPIO_Pin);
}

void app_run(void)
{
    button_handle_t btn1;
    button_init(&btn1, GPIOC, GPIO_PIN_13);

    button_register_event(&btn1, BUTTON_EVENT_PRESS, press_event);
    button_register_event(&btn1, BUTTON_EVENT_RELEASE, release_event);
    button_register_event(&btn1, BUTTON_EVENT_LONG_PRESS, long_press_event);
    button_register_event(&btn1, BUTTON_EVENT_DOUBLE_PRESS, double_press_event);

    printf("Test\n");

    while (1)
    {
        button_poll();
        // HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        // HAL_Delay(1000);
    }
}
