LED library
works for esp32 or some compatible microcontrollers
Pins for leds strips: 2,4,5,12,13,14,18,21
How to reproduce an animation: 
  1) #include<AbtoLeds_ESP32>

  2) Create a strip: AbtoLeds_ESP32 name_of_your_strip(dataPin, numberOfLedsInTheStrip);

  3) Begin the strip: name_of_your_strip.begin();

  4) reproduce the animation you want in the loop: name_of_your_strip.playAnimation(Animation::"Animation_Name");
      a) Names of animations:
     Fire
     Wave
     Blink
     Score
     Take
     Wait
     Error
     Speed
     Fade
     Rainbow
     Water
     Idle
     Good

  5)Update in loop: name_of_your_strip.updateLoop();

Thats it :)
  
