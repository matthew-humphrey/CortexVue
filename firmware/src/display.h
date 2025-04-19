#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class Display {
  public:
    // Initialize the display
    bool begin();
    
    // Update display with footswitch states
    void updateFootswitchStates(bool sw1, bool sw2, bool sw3, bool sw4);
    
    // Display a MIDI message that was sent
    void showMidiMessage(uint8_t type, uint8_t channel, uint8_t data1, uint8_t data2, uint8_t commandIndex);
    
    // Show function preview (when button is pressed but not yet executed)
    void showFunctionPreview(uint8_t commandIndex);
    
    // Clear the MIDI message area only
    void clearMidiMessageArea();
    
    // Clear the display
    void clear();
    
    // Show splash screen on startup
    void showSplashScreen(const char* deviceName);
    
    // Show programming mode screen
    void showProgramMode(uint8_t switchNumber, uint8_t commandIndex);
    
    // Flash the currently selected command
    void flashProgramCommand(bool showText);
    
    // Show command saved confirmation
    void showCommandSaved(uint8_t switchNumber, uint8_t commandIndex);
    
    // Show program mode canceled message
    void showProgramCanceled();
    
    // Variables for programming mode
    bool inProgramMode = false;
    uint8_t programmingSwitch = 0;
    uint8_t selectedCommand = 0;
    
    // Track last pressed footswitch for MIDI message display
    uint8_t lastPressedSwitch = 0;
    
  private:
    Adafruit_SSD1306 display;
    void drawFootswitchStates(bool sw1, bool sw2, bool sw3, bool sw4);
};

extern Display oled;

#endif // DISPLAY_H