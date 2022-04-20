#pragma once

#include "Utilities.h"

class IPlugin {
public:
	virtual void contextControl(const juce::MidiMessageMetadata& metadata) = 0;
	//virtual Helpers::PluginType getPluginType();
private:
};