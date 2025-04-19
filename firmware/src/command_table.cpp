#include "command_table.h"
#include "midi_controller.h"
#include "../include/config.h"
#include <EEPROM.h>

// Array of footswitch assignments - which command index is assigned to each footswitch
uint8_t footswitchAssignments[4] = {0, 1, 2, 3}; // Default assignments

// Table of available MIDI commands
const MidiCommand commandTable[] = {
  // Quad Cortex specific commands
  {"Tuner Toggle", TYPE_CC_TOGGLE, QC_TUNER_CC, 127, 0, 0, true},
  {"Mode: Stomp/Scene", TYPE_CC_CYCLE, QC_MODE_SWITCH_CC, 1, 2, 0, true},
  {"Mode: Preset", TYPE_CC_FIXED, QC_MODE_SWITCH_CC, 0, 0, 0, false},
  {"Gig View Toggle", TYPE_CC_TOGGLE, QC_GIG_VIEW_CC, 127, 0, 0, true},
  
  // Additional generic commands
  {"CC 20 Toggle", TYPE_CC_TOGGLE, 20, 127, 0, 0, true},
  {"CC 21 Toggle", TYPE_CC_TOGGLE, 21, 127, 0, 0, true},
  {"CC 22 Toggle", TYPE_CC_TOGGLE, 22, 127, 0, 0, true},
  {"CC 23 Toggle", TYPE_CC_TOGGLE, 23, 127, 0, 0, true},
  
  // Momentary commands (on press/release)
  {"CC 24 Momentary", TYPE_CC_MOMENTARY, 24, 127, 0, 0, false},
  {"CC 25 Momentary", TYPE_CC_MOMENTARY, 25, 127, 0, 0, false},
  
  // Fixed value commands (always send the same value)
  {"CC 26 Value 64", TYPE_CC_FIXED, 26, 64, 0, 0, false},
  {"CC 27 Value 127", TYPE_CC_FIXED, 27, 127, 0, 0, false},
  
  // Cycle through 3 values
  {"CC 28 Cycle 3 Values", TYPE_CC_CYCLE, 28, 0, 64, 127, true}
};

// Get the number of available commands
uint8_t getCommandCount() {
  return sizeof(commandTable) / sizeof(MidiCommand);
}

// Get command by index
const MidiCommand* getCommand(uint8_t index) {
  if (index < getCommandCount()) {
    return &commandTable[index];
  }
  // Default to first command if index is out of range
  return &commandTable[0];
}

// Get command name for display
const char* getCommandName(uint8_t index) {
  return getCommand(index)->name;
}

// Execute a MIDI command based on its type
void executeCommand(uint8_t commandIndex, bool buttonState, bool* pState) {
  // Get the command
  const MidiCommand* cmd = getCommand(commandIndex);
  
  // Execute based on command type
  switch (cmd->type) {
    case TYPE_CC_TOGGLE:
      if (buttonState) { // Only on press
        // Toggle state
        *pState = !(*pState);
        // Send the appropriate value based on state
        midiController.sendControlChange(cmd->controller, *pState ? cmd->value1 : cmd->value2);
      }
      break;
      
    case TYPE_CC_MOMENTARY:
      // Send different values on press and release
      midiController.sendControlChange(cmd->controller, buttonState ? cmd->value1 : cmd->value2);
      break;
      
    case TYPE_CC_FIXED:
      if (buttonState) { // Only on press
        // Always send the same value
        midiController.sendControlChange(cmd->controller, cmd->value1);
      }
      break;
      
    case TYPE_CC_CYCLE:
      if (buttonState) { // Only on press
        // Cycle through values: 0 -> 1 -> 2 -> 0 -> ...
        if (*pState == 0) {
          *pState = 1;
          midiController.sendControlChange(cmd->controller, cmd->value1);
        } else if (*pState == 1) {
          *pState = 2;
          midiController.sendControlChange(cmd->controller, cmd->value2);
        } else {
          *pState = 0;
          midiController.sendControlChange(cmd->controller, cmd->value3);
        }
      }
      break;
  }
}

// Save footswitch assignments to EEPROM
void saveFootswitchAssignments(uint8_t fs1Cmd, uint8_t fs2Cmd, uint8_t fs3Cmd, uint8_t fs4Cmd) {
  EEPROM.write(EEPROM_VALID_FLAG, EEPROM_VALID_VALUE);
  EEPROM.write(EEPROM_FS1_COMMAND, fs1Cmd);
  EEPROM.write(EEPROM_FS2_COMMAND, fs2Cmd);
  EEPROM.write(EEPROM_FS3_COMMAND, fs3Cmd);
  EEPROM.write(EEPROM_FS4_COMMAND, fs4Cmd);
}

// Load footswitch assignments from EEPROM
void loadFootswitchAssignments(uint8_t* fs1Cmd, uint8_t* fs2Cmd, uint8_t* fs3Cmd, uint8_t* fs4Cmd) {
  // Check if EEPROM has been initialized
  if (EEPROM.read(EEPROM_VALID_FLAG) == EEPROM_VALID_VALUE) {
    // Read values
    *fs1Cmd = EEPROM.read(EEPROM_FS1_COMMAND);
    *fs2Cmd = EEPROM.read(EEPROM_FS2_COMMAND);
    *fs3Cmd = EEPROM.read(EEPROM_FS3_COMMAND);
    *fs4Cmd = EEPROM.read(EEPROM_FS4_COMMAND);
    
    // Validate values are within range
    uint8_t maxCmd = getCommandCount() - 1;
    if (*fs1Cmd > maxCmd) *fs1Cmd = 0;
    if (*fs2Cmd > maxCmd) *fs2Cmd = 1;
    if (*fs3Cmd > maxCmd) *fs3Cmd = 2;
    if (*fs4Cmd > maxCmd) *fs4Cmd = 3;
  } else {
    // Use defaults
    *fs1Cmd = 0; // Tuner Toggle
    *fs2Cmd = 1; // Mode Switch
    *fs3Cmd = 2; // Preset Mode
    *fs4Cmd = 3; // Gig View Toggle
  }
}