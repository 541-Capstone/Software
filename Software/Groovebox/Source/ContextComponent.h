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

/*
 *  Add this virtual class to any contexts so
 *  we can utilize the currentComponent pointer
 *  in MainComponent to utilize context controls!
 *
 */
class ContextComponent {
public:
    virtual ~ContextComponent() = default;
    virtual void contextControl(const juce::MidiMessageMetadata &metadata) = 0;
private:
};
#endif
