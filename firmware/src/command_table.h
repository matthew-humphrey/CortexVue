#ifndef COMMAND_TABLE_H
#define COMMAND_TABLE_H

#include <Arduino.h>

// Command types
enum CommandType {
  TYPE_CC_TOGGLE,     // Toggle between two CC values
  TYPE_CC_MOMENTARY,  // Send one CC value on press, another on release
  TYPE_CC_FIXED,      // Send a fixed CC value
  TYPE_CC_CYCLE       // Cycle through multiple CC values
};

// Structure to define a MIDI command
struct MidiCommand {
  const char* name;        // Name of the command to display
  CommandType type;        // Type of command
  uint8_t controller;      // CC number
  uint8_t value1;          // Primary value
  uint8_t value2;          // Secondary value (for toggle/momentary)
  uint8_t value3;          // Optional third value (for cycle)
  bool stateTracking;      // Whether to track state for this command
};

// Function to execute a MIDI command
void executeCommand(uint8_t commandIndex, bool buttonState, bool* pState);

// Function to get the number of available commands
uint8_t getCommandCount();

// Function to get command by index
const MidiCommand* getCommand(uint8_t index);

// Get command name for a given index
const char* getCommandName(uint8_t index);

// Function to save footswitch assignments to EEPROM
void saveFootswitchAssignments(uint8_t fs1Cmd, uint8_t fs2Cmd, uint8_t fs3Cmd, uint8_t fs4Cmd);

// Function to load footswitch assignments from EEPROM
void loadFootswitchAssignments(uint8_t* fs1Cmd, uint8_t* fs2Cmd, uint8_t* fs3Cmd, uint8_t* fs4Cmd);

extern uint8_t footswitchAssignments[4]; // Stores which command is assigned to each footswitch

#endif // COMMAND_TABLE_H