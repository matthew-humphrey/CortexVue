#ifndef CONFIG_H
#define CONFIG_H

#define FIRMWARE_VERSION "v1.0.0" // Version of the firmware

// Pin Definitions
#define FOOTSWITCH_1_PIN 2    // D2
#define FOOTSWITCH_2_PIN 3    // D3
#define FOOTSWITCH_3_PIN 4    // D4
#define FOOTSWITCH_4_PIN 5    // D5

// OLED Display
#define SCREEN_WIDTH 128      // OLED display width, in pixels
#define SCREEN_HEIGHT 32      // OLED display height, in pixels (changed from 64 to 32)
#define OLED_RESET    -1      // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C   // Address for 128x32 displays is typically 0x3C

// MIDI Configuration
#define MIDI_CHANNEL 1        // MIDI channel (1-16)

// Programming mode timings
#define HOLD_TIME_FOR_PROGRAM   5000  // Time to hold switch for programming mode (ms)
#define PROGRAM_TIMEOUT        10000  // Timeout for programming mode (ms)

// EEPROM addresses for storing footswitch assignments
#define EEPROM_VALID_FLAG      0   // Address to store validation flag
#define EEPROM_FS1_COMMAND     1   // Address to store FS1 command index
#define EEPROM_FS2_COMMAND     2   // Address to store FS2 command index
#define EEPROM_FS3_COMMAND     3   // Address to store FS3 command index
#define EEPROM_FS4_COMMAND     4   // Address to store FS4 command index
#define EEPROM_VALID_VALUE     42  // Value to indicate EEPROM has been initialized

// Debounce time in milliseconds
#define DEBOUNCE_TIME 50

#endif // CONFIG_H