#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>
#include "display.h"
#include "footswitches.h"
#include "midi_controller.h"
#include "command_table.h"
#include "../include/config.h"

// Device name for display
const char* DEVICE_NAME = "MIDI Controller";

// Timing variables for programming mode
unsigned long switchHoldStartTime = 0;
unsigned long lastProgramActionTime = 0;
bool switchBeingHeld = false;
uint8_t heldSwitch = 0;

// Original command assignments (for canceling)
uint8_t originalCommands[4];

// Flash state for programming mode
bool flashState = true;
unsigned long lastFlashTime = 0;
const unsigned long FLASH_INTERVAL = 500; // Flash every 500ms

void setup() {
  // Initialize I2C for OLED
  Wire.begin();
  
  // Initialize the display
  if (!oled.begin()) {
    // If display initialization fails, we'll still continue but won't have visual feedback
  }
  
  // Show splash screen
  oled.showSplashScreen(DEVICE_NAME);
  
  // Initialize footswitches
  footswitches.begin();
  
  // Initialize MIDI
  midiController.begin();
  
  // Load footswitch assignments from EEPROM
  loadFootswitchAssignments(&footswitchAssignments[0], &footswitchAssignments[1], 
                           &footswitchAssignments[2], &footswitchAssignments[3]);
  
  // Show initial footswitch states
  oled.updateFootswitchStates(
    footswitches.getState(1),
    footswitches.getState(2),
    footswitches.getState(3),
    footswitches.getState(4)
  );
}

void loop() {
  unsigned long currentTime = millis();
  
  // Check for footswitch state changes
  if (footswitches.update()) {
    // Get which switch changed
    uint8_t changedSwitch = footswitches.getLastChanged();
    
    if (changedSwitch > 0) {
      // Get the new state
      bool newState = footswitches.getState(changedSwitch);
      
      // Handle programming mode
      if (oled.inProgramMode) {
        if (newState) { // Button pressed
          lastProgramActionTime = currentTime; // Reset timeout
          
          if (changedSwitch == oled.programmingSwitch) {
            // Cycle to next command
            oled.selectedCommand = (oled.selectedCommand + 1) % getCommandCount();
            oled.showProgramMode(changedSwitch, oled.selectedCommand);
          } else {
            // Different switch pressed - save the selected command
            footswitchAssignments[oled.programmingSwitch - 1] = oled.selectedCommand;
            
            // Save to EEPROM
            saveFootswitchAssignments(
              footswitchAssignments[0],
              footswitchAssignments[1],
              footswitchAssignments[2],
              footswitchAssignments[3]
            );
            
            // Show saved confirmation
            oled.showCommandSaved(oled.programmingSwitch, oled.selectedCommand);
            
            // Back to normal mode
            oled.updateFootswitchStates(
              footswitches.getState(1),
              footswitches.getState(2),
              footswitches.getState(3),
              footswitches.getState(4)
            );
          }
        }
      }
      // Normal mode operation
      else {
        if (newState) { // Switch pressed
          // Start tracking for hold detection
          switchBeingHeld = true;
          heldSwitch = changedSwitch;
          switchHoldStartTime = currentTime;
          
          // Execute normal command
          executeCommand(footswitchAssignments[changedSwitch - 1], true, &oled.state[changedSwitch - 1]);
        } 
        else { // Switch released
          // Check if this was the switch being held
          if (switchBeingHeld && heldSwitch == changedSwitch) {
            switchBeingHeld = false;
          }
          
          // Handle button release for momentary commands
          const MidiCommand* cmd = getCommand(footswitchAssignments[changedSwitch - 1]);
          if (cmd->type == TYPE_CC_MOMENTARY) {
            executeCommand(footswitchAssignments[changedSwitch - 1], false, &oled.state[changedSwitch - 1]);
          }
        }
        
        // Update display
        oled.updateFootswitchStates(
          footswitches.getState(1),
          footswitches.getState(2),
          footswitches.getState(3),
          footswitches.getState(4)
        );
      }
    }
  }
  
  // Check for switch hold to enter programming mode
  if (switchBeingHeld && !oled.inProgramMode) {
    if (currentTime - switchHoldStartTime >= HOLD_TIME_FOR_PROGRAM) {
      switchBeingHeld = false;
      
      // Save original assignments in case user cancels
      for (int i = 0; i < 4; i++) {
        originalCommands[i] = footswitchAssignments[i];
      }
      
      // Enter programming mode
      oled.showProgramMode(heldSwitch, footswitchAssignments[heldSwitch - 1]);
      lastProgramActionTime = currentTime;
    }
  }
  
  // Check for programming mode timeout
  if (oled.inProgramMode && (currentTime - lastProgramActionTime >= PROGRAM_TIMEOUT)) {
    // Restore original commands
    for (int i = 0; i < 4; i++) {
      footswitchAssignments[i] = originalCommands[i];
    }
    
    // Show canceled message
    oled.showProgramCanceled();
    
    // Return to normal display
    oled.updateFootswitchStates(
      footswitches.getState(1),
      footswitches.getState(2),
      footswitches.getState(3),
      footswitches.getState(4)
    );
  }
  
  // Handle flashing in programming mode
  if (oled.inProgramMode && (currentTime - lastFlashTime >= FLASH_INTERVAL)) {
    flashState = !flashState;
    oled.flashProgramCommand(flashState);
    lastFlashTime = currentTime;
  }
  
  // Process any incoming MIDI messages
  midiController.update();
  
  // Small delay to prevent excessive CPU usage
  delay(1);
}