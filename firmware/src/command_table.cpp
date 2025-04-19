#include "command_table.h"
#include "midi_controller.h"
#include "../include/config.h"
#include <EEPROM.h>
#include "display.h"

#define ARRAY_LENGTH(x) (sizeof(x) / sizeof((x)[0]))

// Quad Cortex CC Definitions
#define QC_TUNER_CC             45  // Tuner toggle (127=ON, 0=OFF)
#define QC_MODE_SWITCH_CC       47  // Mode switch (0=Preset, 1=Stomp, 2=Scene)
#define QC_GIG_VIEW_CC          46  // Gig View toggle (127=ON, 0=OFF)
#define QC_SCENE_SELECT_CC      43  // Select scene (0-7 for scenes A-H)
#define QC_LOOPER_PARAMETERS_CC 48  // Looper parameters (0=OPEN, 127=CLOSE)
#define QC_LOOPER_DUPLICATE_CC  49  // Looper duplicate / stop duplicate (127 toggle on/off)
#define QC_LOOPER_ONESHOT_CC    50  // Looper one shot (127 toggle on/off)
#define QC_LOOPER_HALFSPEED_CC  51  // Looper half speed (127 toggle on/off)
#define QC_LOOPER_PUNCH_CC      52  // Looper punch in/out (127 toggle punch in/out)
#define QC_LOOPER_RECORD_CC     53  // (0 - STOP, 127 - RECORD/OVERDUB/STOP)
#define QC_LOOPER_PLAY_STOP_CC  54
#define QC_LOOPER_REVERSE_CC    55  // Looper enable/disable reverse (127)
#define QC_LOOPER_UNDOREDO_CC   56  // Looper undo/redo (127)

// Define strings in PROGMEM
const char tunerToggleName[] PROGMEM = "Tuner Toggle";
const char tunerShortName[] PROGMEM = "Tuner";
const char presetSceneStompName[] PROGMEM = "Preset/Scene/Stomp";
const char modeShortName[] PROGMEM = "Mode";
const char gigViewToggleName[] PROGMEM = "Gig View Toggle";
const char gigViewShortName[] PROGMEM = "GigV";
const char bankSelectName[] PROGMEM = "Bank Select";
const char bankSelectShortName[] PROGMEM = "BankSel";
const char sceneAName[] PROGMEM = "Scene A";
const char sceneAShortName[] PROGMEM = "Scene A";
const char sceneBName[] PROGMEM = "Scene B";
const char sceneBShortName[] PROGMEM = "Scene B";
const char sceneCName[] PROGMEM = "Scene C";
const char sceneCShortName[] PROGMEM = "Scene C";
const char sceneDName[] PROGMEM = "Scene D";
const char sceneDShortName[] PROGMEM = "Scene D";
const char sceneEName[] PROGMEM = "Scene E";
const char sceneEShortName[] PROGMEM = "Scene E";
const char sceneFName[] PROGMEM = "Scene F";
const char sceneFShortName[] PROGMEM = "Scene F";
const char sceneGName[] PROGMEM = "Scene G";
const char sceneGShortName[] PROGMEM = "Scene G";
const char sceneHName[] PROGMEM = "Scene H";
const char sceneHShortName[] PROGMEM = "Scene H";
const char looperParamsName[] PROGMEM = "Looper Params";
const char looperParamsShortName[] PROGMEM = "LoopPrm";
const char looperDuplicateName[] PROGMEM = "Looper Duplicate";
const char looperDuplicateShortName[] PROGMEM = "LoopDup";
const char looperOneShotName[] PROGMEM = "Looper One Shot";
const char looperOneShotShortName[] PROGMEM = "LoopOne";
const char looperHalfSpeedName[] PROGMEM = "Looper Half Speed";
const char looperHalfSpeedShortName[] PROGMEM = "LoopHlf";
const char looperPunchName[] PROGMEM = "Looper Punch In/Out";
const char looperPunchShortName[] PROGMEM = "LoopPun";
const char looperRecordName[] PROGMEM = "Looper Record/Stop";
const char looperRecordShortName[] PROGMEM = "LoopRec";
const char looperRecordDubName[] PROGMEM = "Looper Record/Dub/Stop";
const char looperRecordDubShortName[] PROGMEM = "LoopRDS";
const char looperPlayStopName[] PROGMEM = "Looper Play/Stop";
const char looperPlayStopShortName[] PROGMEM = "LoopPly";
const char looperReverseName[] PROGMEM = "Looper Reverse";
const char looperReverseShortName[] PROGMEM = "LoopRev";

// Table of available MIDI commands
const MidiCommand commandTable[] PROGMEM = {
  // Display mode commands
  {tunerToggleName, tunerShortName, TYPE_CC_TOGGLE, QC_TUNER_CC, 127, 0, 0, true},
  {presetSceneStompName, modeShortName, TYPE_CC_CYCLE, QC_MODE_SWITCH_CC, 1, 2, 0, true},
  {gigViewToggleName, gigViewShortName, TYPE_CC_TOGGLE, QC_GIG_VIEW_CC, 127, 0, 0, true},
  {bankSelectName, bankSelectShortName, TYPE_CC_TOGGLE, QC_GIG_VIEW_CC, 0, 1, 0, true},

  // Scene select commands
  {sceneAName, sceneAShortName, TYPE_CC_FIXED, QC_SCENE_SELECT_CC, 0, 0, 0, false},
  {sceneBName, sceneBShortName, TYPE_CC_FIXED, QC_SCENE_SELECT_CC, 1, 0, 0, false},
  {sceneCName, sceneCShortName, TYPE_CC_FIXED, QC_SCENE_SELECT_CC, 2, 0, 0, false},
  {sceneDName, sceneDShortName, TYPE_CC_FIXED, QC_SCENE_SELECT_CC, 3, 0, 0, false},
  {sceneEName, sceneEShortName, TYPE_CC_FIXED, QC_SCENE_SELECT_CC, 4, 0, 0, false},
  {sceneFName, sceneFShortName, TYPE_CC_FIXED, QC_SCENE_SELECT_CC, 5, 0, 0, false},
  {sceneGName, sceneGShortName, TYPE_CC_FIXED, QC_SCENE_SELECT_CC, 6, 0, 0, false},
  {sceneHName, sceneHShortName, TYPE_CC_FIXED, QC_SCENE_SELECT_CC, 7, 0, 0, false},

  // Looper commands
  {looperParamsName, looperParamsShortName, TYPE_CC_TOGGLE, QC_LOOPER_PARAMETERS_CC, 0, 127, 0, true},
  {looperDuplicateName, looperDuplicateShortName, TYPE_CC_FIXED, QC_LOOPER_DUPLICATE_CC, 127, 0, 0, false},
  {looperOneShotName, looperOneShotShortName, TYPE_CC_FIXED, QC_LOOPER_ONESHOT_CC, 127, 0, 0, false},
  {looperHalfSpeedName, looperHalfSpeedShortName, TYPE_CC_FIXED, QC_LOOPER_HALFSPEED_CC, 127, 0, 0, false},
  {looperPunchName, looperPunchShortName, TYPE_CC_FIXED, QC_LOOPER_PUNCH_CC, 127, 0, 0, false},
  {looperRecordName, looperRecordShortName, TYPE_CC_FIXED, QC_LOOPER_RECORD_CC, 0, 0, 0, false},
  {looperRecordDubName, looperRecordDubShortName, TYPE_CC_FIXED, QC_LOOPER_RECORD_CC, 127, 0, 0, false},
  {looperPlayStopName, looperPlayStopShortName, TYPE_CC_FIXED, QC_LOOPER_PLAY_STOP_CC, 127, 0, 0, false},
  {looperReverseName, looperReverseShortName, TYPE_CC_FIXED, QC_LOOPER_REVERSE_CC, 127, 0, 0, false},
};

// Array of footswitch assignments - which command index is assigned to each footswitch
uint8_t footswitchAssignments[4] = {0, 1, 2, 3}; // Default assignments

// Array to track state for each command
uint8_t commandStates[ARRAY_LENGTH(commandTable)] = {0}; // Initialize all states to 0

// Get the number of available commands
uint8_t getCommandCount() {
  return sizeof(commandTable) / sizeof(MidiCommand);
}

// Get command by index - now we need to copy from PROGMEM
MidiCommand getCommand(uint8_t index) {
  MidiCommand result;
  if (index < getCommandCount()) {
    memcpy_P(&result, &commandTable[index], sizeof(MidiCommand));
  } else {
    memcpy_P(&result, &commandTable[0], sizeof(MidiCommand));
  }
  return result;
}

// Get command name for display
const __FlashStringHelper* getCommandName(uint8_t index) {
  MidiCommand cmd = getCommand(index);
  return reinterpret_cast<const __FlashStringHelper*>(cmd.name);
}

// Get short command name for display
const __FlashStringHelper* getCommandShortName(uint8_t index) {
  MidiCommand cmd = getCommand(index);
  return reinterpret_cast<const __FlashStringHelper*>(cmd.shortName);
}

// Get current state for a command
uint8_t getCommandState(uint8_t commandIndex) {
  if (commandIndex < getCommandCount()) {
    return commandStates[commandIndex];
  }
  return 0;
}

// Set state for a command
void setCommandState(uint8_t commandIndex, uint8_t state) {
  if (commandIndex < getCommandCount()) {
    commandStates[commandIndex] = state;
  }
}

// Execute a MIDI command based on its type
void executeCommand(uint8_t commandIndex, bool buttonState) {
  // Get the command
  MidiCommand cmd = getCommand(commandIndex);
  uint8_t currentState = getCommandState(commandIndex);
  
  // Execute based on command type
  switch (cmd.type) {
    case TYPE_CC_TOGGLE:
      if (buttonState) { // Only on press
        // Toggle state between 0 and 1
        currentState = !currentState;
        setCommandState(commandIndex, currentState);
        // Send the appropriate value based on state
        midiController.sendControlChange(cmd.controller, currentState ? cmd.value1 : cmd.value2);
      }
      break;
      
    case TYPE_CC_MOMENTARY:
      // Send different values on press and release
      midiController.sendControlChange(cmd.controller, buttonState ? cmd.value1 : cmd.value2);
      break;
      
    case TYPE_CC_FIXED:
      if (buttonState) { // Only on press
        // Always send the same value
        midiController.sendControlChange(cmd.controller, cmd.value1);
      }
      break;
      
    case TYPE_CC_CYCLE:
      if (buttonState) { // Only on press
        // Cycle through values: 0 -> 1 -> 2 -> 0 -> ...
        uint8_t valueToSend;
        
        // Update state (cycle through 0, 1, 2)
        currentState = (currentState + 1) % 3;
        setCommandState(commandIndex, currentState);
        
        // Send appropriate value based on state
        switch (currentState) {
          case 0:
            valueToSend = cmd.value3;
            break;
          case 1:
            valueToSend = cmd.value1;
            break;
          case 2:
            valueToSend = cmd.value2;
            break;
        }
        
        midiController.sendControlChange(cmd.controller, valueToSend);
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
    *fs1Cmd = 0;
    *fs2Cmd = 1;
    *fs3Cmd = 2;
    *fs4Cmd = 3;
  }
}