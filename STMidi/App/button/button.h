#pragma once
#include "stm32f4xx.h"

#define BUTTON_EVENT_COUNT 4

typedef enum
{
    BUTTON_EVENT_PRESS,
    BUTTON_EVENT_RELEASE,
    BUTTON_EVENT_LONG_PRESS,
    BUTTON_EVENT_DOUBLE_PRESS
} button_event_type_t;

typedef void (*button_callback_t)(void);

typedef struct
{
    button_event_type_t type;
    button_callback_t callback;
} button_event_t;

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
    button_event_t _events[BUTTON_EVENT_COUNT];
    uint8_t _events_registed_count;
} button_handle_t;

void button_init(button_handle_t *btn);
void button_register_event(button_handle_t *btn, button_event_type_t type, button_callback_t cb);
void button_poll(void);
