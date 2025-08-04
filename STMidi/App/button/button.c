#include "button.h"

#define MAX_INTERRUPTS              16
#define MAX_BUTTONS_EVENTS          (MAX_INTERRUPTS * BUTTON_EVENT_COUNT)
#define BUTTON_ACTIVE_LOW           0
#define BUTTON_ACTIVE_HIGH          1

#define LONG_PRESS_HOLD_TIME_MS     1500
#define DOUBLE_PRESS_TIME_WINDOW_MS 1000

button_handle_t *_buttons[MAX_INTERRUPTS] = {NULL};

static void _execute_event(button_handle_t *btn, button_event_t type)
{
    if (btn->_events[type].callback != NULL)
        btn->_events[type].callback();
}

void button_init(button_handle_t *btn, GPIO_TypeDef *port, uint16_t pin)
{
    uint32_t index = __builtin_ctz(pin);
    if (index >= MAX_INTERRUPTS)
        return;

    // Check if EXTI interrupt is enabled for this pin
    if ((EXTI->IMR & (1 << index)) == 0)
        return;

    for (button_event_t event = 0; event < BUTTON_EVENT_COUNT; event++)
    {
        btn->_events[event].type     = event;
        btn->_events[event].callback = NULL;
    }

    GPIO_PinState idle_state = HAL_GPIO_ReadPin(port, pin);
    btn->_active_level   = (idle_state == GPIO_PIN_SET) ? BUTTON_ACTIVE_LOW : BUTTON_ACTIVE_HIGH;
    btn->_state          = 0;
    btn->_pin            = pin;
    btn->_port           = port;
    btn->_released       = 0;
    btn->_pressed        = 0;
    btn->_last_irq       = 0;
    btn->_last_press     = 0;
    btn->_first_press    = 0;
    btn->_second_press   = 0;
    btn->_holding_button = 0;

    _buttons[index]      = btn;
}

void button_register_event(button_handle_t *btn, button_event_t type, button_callback_t cb)
{
    for (int event = 0; event < BUTTON_EVENT_COUNT; event++)
        if (btn->_events[event].type == type)
            btn->_events[event].callback = cb;
}

void button_unregister_event(button_handle_t *btn, button_event_t type)
{
    for (int event = 0; event < BUTTON_EVENT_COUNT; event++)
        if (btn->_events[event].type == type)
            btn->_events[event].callback = NULL;
}

void button_unregister_all(button_handle_t *btn)
{
    for (button_event_t event = 0; event < BUTTON_EVENT_COUNT; event++)
        btn->_events[event].callback = NULL;
}

void button_interrupt_handler(uint16_t GPIO_Pin)
{
    uint32_t index       = __builtin_ctz(GPIO_Pin);
    button_handle_t *btn = _buttons[index];

    if (btn)
    {
        // Debounce
        uint32_t now = HAL_GetTick();
        if (now - btn->_last_irq < 50)
            return;
        btn->_last_irq = now;

        // Determine logical button state (pressed = 1, released = 0)
        GPIO_PinState pin_state = HAL_GPIO_ReadPin(btn->_port, btn->_pin);
        btn->_state = (btn->_active_level == BUTTON_ACTIVE_LOW) ? (pin_state == GPIO_PIN_RESET)
                                                                : (pin_state == GPIO_PIN_SET);

        if (btn->_state)
            btn->_pressed = 1;
        else
            btn->_released = 1;
    }
}

void button_poll(void)
{
    for (int pin = 0; pin < 16; pin++)
    {
        button_handle_t *btn = _buttons[pin];
        if (btn == NULL)
            continue;

        if (btn->_pressed)
        {
            btn->_last_press     = HAL_GetTick();
            btn->_holding_button = 1;
            btn->_pressed        = 0;

            _execute_event(btn, BUTTON_EVENT_PRESS);

            // check for double press
            if (btn->_first_press == 0)
            {
                btn->_first_press = btn->_last_press;
            }
            else
            {
                btn->_second_press = btn->_last_press;
                if (btn->_second_press - btn->_first_press < DOUBLE_PRESS_TIME_WINDOW_MS)
                    _execute_event(btn, BUTTON_EVENT_DOUBLE_PRESS);

                btn->_first_press  = 0;
                btn->_second_press = 0;
            }
        }

        if (btn->_released)
        {
            btn->_holding_button = 0;
            btn->_released       = 0;
            _execute_event(btn, BUTTON_EVENT_RELEASE);
        }

        if (btn->_holding_button)
        {
            // check for long press
            if (HAL_GetTick() - btn->_last_press > LONG_PRESS_HOLD_TIME_MS)
            {
                _execute_event(btn, BUTTON_EVENT_LONG_PRESS);
                btn->_last_press     = 0;
                btn->_holding_button = 0;
            }
        }
    }
}
