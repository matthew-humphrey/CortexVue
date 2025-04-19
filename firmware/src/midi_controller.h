#ifndef MIDI_CONTROLLER_H
#define MIDI_CONTROLLER_H

#include <Arduino.h>
#include <MIDI.h>

// Create MIDI interface instance
struct MyMidiSettings : public midi::DefaultSettings {
  static const bool UseRunningStatus = false; // Disable running status for hardware MIDI
  static const bool Use1ByteParsing = true;
  static const long BaudRate = 31250;
};

class MidiController {
  public:
    // Initialize MIDI functionality
    void begin();
    
    // Send CC message when footswitch is pressed/released
    void sendFootswitchCC(uint8_t switchNumber, bool state);
    
    // Send a note on/off message
    void sendNote(uint8_t note, bool on, uint8_t velocity = 127);
    
    // Send a MIDI Control Change message
    void sendControlChange(uint8_t controller, uint8_t value);
    
    // Send a MIDI Program Change message
    void sendProgramChange(uint8_t program);
    
    // Process MIDI input (if needed)
    void update();
};

extern MidiController midiController;
extern midi::SerialMIDI<HardwareSerial, MyMidiSettings> serialMIDI;
extern midi::MidiInterface<midi::SerialMIDI<HardwareSerial, MyMidiSettings>> MIDI;

#endif // MIDI_CONTROLLER_H