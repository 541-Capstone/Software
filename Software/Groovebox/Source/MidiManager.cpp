#include "MidiManager.h"

MidiManager::MidiManager(int rate) {
	midiBuffer = new juce::MidiBuffer();
	sampleRate = rate;
	midiMsg = new juce::MidiMessage();
	midiService = new MidiService(sampleRate, midiBuffer);
	midiService->addActionListener(this);
	midiService->setMidiMessagePointer(midiMsg);
	//auto midiMsg = std::make_shared<juce::MidiMessage>();
	addAndMakeVisible(midiService);
}

MidiManager::~MidiManager() {
	delete midiBuffer;
	delete midiService;
	delete midiMsg;
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

juce::MidiMessage* MidiManager::getMidiMessage() {
	return midiMsg;
}

void MidiManager::actionListenerCallback(const juce::String& message) {
	LOG("Manager heard a message: " + message);
	sendActionMessage("Universal");
}

