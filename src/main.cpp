#include "main.h"

AddrLed Strip(STRIP_LED, NUM_LED);

void setup() {
  Strip.setPixel(49, 255, 255, 255);
  Strip.refresh();
}

void loop() {
  // put your main code here, to run repeatedly:
}