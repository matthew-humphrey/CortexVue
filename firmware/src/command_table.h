#ifndef COMMAND_TABLE_H
#define COMMAND_TABLE_H

#include <Arduino.h>
#include <avr/pgmspace.h>

// Helper macro for defining flash strings (safe for global context)
#define FLASH_STR(string_literal) (reinterpret_cast<const __FlashStringHelper*>(PSTR(string_literal)))


// Command types
enum CommandType {
  TYPE_CC_TOGGLE,     // Toggle between two CC values
  TYPE_CC_MOMENTARY,  // Send one CC value on press, another on release
  TYPE_CC_FIXED,      // Send a fixed CC value
  TYPE_CC_CYCLE       // Cycle through multiple CC values
};

// Structure to define a MIDI command
struct MidiCommand {
  PGM_P name;        // Full name of the command (for programming mode)
  PGM_P shortName;   // Short name (7 chars or less) for main display
  CommandType type;                       // Type of command
  uint8_t controller;                     // CC number
  uint8_t value1;                         // Primary value
  uint8_t value2;                         // Secondary value (for toggle/momentary)
  uint8_t value3;                         // Optional third value (for cycle)
  bool stateTracking;                     // Whether to track state for this command
};

// Function to execute a MIDI command
void executeCommand(uint8_t commandIndex, bool buttonState);

// Get current state value for a command (for toggle and cycle types)
uint8_t getCommandState(uint8_t commandIndex);

// Set state for a command (for toggle and cycle types)
void setCommandState(uint8_t commandIndex, uint8_t state);

// Function to get the number of available commands
uint8_t getCommandCount();

// Function to get command by index
MidiCommand getCommand(uint8_t index);

// Get command name for a given index
const __FlashStringHelper* getCommandName(uint8_t index);

// Get short command name for a given index
const __FlashStringHelper* getCommandShortName(uint8_t index);

// Function to save footswitch assignments to EEPROM
void saveFootswitchAssignments(uint8_t fs1Cmd, uint8_t fs2Cmd, uint8_t fs3Cmd, uint8_t fs4Cmd);

// Function to load footswitch assignments from EEPROM
void loadFootswitchAssignments(uint8_t* fs1Cmd, uint8_t* fs2Cmd, uint8_t* fs3Cmd, uint8_t* fs4Cmd);

extern uint8_t footswitchAssignments[4]; // Stores which command is assigned to each footswitch

#endif // COMMAND_TABLE_H