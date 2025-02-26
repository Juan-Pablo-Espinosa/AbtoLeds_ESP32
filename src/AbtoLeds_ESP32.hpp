#ifndef AbtoLeds_ESP32_HPP
#define AbtoLeds_ESP32_HPP

#pragma once

#include "Animation.hpp"
#include <Arduino.h>
#include <FastLED.h>

class AbtoLeds_ESP32 {
public:
    AbtoLeds_ESP32(int _dataPin, int _numLeds);
    ~AbtoLeds_ESP32();

    void begin();
    void updateLoop();
    void setFireDelay(int _fD);
    void setWaterDelay(int _wD);
    void setBrightness(int _brightness);
    void turnLedsOff();
    void setSolid(int R, int G, int B);
    void setSpeed(int _speed);
    
    void playAnimation(const Animation& anim);
    void Fire();
    void Wave();
    void Blink();
    void Score();
    void Take();
    void Wait();
    void Error();
    void Speed();
    void Fade();
    void Rainbow();
    void Water();
    void Idle();
    void Good();

private:
    int waterDelay = 40; 
    int fireDelay = 5;
    bool flag1;
    int speed;
    u_long time; 
    String state;
    int dataPin;
    int numLeds;
    CRGB* leds;
    CLEDController* controller;
};

#endif  // AbtoLeds_ESP32_HP