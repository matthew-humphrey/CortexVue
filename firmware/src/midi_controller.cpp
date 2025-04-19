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

void MidiController::sendControlChange(uint8_t controller, uint8_t value) {
  MIDI.sendControlChange(controller, value, MIDI_CHANNEL);
  // Display handling is now done in the main loop
}

void MidiController::sendNote(uint8_t note, bool on, uint8_t velocity) {
  if (on) {
    MIDI.sendNoteOn(note, velocity, MIDI_CHANNEL);
  } else {
    MIDI.sendNoteOff(note, velocity, MIDI_CHANNEL);
  }
  // Display handling is now done in the main loop
}

void MidiController::sendProgramChange(uint8_t program) {
  MIDI.sendProgramChange(program, MIDI_CHANNEL);
  // Display handling is now done in the main loop
}

void MidiController::update() {
  // Process incoming MIDI messages (if needed)
  if (MIDI.read()) {
    // Handle incoming MIDI messages here if needed
  }
}