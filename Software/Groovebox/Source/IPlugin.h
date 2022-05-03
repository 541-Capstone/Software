#pragma once

#include "Utilities.h"

class IPlugin {
public:
	struct Parameter {
		juce::String name;
		float value;
	};
	virtual void contextControl(const juce::MidiMessageMetadata& metadata) = 0;
	virtual Parameter getParameterValue(const int index) = 0;
    virtual ~IPlugin() = default;
private:
};
