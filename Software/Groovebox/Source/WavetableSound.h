/*
  ==============================================================================

    WavetableSound.h
    Created: 4 Apr 2022 7:57:59pm
    Author:  Dima

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class WavetableSound : public juce::SynthesiserSound {
public:
    bool appliesToNote(int midiNoteNumber) override { return true; }
    bool appliesToChannel(int channelNumber)  override { return true; }
private:

};