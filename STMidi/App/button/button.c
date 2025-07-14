#include "button.h"

#define MAX_BUTTONS 10

uint8_t _btn_cnt                       = 0;
button_handle_t *_buttons[MAX_BUTTONS] = {0};

void button_init(button_handle_t *btn)
{
    for (button_event_type_t event = 0; event < BUTTON_EVENT_COUNT; event++)
    {
        btn->_events[event].type     = event;
        btn->_events[event].callback = NULL;
    }
}

void button_register_event(button_handle_t *btn, button_event_type_t type, button_callback_t cb)
{
    for (button_event_type_t event = 0; event < BUTTON_EVENT_COUNT; event++)
        if (btn->_events[event].type == type)
            btn->_events[event].callback = cb;
}

void button_poll(void)
{
}
