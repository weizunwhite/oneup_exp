#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include "oneup_config.h"

// ??????
enum ButtonEvent {
    BUTTON_EVENT_NONE = 0,
    BUTTON_EVENT_SINGLE,
    BUTTON_EVENT_DOUBLE,
    BUTTON_EVENT_LONG
};

// ??????????/??/??
class ButtonManager {
public:
    void begin(uint8_t pin, bool activeLow = true);
    void update();
    ButtonEvent getEvent();

private:
    uint8_t _pin = 0;
    bool _activeLow = true;
    bool _pressed = false;
    bool _longTriggered = false;
    unsigned long _pressTime = 0;
    unsigned long _lastReleaseTime = 0;
    unsigned long _lastDebounceTime = 0;
    uint8_t _clickCount = 0;
    ButtonEvent _event = BUTTON_EVENT_NONE;
};

#endif // BUTTON_H
