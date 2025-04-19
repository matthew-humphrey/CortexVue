#include "footswitches.h"
#include "../include/config.h"

Footswitches footswitches;

void Footswitches::begin() {  
  // Initialize pins as inputs with pull-up resistors
  pinMode(FOOTSWITCH_1_PIN, INPUT_PULLUP);
  pinMode(FOOTSWITCH_2_PIN, INPUT_PULLUP);
  pinMode(FOOTSWITCH_3_PIN, INPUT_PULLUP);
  pinMode(FOOTSWITCH_4_PIN, INPUT_PULLUP);
  
  // Initial read of switch states
  currentStates[0] = !digitalRead(FOOTSWITCH_1_PIN); // Invert because pullup
  currentStates[1] = !digitalRead(FOOTSWITCH_2_PIN);
  currentStates[2] = !digitalRead(FOOTSWITCH_3_PIN);
  currentStates[3] = !digitalRead(FOOTSWITCH_4_PIN);
  
  // Copy to previous states
  memcpy(previousStates, currentStates, sizeof(currentStates));
}

bool Footswitches::readRawState(uint8_t pin) {
  // Invert the reading since we're using pull-up resistors
  // (LOW means the switch is pressed)
  return !digitalRead(pin);
}

bool Footswitches::update() {
  bool stateChanged = false;
  unsigned long currentTime = millis();
  
  // Array of pin numbers for easier iteration
  static const uint8_t pins[4] = {
    FOOTSWITCH_1_PIN, FOOTSWITCH_2_PIN, FOOTSWITCH_3_PIN, FOOTSWITCH_4_PIN
  };
  
  // Check each switch
  for (uint8_t i = 0; i < 4; i++) {
    // Read the current raw state
    bool reading = readRawState(pins[i]);
    
    // If the switch changed, due to noise or pressing
    if (reading != previousStates[i]) {
      // Reset the debouncing timer
      lastDebounceTime[i] = currentTime;
    }
    
    // If enough time has passed since the last state change
    if ((currentTime - lastDebounceTime[i]) > DEBOUNCE_TIME) {
      // If the current reading is different from the current state
      if (reading != currentStates[i]) {
        currentStates[i] = reading;
        lastChangedSwitch = i + 1; // Store which switch changed (1-based index)
        lastChangeTimestamp = currentTime;
        stateChanged = true;
      }
    }
    
    // Save the reading for the next comparison
    previousStates[i] = reading;
  }
  
  return stateChanged;
}

bool Footswitches::getState(uint8_t switchNumber) {
  if (switchNumber >= 1 && switchNumber <= 4) {
    return currentStates[switchNumber - 1];
  }
  return false;
}

uint8_t Footswitches::getLastChanged() {
  return lastChangedSwitch;
}

unsigned long Footswitches::getLastChangeTime() {
  return lastChangeTimestamp;
}