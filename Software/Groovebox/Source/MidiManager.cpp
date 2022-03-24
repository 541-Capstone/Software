#include "MidiManager.h"

MidiManager::MidiManager(int rate) {
	midiBuffer = new juce::MidiBuffer();
	sampleRate = rate;
	midiService = new MidiService();
	midiService->setSampleRate(sampleRate);
	midiService->setMidiBuffer(midiBuffer);
	addAndMakeVisible(midiService);
}

MidiManager::~MidiManager() {
	delete midiBuffer;
}

//Sets the midi buffer for the manager and associated midi service, then deletes the old buffer
void MidiManager::setMidiBuffer(juce::MidiBuffer* buffer) {
	midiService->setMidiBuffer(midiBuffer);
	midiBuffer = buffer;
}

void MidiManager::setSampleRate(int newRate) {
	midiService->setSampleRate(newRate);
	sampleRate = newRate;
}

void MidiManager::setSampleRateFromTransport(te::TransportControl& t) {
	sampleRate = (int)t.engine.getDeviceManager().getSampleRate();
	midiService->setSampleRate(sampleRate);
}

void MidiManager::actionListenerCallback(const juce::String& message) {

}

