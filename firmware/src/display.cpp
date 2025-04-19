#include "display.h"
#include "../include/config.h"
#include <Wire.h>
#include "command_table.h"

Display oled;

bool Display::begin() {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    return false;
  }
  
  // Clear the buffer
  display.clearDisplay();
  display.display();
  
  // Set text color
  display.setTextColor(SSD1306_WHITE);
  
  return true;
}

void Display::showSplashScreen(const char* deviceName) {
  display.clearDisplay();
  
  // Display title
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F("MIDI Controller"));
  
  // Display device name
  display.setCursor(0, 10);
  display.println(deviceName);
  
  // Display version
  display.setCursor(0, 20);
  display.println(F("v1.0.0"));
  
  display.display();
  delay(2000); // Show splash for 2 seconds
  
  // After splash, show footswitch labels
  display.clearDisplay();
  
  display.setCursor(0, 0);
  display.println(F("1:Tuner 2:Mode"));
  
  display.setCursor(0, 10);
  display.println(F("3:Preset 4:Gig"));
  
  display.display();
  delay(2000); // Show labels for 2 seconds
}

void Display::updateFootswitchStates(bool sw1, bool sw2, bool sw3, bool sw4) {
  // Only clear the top part of the display (leave MIDI message area intact)
  display.fillRect(0, 0, SCREEN_WIDTH, 20, SSD1306_BLACK);
  
  // Draw footswitch states
  drawFootswitchStates(sw1, sw2, sw3, sw4);
  
  // Create a horizontal line to separate footswitch states from MIDI messages
  display.drawLine(0, 20, SCREEN_WIDTH, 20, SSD1306_WHITE);
  
  display.display();
}

void Display::drawFootswitchStates(bool sw1, bool sw2, bool sw3, bool sw4) {
  display.setTextSize(1);
  
  // First row - Footswitches 1 & 2
  display.setCursor(0, 0);
  display.print(F("1:"));
  
  // Get the name of the command assigned to FS1, show short version
  const char* fs1Name = getCommandName(footswitchAssignments[0]);
  char fs1ShortName[8] = {0}; // Short name for display space
  strncpy(fs1ShortName, fs1Name, 7);
  display.print(fs1ShortName);
  
  display.setCursor(64, 0);
  display.print(F("2:"));
  
  // Get the name of the command assigned to FS2, show short version
  const char* fs2Name = getCommandName(footswitchAssignments[1]);
  char fs2ShortName[8] = {0};
  strncpy(fs2ShortName, fs2Name, 7);
  display.print(fs2ShortName);
  
  // Second row - Footswitches 3 & 4
  display.setCursor(0, 10);
  display.print(F("3:"));
  
  // Get the name of the command assigned to FS3, show short version
  const char* fs3Name = getCommandName(footswitchAssignments[2]);
  char fs3ShortName[8] = {0};
  strncpy(fs3ShortName, fs3Name, 7);
  display.print(fs3ShortName);
  
  display.setCursor(64, 10);
  display.print(F("4:"));
  
  // Get the name of the command assigned to FS4, show short version
  const char* fs4Name = getCommandName(footswitchAssignments[3]);
  char fs4ShortName[8] = {0};
  strncpy(fs4ShortName, fs4Name, 7);
  display.print(fs4ShortName);
}

void Display::showMidiMessage(uint8_t type, uint8_t channel, uint8_t data1, uint8_t data2) {
  // Write on the bottom area of the display - clear the area first
  display.fillRect(0, 21, SCREEN_WIDTH, 11, SSD1306_BLACK);
  display.setCursor(0, 22);
  
  // Show MIDI message type
  if (type == 0xB0) {
    display.print(F("CC "));
  } else if (type == 0x90) {
    display.print(F("Note On "));
  } else if (type == 0x80) {
    display.print(F("Note Off "));
  } else {
    display.print(F("MIDI 0x"));
    display.print(type, HEX);
    display.print(F(" "));
  }
  
  // Show channel and data
  display.print(F("Ch:"));
  display.print(channel);
  display.print(F(" "));
  display.print(data1);
  display.print(F(":"));
  display.print(data2);
  
  display.display();
}

void Display::clear() {
  display.clearDisplay();
  display.display();
}

void Display::showProgramMode(uint8_t switchNumber, uint8_t commandIndex) {
  inProgramMode = true;
  programmingSwitch = switchNumber;
  selectedCommand = commandIndex;
  
  display.clearDisplay();
  display.setTextSize(1);
  
  // Show programming mode header
  display.setCursor(0, 0);
  display.println(F("PROGRAM MODE"));
  
  // Show switch number
  display.setCursor(0, 10);
  display.print(F("FS"));
  display.print(switchNumber);
  display.print(F(": "));
  
  // Show command name
  display.setCursor(0, 20);
  display.print(getCommandName(commandIndex));
  
  display.display();
}

void Display::flashProgramCommand(bool showText) {
  // Only update the command area (not the whole screen)
  display.fillRect(0, 20, SCREEN_WIDTH, 12, SSD1306_BLACK);
  
  if (showText) {
    display.setCursor(0, 20);
    display.print(getCommandName(selectedCommand));
  }
  
  display.display();
}

void Display::showCommandSaved(uint8_t switchNumber, uint8_t commandIndex) {
  display.clearDisplay();
  display.setTextSize(1);
  
  display.setCursor(0, 0);
  display.println(F("SAVED!"));
  
  display.setCursor(0, 10);
  display.print(F("FS"));
  display.print(switchNumber);
  display.print(F(" = "));
  
  display.setCursor(0, 20);
  display.print(getCommandName(commandIndex));
  
  display.display();
  delay(1500); // Show for 1.5 seconds
  
  // Return to normal mode
  inProgramMode = false;
}

void Display::showProgramCanceled() {
  display.clearDisplay();
  display.setTextSize(1);
  
  display.setCursor(0, 10);
  display.println(F("PROGRAMMING"));
  display.setCursor(0, 20);
  display.println(F("CANCELED"));
  
  display.display();
  delay(1500); // Show for 1.5 seconds
  
  // Return to normal mode
  inProgramMode = false;
}