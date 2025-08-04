#pragma once
#include "stm32f4xx.h"

#define BUTTON_EVENT_COUNT 4

typedef enum
{
    BUTTON_EVENT_PRESS,
    BUTTON_EVENT_RELEASE,
    BUTTON_EVENT_LONG_PRESS,
    BUTTON_EVENT_DOUBLE_PRESS
} button_event_t;

typedef void (*button_callback_t)(void);

typedef struct
{
    GPIO_TypeDef *_port;
    uint16_t _pin;
    volatile uint8_t _state;
    volatile uint8_t _released;
    volatile uint8_t _pressed;
    volatile uint32_t _last_irq;
    uint8_t _active_level;

    uint8_t _holding_button;
    uint32_t _last_press;
    uint32_t _first_press;
    uint32_t _second_press;

    struct
    {
        button_event_t type;
        button_callback_t callback;
    } _events[BUTTON_EVENT_COUNT];

} button_handle_t;

void button_init(button_handle_t *btn, GPIO_TypeDef *port, uint16_t pin);
void button_register_event(button_handle_t *btn, button_event_t type, button_callback_t cb);
void button_unregister_event(button_handle_t *btn, button_event_t type);
void button_unregister_all(button_handle_t *btn);
void button_interrupt_handler(uint16_t GPIO_Pin);
void button_poll(void);
