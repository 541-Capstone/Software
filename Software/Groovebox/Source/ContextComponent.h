/*
  ==============================================================================

    ContextComponent.h
    Created: 7 Apr 2022 12:35:45pm
    Author:  Peter Tso

  ==============================================================================
*/

#pragma once
#ifndef contextcomponent_h
#define contextcomponent_h
#include <JuceHeader.h>
class ContextComponent {
public:
    virtual ~ContextComponent() = default;
    virtual void contextControl(const juce::MidiMessageMetadata &metadata) = 0;
private:
};
#endif
