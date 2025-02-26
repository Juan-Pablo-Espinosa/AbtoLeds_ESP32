#include <Arduino.h>
#include <FastLED.h>
#include "AbtoLeds_ESP32.hpp"
#include "Animation.hpp"

AbtoLeds_ESP32 strip(13, 60); // Define an LED strip (Note: the value will be multiply by 3)

void setup() {
    Serial.begin(115200);
    strip.begin();
    delay(1000);
    strip.turnLedsOff();
}

void loop() {
 strip.playAnimation(Animation::Idle);
 strip.updateLoop(); 
}
