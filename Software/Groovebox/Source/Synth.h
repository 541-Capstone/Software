/*
  ==============================================================================

    Synth.h
    Created: 19 Apr 2022 10:54:12pm
    Author:  Dima

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Utilities.h"

//==============================================================================
/*
*/
class Synth  : public juce::Component
{
public:
    Synth(int& octaveShiftRef);
    ~Synth() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    virtual void contextControl(const juce::MidiMessageMetadata& metadata) = 0;

private:
    int& octaveShift;

    std::vector<IPlugin> synth;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Synth)
};
