#include "AbtoLeds_ESP32.hpp"


AbtoLeds_ESP32::AbtoLeds_ESP32(int _dataPin, int _numLeds) : dataPin(_dataPin), numLeds(_numLeds) {
    leds = new CRGB[numLeds];
}

AbtoLeds_ESP32::~AbtoLeds_ESP32() {
    delete[] leds;
}

void AbtoLeds_ESP32::begin() {
    bool validPin = true;

    switch (dataPin) {
        case 2:  FastLED.addLeds<NEOPIXEL, 2>(leds, numLeds); break;
        case 4:  FastLED.addLeds<NEOPIXEL, 4>(leds, numLeds); break;
        case 5:  FastLED.addLeds<NEOPIXEL, 5>(leds, numLeds); break;
        case 12: FastLED.addLeds<NEOPIXEL, 12>(leds, numLeds); break;
        case 13: FastLED.addLeds<NEOPIXEL, 13>(leds, numLeds); break;
        case 14: FastLED.addLeds<NEOPIXEL, 14>(leds, numLeds); break;
        case 18: FastLED.addLeds<NEOPIXEL, 18>(leds, numLeds); break;
        case 21: FastLED.addLeds<NEOPIXEL, 21>(leds, numLeds); break;
        default:
            Serial.print("Unsupported pin: ");
            Serial.println(dataPin);
            validPin = false;
            break;
    }

    if (validPin) {
        setSolid(0, 255, 0);  // Set to green to confirm it works
        FastLED.show(); // Make sure LEDs update immediately
    }

    time = millis();  // Initialize time to current time
    flag1 = true;     // Initialize flag1 as true (LEDs should be on initially)
    setSpeed(1000);   // Set the blink speed (1000ms or 1 second)
}
void AbtoLeds_ESP32::setSpeed(int _speed){speed = _speed;}

void AbtoLeds_ESP32::updateLoop() {
    
    if (state == "Fire") {
        Fire();
    } else if (state == "Wave") {
        Wave();
    } else if (state == "Blink") {
        Blink();
    } else if (state == "Score") {
        Score();
    } else if (state == "Take") {
        Take();
    } else if (state == "Wait") {
        Wait();
    } else if (state == "Error") {
        Error();
    } else if (state == "Speed") {
        Speed();
    } else if (state == "Fade") {
        Fade();
    } else if (state == "Rainbow") {
        Rainbow();
    } else if (state == "Water") {
        Water();
    } else if (state == "Idle") {
        Idle();
    } else if (state == "Good") {
        Good();
    } else {
        Serial.println("Unknown animation: " + state);
    }
    FastLED.show();
}

void AbtoLeds_ESP32::setFireDelay(int _fD){
    fireDelay = _fD;
}
void AbtoLeds_ESP32::setWaterDelay(int _wD){
    waterDelay = _wD;
}

void AbtoLeds_ESP32::setBrightness(int _brightness) {
    FastLED.setBrightness(_brightness);
}

void AbtoLeds_ESP32::turnLedsOff() {
    fill_solid(leds, numLeds, CRGB::Black);
    FastLED.show();
}

void AbtoLeds_ESP32::setSolid(int R, int G, int B) {
    fill_solid(leds, numLeds, CRGB(R, G, B));
    FastLED.show();
}

void AbtoLeds_ESP32::playAnimation(const Animation& anim) {
    state = anim.name;
    // Serial.print("Playing animation: ");
    // Serial.println(anim.name);
    // Animation Fire;
    // Animation Wave;
    // Animation Blink;
    // Animation Score;
    // Animation Take;
    // Animation Wait;
    // Animation Error;
    // Animation Speed;
    // Animation Fade;
    // Animation Rainbow;
    // Animation Water;
    // Animation Idle;
    // Animation Good;
}

void AbtoLeds_ESP32::Fire(){
    static bool initialized = false;
    static int phase = 0;         // 0 = ascending, 1 = descending
    static int j = 0;             // Outer cycle counter (runs 5 cycles)
    static int i = 0;             // Current index in the inner loop
    static int randomNum = 0;     // Random limit for the current cycle
    static unsigned long lastUpdate = 0;
    const int fireDelay = 5;      // Delay between steps in milliseconds

    // One-time initialization
    if (!initialized) {
        j = 0;
        phase = 0;      // Start with ascending phase
        i = 0;
        randomNum = random(int(numLeds * 0.7), numLeds);
        lastUpdate = millis();
        fill_solid(leds, numLeds, CRGB::Black);
        FastLED.setBrightness(random(30, 100));
        initialized = true;
    }

    // Only update if enough time has passed
    if (millis() - lastUpdate < fireDelay) {
        return;
    }
    lastUpdate = millis();

    // --- ASCENDING PHASE ---
    if (phase == 0) {
        // Occasionally, fill a larger portion with the fire color (green in this example)
        if (random(0, 10) > 7) {
            int limit = int(numLeds * 0.40);
            for (int k = 0; k < limit && k < numLeds; k++) {
                leds[k] = CRGB(0, 204, 0);
            }
        } else {
            int limit = int(numLeds * 0.25);
            for (int k = 0; k < limit && k < numLeds; k++) {
                leds[k] = CRGB(0, 204, 0);
            }
        }

        // Light up the current index
        if (i < numLeds) {
            leds[i] = CRGB(0, 204, 0);
        }

        // Additional effect: for a fraction of LEDs, modify the color based on i
        int innerLimit = int(numLeds * 0.25 * (i / 15.0));
        for (int k = 0; k < innerLimit && k < numLeds; k++){
            leds[k] = CRGB(i, 204, i);
        }

        i++;  // Move to the next index
        if (i >= randomNum) {
            phase = 1;  // Switch to descending phase
        }
    }
    // --- DESCENDING PHASE ---
    else {
        // Occasionally, fill a larger portion with fire color during descent
        if (random(0, 10) > 7) {
            int limit = int(numLeds * 0.40);
            for (int k = 0; k < limit && k < numLeds; k++){
                leds[k] = CRGB(0, 204, 0);
            }
        } else {
            int limit = int(numLeds * 0.25);
            for (int k = 0; k < limit && k < numLeds; k++){
                leds[k] = CRGB(0, 204, 0);
            }
        }
        // Turn off the LED at index i
        if (i < numLeds) {
            leds[i] = CRGB::Black;
        }
        i--;  // Step backward
        if (i <= 0) {
            j++;  // Completed one ascending/descending cycle
            if (j < 5) {
                // Prepare for the next cycle
                randomNum = random(int(numLeds * 0.7), numLeds);
                phase = 0;
                i = 0;
            } else {
                // Restart the entire fire cycle continuously
                j = 0;
                randomNum = random(int(numLeds * 0.7), numLeds);
                phase = 0;
                i = 0;
            }
        }
    }

    // Finally, update the LED strip
    FastLED.show();
}
void AbtoLeds_ESP32::Wave(){
    static unsigned long lastUpdate = 0;
    const unsigned long updateDelay = 30; // update every 30ms
    if (millis() - lastUpdate < updateDelay) {
        return;
    }
    lastUpdate = millis();

    // 'pos' is the index of the brightest LED (the wave crest).
    // 'direction' determines whether the wave is moving upward (1) or downward (-1)
    static int pos = 0;
    static int direction = 1;

    // Update the position of the wave crest.
    pos += direction;
    if (pos >= numLeds - 1) {
        pos = numLeds - 1;
        direction = -1;
    } else if (pos <= 0) {
        pos = 0;
        direction = 1;
    }

    // For each LED, calculate its brightness based on its distance from the crest.
    for (int i = 0; i < numLeds; i++) {
        int distance = abs(i - pos);
        // The farther an LED is from the crest, the dimmer it gets.
        int brightness = 255 - (distance * 50); // Adjust the multiplier for trail width.
        if (brightness < 0) {
            brightness = 0;
        }
        // Set the LED to a blue color with the calculated brightness.
        leds[i] = CRGB(0, 0, brightness);
    }

    FastLED.show();
}
void AbtoLeds_ESP32::Blink() {
    
    if (millis() - time >= speed) {
        time = millis();  

        flag1 = !flag1;    

        if (flag1) {
            Serial.println("Blink ON");
            setSolid(204, 255, 255);  
        } else {
            Serial.println("Blink OFF");
            turnLedsOff(); 
        }

        FastLED.show();  
    }
}
void AbtoLeds_ESP32::Score(){
    for (int i = 0; i < numLeds; i++) {
        // With about a 12% chance, light this LED with a random vibrant color.
        if (random(0, 100) < 12) {
            leds[i] = CHSV(random(0, 255), 255, 255);
        } else {
            // Otherwise, gently fade the current color.
            leds[i].nscale8(230);  
        }
    }

    // Every 'speed' milliseconds, alternate with a full-strip flash.
    if (millis() - time >= speed) {
        time = millis();  // Reset the timer

        // Use a static flag to alternate the full flash on/off.
        static bool flashMode = false;
        flashMode = !flashMode;
        if (flashMode) {
            // Create a full flash with a random color.
            CRGB flashColor = CHSV(random(0, 255), 255, 255);
            fill_solid(leds, numLeds, flashColor);
        }
    }
    
    FastLED.show();
}
void AbtoLeds_ESP32::Take(){
        // States:
        // 0 = Beam ascending
        // 1 = Flash abduction
        // 2 = Beam descending (coming back)
        // 3 = Pause and reset
        static int state = 0;
        static int beamPos = 0;               // Current starting position of the beam
        static unsigned long lastUpdate = 0;  // For timing updates
        static unsigned long stateStartTime = 0; // To track state durations
    
        const int updateDelay = 25;           // Faster update (30 ms between steps)
        const int beamHeight = 5;             // Height of the abduction beam in LEDs
        const unsigned long flashDuration = 100; // Flash duration in state 1 (100 ms)
        const unsigned long pauseDuration = 100; // Pause after descending (100 ms)
    
        // Wait until enough time has passed before updating the state.
        if (millis() - lastUpdate < updateDelay) {
            return;
        }
        lastUpdate = millis();
    
        // --- State Machine ---
        if (state == 0) {
            // STATE 0: Beam Ascending
            // Clear the strip
            fill_solid(leds, numLeds, CRGB::Black);
            // Draw the beam as a purple gradient starting at beamPos
            for (int i = beamPos; i < beamPos + beamHeight && i < numLeds; i++) {
                int relative = i - beamPos;
                // Compute brightness so the beam has a gradient effect
                uint8_t brightness = 80 + (relative * (175 / beamHeight));
                leds[i] = CHSV(160, 255, brightness); // Hue 160 gives a purple tone
            }
            FastLED.show();
            beamPos++;
            // When the beam reaches near the top, switch to flash state.
            if (beamPos >= numLeds - beamHeight) {
                state = 1;
                stateStartTime = millis();
            }
        }
        else if (state == 1) {
            // STATE 1: Flash Abduction
            // Flash the entire strip white to simulate the abduction burst.
            fill_solid(leds, numLeds, CRGB::White);
            FastLED.show();
            // Hold this flash for flashDuration milliseconds.
            if (millis() - stateStartTime >= flashDuration) {
                state = 2;
            }
        }
        else if (state == 2) {
            // STATE 2: Beam Descending (Coming Back)
            // Clear the strip
            fill_solid(leds, numLeds, CRGB::Black);
            // Draw the descending beam starting at beamPos
            for (int i = beamPos; i < beamPos + beamHeight && i < numLeds; i++) {
                int relative = i - beamPos;
                uint8_t brightness = 80 + (relative * (175 / beamHeight));
                leds[i] = CHSV(160, 255, brightness);
            }
            FastLED.show();
            beamPos--;
            // When the beam has retracted fully, move to the reset pause.
            if (beamPos <= 0) {
                state = 3;
                stateStartTime = millis();
            }
        }
        else if (state == 3) {
            // STATE 3: Pause and Reset
            fill_solid(leds, numLeds, CRGB::Black);
            FastLED.show();
            if (millis() - stateStartTime >= pauseDuration) {
                // Reset all variables for the next cycle.
                state = 0;
                beamPos = 0;
            }
        }
}
void AbtoLeds_ESP32::Wait(){
    // Use a dynamic update rate based on the "speed" variable.
    // Lower speed value = faster updates.
    static unsigned long lastUpdate = 0;
    int updateDelay = 30;  // 'speed' is assumed to be set externally via setSpeed()

    // Static state variables for the moving comet.
    static int pos = 0;       // Current position of the moving dot
    static int direction = 1; // 1 = moving right, -1 = moving left

    // Only update if enough time has passed
    if (millis() - lastUpdate < updateDelay) {
        return;
    }
    lastUpdate = millis();

    // Create a trailing effect by fading all LEDs a bit.
    // Adjust the fade amount (here, 64 out of 256) for a longer or shorter trail.
    fadeToBlackBy(leds, numLeds, 64);

    // Set the current position to a bright color to simulate a "speed" dot.
    // You can change CRGB::White to any color you like.
    leds[pos] = CRGB::White;

    // Move the dot along the strip.
    pos += direction;
    // Reverse direction if we hit either end of the strip.
    if (pos >= numLeds) {
        pos = numLeds - 1;
        direction = -1;
    } else if (pos < 0) {
        pos = 0;
        direction = 1;
    }

    // Update the LED strip with the new state.
    FastLED.show();
}
void AbtoLeds_ESP32::Error(){
    // Static variables to keep track of the error animation state
    static int phase = 0;            // 0 = flashing, 1 = pause
    static int flashCount = 3;       // Number of red flashes per cycle
    static bool flashOn = false;     // Whether the current flash is on or off
    static unsigned long lastUpdate = 0;  // Timestamp for the last state change

    const unsigned long flashDuration = 100;  // Duration of each flash (ms)
    const unsigned long pauseDuration = 1000;   // Pause after flashes (ms)

    // PHASE 0: Flashing phase (three rapid red flashes)
    if (phase == 0) {
        // Wait until enough time has passed for the next flash toggle
        if (millis() - lastUpdate < flashDuration) {
            return;
        }
        lastUpdate = millis();
        flashOn = !flashOn;  // Toggle the flash state

        // When turning off, count one complete flash
        if (!flashOn) {
            flashCount--;
        }

        // Set the LEDs: red when on, black when off
        if (flashOn) {
            fill_solid(leds, numLeds, CRGB::Red);
        } else {
            fill_solid(leds, numLeds, CRGB::Black);
        }
        FastLED.show();

        // If we've completed all flashes and are off, move to the pause phase
        if (flashCount <= 0 && !flashOn) {
            phase = 1;
            lastUpdate = millis();
        }
    }
    // PHASE 1: Pause phase (LEDs remain off)
    else {
        fill_solid(leds, numLeds, CRGB::Black);
        FastLED.show();

        // After the pause duration, reset the flashing sequence
        if (millis() - lastUpdate >= pauseDuration) {
            phase = 0;
            flashCount = 3;
            flashOn = false;
            lastUpdate = millis();
        }
    }
}
void AbtoLeds_ESP32::Speed(){
    static int phase = 0;           // 0 = waiting, 1 = flashing
    static int flashCount = 0;      // Number of flashes remaining in current strike
    static bool flashOn = false;    // Whether the current flash is on or off
    static unsigned long lastUpdate = 0;  // Timestamp of last state change
    static unsigned long waitTime = 0;    // How long to wait before next strike

    const unsigned long flashDuration = 50;  // Duration (ms) of each flash phase

    // PHASE 0: Waiting for the next lightning strike.
    if (phase == 0) {
        fill_solid(leds, numLeds, CRGB::Black);
        // Set a new wait time if not set.
        if (waitTime == 0) {
            waitTime = random(0, 200); // Wait time between 0 and 200 ms (adjust as needed)
            lastUpdate = millis();
        }
        // Check if it's time for a strike.
        if (millis() - lastUpdate >= waitTime) {
            phase = 1;                   // Switch to flashing phase.
            flashCount = random(2, 7);     // Randomly 2 to 6 flashes per strike.
            flashOn = false;             // Start with flash off.
            lastUpdate = millis();
        }
    }
    // PHASE 1: Flashing phase.
    else if (phase == 1) {
        if (millis() - lastUpdate >= flashDuration) {
            flashOn = !flashOn;         // Toggle flash state.
            lastUpdate = millis();
            // When turning off, count one complete flash.
            if (!flashOn) {
                flashCount--;
            }
        }
        // Choose a random color with a weighted chance:
        // ~40% Yellow, ~40% Green, ~20% Light Blue.
        int r = random(0, 10);
        CRGB flashColor;
        if (r < 3) {
            flashColor = CRGB::Yellow;
        } else if (r < 9) {
            flashColor = CRGB::Green;
        } else {
            // Light blue color (you can adjust the values for your preferred tone)
            flashColor = CRGB(173, 216, 230);  
        }

        // Set LED strip based on flash state.
        if (flashOn) {
            fill_solid(leds, numLeds, flashColor);
        } else {
            fill_solid(leds, numLeds, CRGB::Black);
        }
        // When done with all flashes, reset for the next strike.
        if (flashCount <= 0 && !flashOn) {
            phase = 0;
            waitTime = random(200, 500); // New wait time between strikes.
            lastUpdate = millis();
        }
    }
    FastLED.show();
}
void AbtoLeds_ESP32::Fade(){
    // Static variables to hold the fade state between calls.
    static unsigned long lastUpdate = 0;
    const unsigned long fadeDelay = 20;  // Time (in ms) between brightness updates
    static uint8_t brightness = 0;         // Current brightness (0-255)
    static int8_t direction = 1;           // Fade direction: 1 for up, -1 for down

    // Only update if enough time has passed
    if (millis() - lastUpdate < fadeDelay) {
        return;
    }
    lastUpdate = millis();

    // Update brightness
    brightness += direction;
    // Reverse the fade direction at the boundaries
    if (brightness == 0 || brightness == 255) {
        direction = -direction;
    }

    // Fill the LED strip with a green tone using the current brightness.
    // Hue 96 represents a green shade in FastLED.
    fill_solid(leds, numLeds, CHSV(96, 255, brightness));

    FastLED.show();
}
void AbtoLeds_ESP32::Rainbow(){
    static unsigned long lastUpdate = 0;
    const uint8_t rainbowDelay = 20;  // Delay between updates in ms
    static uint8_t startHue = 0;       // Starting hue for the rainbow

    // Only update when enough time has passed
    if (millis() - lastUpdate < rainbowDelay) {
        return;
    }
    lastUpdate = millis();

    // Fill the LED strip with a rainbow, starting at startHue.
    // The "7" defines the hue delta between adjacent LEDs.
    fill_rainbow(leds, numLeds, startHue, 7);

    // Increment the starting hue to create the moving effect.
    startHue++;

    // Update the LED strip.
    FastLED.show();
}
void AbtoLeds_ESP32::Water(){
    static bool initialized = false;
    static int phase = 0;      
    static int i = 0;          
    static int maxLevel = 0;  
    static unsigned long lastUpdate = 0;

    if (!initialized) {
        initialized = true;
        phase = 0;                      
        i = 0;
        maxLevel = random(numLeds / 2, numLeds);
        lastUpdate = millis();
        fill_solid(leds, numLeds, CRGB::Black);
        FastLED.setBrightness(150);     
    }

    if (millis() - lastUpdate < waterDelay) {
        return;
    }
    lastUpdate = millis();

    if (phase == 0) { 
        
        leds[i] = CRGB(0, 150, 255);
        i++;
        if (i >= maxLevel) {
            phase = 1;
            i = maxLevel - 1; 
        }
    } else {
        leds[i] = CRGB::Black;
        if (i > 0) {
            i--;
        } else {
            maxLevel = random(numLeds / 2, numLeds);
            phase = 0;
            i = 0;
        }
    }
    FastLED.show();
}
void AbtoLeds_ESP32::Idle(){
        // ------------------ LIGHTNING VARIABLES ------------------
        static bool lightningActive = false; // Flag: lightning override active?
        static int lightPhase = 0;           // 0 = waiting, 1 = flashing
        static int lightFlashCount = 0;      // Number of flashes remaining in strike
        static bool lightFlashOn = false;    // Current flash state
        static unsigned long lightLastUpdate = 0;  // Timestamp for lightning updates
        static unsigned long lightWaitTime = 0;    // How long to wait before next strike
        const unsigned long flashDuration = 50;    // Duration of each lightning flash (ms)
        
        // ------------------ FIRE VARIABLES ------------------
        static bool fireInitialized = false;
        static int firePhase = 0;       // 0 = ascending, 1 = descending
        static int fireJ = 0;           // Outer cycle counter (runs 5 cycles)
        static int fireI = 0;           // Current index for the fire effect
        static int fireRandomNum = 0;   // Random limit for the current fire cycle
        static unsigned long fireLastUpdate = 0;
        const int fireDelay = 5;        // Delay between fire effect steps (ms)
        
        // ------------------ LIGHTNING SCHEDULING ------------------
        // Schedule the next lightning strike if not already scheduled.
        static unsigned long nextLightningTime = 0;
        if (nextLightningTime == 0) {
            nextLightningTime = millis() + random(200, 2000); // First strike after 5-15 sec
        }
        // Trigger lightning if it's time and not already active.
        if (!lightningActive && millis() >= nextLightningTime) {
            lightningActive = true;
            // Initialize lightning state:
            lightPhase = 1; // Immediately enter flashing phase.
            lightFlashCount = random(2, 7);  // 2 to 6 flashes.
            lightFlashOn = false;
            lightLastUpdate = millis();
        }
        
        // ------------------ LIGHTNING EFFECT ------------------
        if (lightningActive) {
            // Lightning Phase 1: Flashing.
            if (millis() - lightLastUpdate >= flashDuration) {
                lightFlashOn = !lightFlashOn; // Toggle flash state.
                lightLastUpdate = millis();
                if (!lightFlashOn) { // When turning off, count a flash.
                    lightFlashCount--;
                }
            }
            // Choose a flash color with weighted chances:
            // ~30% Yellow, ~60% Green, ~10% Light Blue.
            int r = random(0, 10);
            CRGB flashColor;
            if (r < 3) {
                flashColor = CRGB::Yellow;
            } else if (r < 9) {
                flashColor = CRGB::Green;
            } else {
                flashColor = CRGB(173, 216, 230); // Light blue
            }
            // Display lightning flash.
            if (lightFlashOn) {
                fill_solid(leds, numLeds, flashColor);
            } else {
                fill_solid(leds, numLeds, CRGB::Black);
            }
            // If all flashes are done, exit lightning mode.
            if (lightFlashCount <= 0 && !lightFlashOn) {
                lightningActive = false;
                nextLightningTime = millis() + random(200, 3000);
                // Reset lightning variables.
                lightWaitTime = 0;
                lightPhase = 0;
            }
            FastLED.show();
            return; // Exit: lightning overrides fire.
        }
        
        // ------------------ FIRE EFFECT ------------------
        // One-time initialization for the fire effect.
        if (!fireInitialized) {
            fireJ = 0;
            firePhase = 0;      // Start with ascending phase.
            fireI = 0;
            fireRandomNum = random(int(numLeds * 0.7), numLeds);
            fireLastUpdate = millis();
            fill_solid(leds, numLeds, CRGB::Black);
            FastLED.setBrightness(random(30, 100));
            fireInitialized = true;
        }
        
        // Only update the fire effect if enough time has passed.
        if (millis() - fireLastUpdate < fireDelay) {
            FastLED.show();
            return;
        }
        fireLastUpdate = millis();
        
        // --- Fire Ascending Phase ---
        if (firePhase == 0) {
            // Occasionally fill a larger portion.
            if (random(0, 10) > 7) {
                int limit = int(numLeds * 0.40);
                for (int k = 0; k < limit && k < numLeds; k++) {
                    leds[k] = CRGB(0, 204, 0);
                }
            } else {
                int limit = int(numLeds * 0.25);
                for (int k = 0; k < limit && k < numLeds; k++) {
                    leds[k] = CRGB(0, 204, 0);
                }
            }
            // Light up the current fire index.
            if (fireI < numLeds) {
                leds[fireI] = CRGB(0, 204, 0);
            }
            // Additional effect: modify some LEDs based on fireI.
            int innerLimit = int(numLeds * 0.25 * (fireI / 15.0));
            for (int k = 0; k < innerLimit && k < numLeds; k++){
                leds[k] = CRGB(fireI, 204, fireI);
            }
            fireI++;
            if (fireI >= fireRandomNum) {
                firePhase = 1;  // Switch to descending phase.
            }
        }
        // --- Fire Descending Phase ---
        else {
            if (random(0, 10) > 7) {
                int limit = int(numLeds * 0.40);
                for (int k = 0; k < limit && k < numLeds; k++){
                    leds[k] = CRGB(0, 204, 0);
                }
            } else {
                int limit = int(numLeds * 0.25);
                for (int k = 0; k < limit && k < numLeds; k++){
                    leds[k] = CRGB(0, 204, 0);
                }
            }
            if (fireI < numLeds) {
                leds[fireI] = CRGB::Black;
            }
            fireI--;
            if (fireI <= 0) {
                fireJ++;
                if (fireJ < 5) {
                    fireRandomNum = random(int(numLeds * 0.7), numLeds);
                    firePhase = 0;
                    fireI = 0;
                } else {
                    fireJ = 0;
                    fireRandomNum = random(int(numLeds * 0.7), numLeds);
                    firePhase = 0;
                    fireI = 0;
                }
            }
        }
        FastLED.show();
}
void AbtoLeds_ESP32::Good(){
    static int phase = 0;         // 0 = flashing, 1 = steady
    static int flashCount = 3;    // Number of quick flashes
    static bool flashOn = false;  // Current flash state
    static unsigned long lastUpdate = 0;  // For timing the flash toggles
    static unsigned long steadyStart = 0; // Time when steady phase began

    const unsigned long flashDuration = 50;   // Duration (ms) for each flash toggle
    const unsigned long steadyDuration = 1000;  // Duration (ms) for the steady green phase

    // --- FLASHING PHASE ---
    if (phase == 0) {
        if (millis() - lastUpdate < flashDuration) {
            return;
        }
        lastUpdate = millis();
        flashOn = !flashOn; // Toggle flash state

        // When turning off, count one complete flash.
        if (!flashOn) {
            flashCount--;
        }

        // Set LED strip based on flash state.
        if (flashOn) {
            fill_solid(leds, numLeds, CRGB::Green);
        } else {
            fill_solid(leds, numLeds, CRGB::Black);
        }
        FastLED.show();

        // Transition to steady phase after flashing is complete.
        if (flashCount <= 0 && !flashOn) {
            phase = 1;
            steadyStart = millis();
        }
    }
    // --- STEADY PHASE ---
    else if (phase == 1) {
        // Keep the strip solid green.
        fill_solid(leds, numLeds, CRGB::Green);
        FastLED.show();
        // After the steady duration, reset the animation.
        if (millis() - steadyStart >= steadyDuration) {
            phase = 0;
            flashCount = 3;
            flashOn = false;
            lastUpdate = millis();
        }
    }
}
