#ifndef FOOTSWITCHES_H
#define FOOTSWITCHES_H

#include <Arduino.h>

class Footswitches {
  public:
    // Initialize footswitches
    void begin();
    
    // Update and check footswitch states, returns true if any state changed
    bool update();
    
    // Get current states
    bool getState(uint8_t switchNumber);
    
    // Get the footswitch that changed its state most recently
    uint8_t getLastChanged();
    
    // Get timestamp of last state change
    unsigned long getLastChangeTime();
    
  private:
    bool currentStates[4] = {false, false, false, false};
    bool previousStates[4] = {false, false, false, false};
    unsigned long lastDebounceTime[4] = {0, 0, 0, 0};
    uint8_t lastChangedSwitch = 0;
    unsigned long lastChangeTimestamp = 0;
    
    // Read the raw state (without debouncing)
    bool readRawState(uint8_t pin);
};

extern Footswitches footswitches;

#endif // FOOTSWITCHES_H