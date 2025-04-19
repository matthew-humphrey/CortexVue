#include "midi_controller.h"
#include "../include/config.h"
#include "display.h"

// Create a Serial MIDI port
midi::SerialMIDI<HardwareSerial, MyMidiSettings> serialMIDI(Serial);

// Create instance of the MIDI interface
midi::MidiInterface<midi::SerialMIDI<HardwareSerial, MyMidiSettings>> MIDI(serialMIDI);

MidiController midiController;

void MidiController::begin() {
  // Initialize MIDI
  MIDI.begin(MIDI_CHANNEL_OMNI); // Listen to all channels
  
  // We'll be using Serial for MIDI output
  Serial.begin(31250); // Standard MIDI baud rate
}

void MidiController::sendFootswitchCC(uint8_t switchNumber, bool state) {
  uint8_t ccNumber;
  
  // Map switch number to CC number based on Quad Cortex functions
  switch (switchNumber) {
    case 1:
      ccNumber = QC_TUNER_CC;
      break;
    case 2:
      ccNumber = QC_MODE_SWITCH_CC;
      break;
    case 3:
      ccNumber = QC_MODE_SWITCH_CC;
      break;
    case 4:
      ccNumber = QC_GIG_VIEW_CC;
      break;
    default:
      return; // Invalid switch number
  }
  
  // For simplicity, sending 127 for ON and 0 for OFF
  uint8_t value = state ? 127 : 0;
  
  // Send the Control Change message
  MIDI.sendControlChange(ccNumber, value, MIDI_CHANNEL);
  
  // Show on display
  oled.showMidiMessage(0xB0, MIDI_CHANNEL, ccNumber, value);
}

void MidiController::sendNote(uint8_t note, bool on, uint8_t velocity) {
  if (on) {
    MIDI.sendNoteOn(note, velocity, MIDI_CHANNEL);
    oled.showMidiMessage(0x90, MIDI_CHANNEL, note, velocity);
  } else {
    MIDI.sendNoteOff(note, velocity, MIDI_CHANNEL);
    oled.showMidiMessage(0x80, MIDI_CHANNEL, note, velocity);
  }
}

void MidiController::sendControlChange(uint8_t controller, uint8_t value) {
  MIDI.sendControlChange(controller, value, MIDI_CHANNEL);
  oled.showMidiMessage(0xB0, MIDI_CHANNEL, controller, value);
}

void MidiController::sendProgramChange(uint8_t program) {
  MIDI.sendProgramChange(program, MIDI_CHANNEL);
  oled.showMidiMessage(0xC0, MIDI_CHANNEL, program, 0);
}

void MidiController::update() {
  // Process incoming MIDI messages (if needed)
  if (MIDI.read()) {
    // Handle incoming MIDI messages here if needed
  }
}