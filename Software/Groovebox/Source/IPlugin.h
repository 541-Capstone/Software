#pragma once

#include "Utilities.h"

class IPlugin {
public:
	virtual void handleMidiEvent(juce::MidiMessage msg, int sampleNumber, bool record);
	//virtual Helpers::PluginType getPluginType();
private:
};