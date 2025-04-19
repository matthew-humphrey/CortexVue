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
  display.println(F("Cortex Vue"));
  
  // Display device name
  display.setCursor(0, 10);
  display.println(deviceName);
  
  // Display version
  display.setCursor(0, 20);
  display.println(F(FIRMWARE_VERSION));
  
  display.display();
  delay(2000); // Show splash for 2 seconds
  
  // After splash, show footswitch labels
  display.clearDisplay();  
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
  display.print(getCommandShortName(footswitchAssignments[0]));
  
  display.setCursor(64, 0);
  display.print(F("2:"));
  display.print(getCommandShortName(footswitchAssignments[1]));
  
  // Second row - Footswitches 3 & 4
  display.setCursor(0, 10);
  display.print(F("3:"));
  display.print(getCommandShortName(footswitchAssignments[2]));
  
  display.setCursor(64, 10);
  display.print(F("4:"));
  display.print(getCommandShortName(footswitchAssignments[3]));
}

void Display::showMidiMessage(uint8_t type, uint8_t channel, uint8_t data1, uint8_t data2, uint8_t commandIndex) {
  // Write on the bottom area of the display - clear the area first
  display.fillRect(0, 21, SCREEN_WIDTH, 11, SSD1306_BLACK);
  display.setCursor(0, 22);
  
  // Show command name instead of MIDI details
  display.print(getCommandName(commandIndex));
  
  display.display();
}

void Display::showFunctionPreview(uint8_t commandIndex) {
  // Write on the bottom area of the display - clear the area first
  display.fillRect(0, 21, SCREEN_WIDTH, 11, SSD1306_BLACK);
  display.setCursor(0, 22);
  
  // Show command name 
  display.print(getCommandName(commandIndex));
  
  display.display();
}

void Display::clearMidiMessageArea() {
  // Clear just the bottom message area
  display.fillRect(0, 21, SCREEN_WIDTH, 11, SSD1306_BLACK);
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

  clearMidiMessageArea(); // Clear the MIDI message area
  
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