#ifndef BUTTON_H_
#define BUTTON_H_

#include <Arduino.h>
#include <FunctionalInterrupt.h>

#define DEBOUNCE_80MS     80

class Button {
	private:
        int pin;
        uint8_t lastState;
        uint8_t currState;
        unsigned long debounceTimer;
        unsigned long debounceDelay;
        void (*onPressFunc)(void);
        void (*onReleaseFunc)(void);
	public:
		Button(int button_pin, unsigned long debounce);
        ~Button(void);
        void onPress(void (*func)(void));
        void onRelease(void (*func)(void));
		void loop(void);
};

#endif  // BUTTON_H_
