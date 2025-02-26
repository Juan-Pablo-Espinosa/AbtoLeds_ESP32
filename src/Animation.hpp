#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <Arduino.h>

class Animation {
public:
    static const Animation Fire;
    static const Animation Wave;
    static const Animation Blink;
    static const Animation Score;
    static const Animation Take;
    static const Animation Wait;
    static const Animation Error;
    static const Animation Speed;
    static const Animation Fade;
    static const Animation Rainbow;
    static const Animation Water;
    static const Animation Idle;
    static const Animation Good;



    String name;

    Animation(String animName) : name(animName) {}
};

#endif  // ANIMATION_HPP