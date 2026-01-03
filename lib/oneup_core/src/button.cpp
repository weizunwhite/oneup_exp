#include "button.h"

void ButtonManager::begin(uint8_t pin, bool activeLow) {
    _pin = pin;
    _activeLow = activeLow;
    pinMode(_pin, activeLow ? INPUT_PULLUP : INPUT);
    _pressed = false;
    _longTriggered = false;
    _pressTime = 0;
    _lastReleaseTime = 0;
    _lastDebounceTime = 0;
    _clickCount = 0;
    _event = BUTTON_EVENT_NONE;
}

void ButtonManager::update() {
    unsigned long now = millis();
    int raw = digitalRead(_pin);
    bool isPressed = _activeLow ? (raw == LOW) : (raw == HIGH);

    // 去抖：只在稳定后处理
    if (isPressed != _pressed && (now - _lastDebounceTime) > BUTTON_DEBOUNCE_MS) {
        _lastDebounceTime = now;
        _pressed = isPressed;

        if (_pressed) {
            _pressTime = now;
            _longTriggered = false;
        } else {
            if (!_longTriggered) {
                _clickCount++;
                _lastReleaseTime = now;
            }
        }
    }

    // 长按检测
    if (_pressed && !_longTriggered && (now - _pressTime >= BUTTON_LONG_PRESS_TIME)) {
        _longTriggered = true;
        _clickCount = 0;
        _event = BUTTON_EVENT_LONG;
    }

    // 双击窗口判断
    if (!_pressed && _clickCount > 0 && (now - _lastReleaseTime > BUTTON_DOUBLE_CLICK_MS)) {
        if (_clickCount == 1) {
            _event = BUTTON_EVENT_SINGLE;
        } else {
            _event = BUTTON_EVENT_DOUBLE;
        }
        _clickCount = 0;
    }
}

ButtonEvent ButtonManager::getEvent() {
    ButtonEvent evt = _event;
    _event = BUTTON_EVENT_NONE;
    return evt;
}
