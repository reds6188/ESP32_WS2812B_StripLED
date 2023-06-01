#include "button.h"

void fNULL(void) {}

Button::Button(int button_pin, unsigned long debounce) {
    pin = button_pin;
    debounceDelay = debounce;
    pinMode(pin, INPUT);
    onPressFunc = fNULL;
    onReleaseFunc = fNULL;
}

Button::~Button(void) {
}

void Button::onPress(void (*func)(void)) {
    onPressFunc = func;
}

void Button::onRelease(void (*func)(void)) {
    onReleaseFunc = func;
}

void Button::loop(void) {
    uint8_t reading = digitalRead(pin);

    if(reading != lastState) {
        debounceTimer = millis();
    }

    if((millis() - debounceTimer) > debounceDelay) {
        // if the button state has changed:
        if(reading != currState) {
            currState = reading;

            if(currState == LOW)
                (*onPressFunc)();
            else
                (*onReleaseFunc)();
        }
    }

    lastState = reading;
}