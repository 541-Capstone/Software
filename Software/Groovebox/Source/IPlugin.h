#pragma once

#include "Utilities.h"

class IPlugin {
public:
	virtual void handleMidiControlEvent(juce::MidiMessage msg);
	virtual void handleMidiNoteEvent(juce::MidiMessage msg);
	virtual void getPluginType();
private:
	std::shared_ptr<juce::MidiBuffer> midiBuffer;
};