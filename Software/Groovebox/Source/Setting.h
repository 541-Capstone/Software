/*
  ==============================================================================

    Setting.h
    Created: 11 Apr 2022 1:31:29pm
    Author:  Peter Tso

  ==============================================================================
*/

#pragma once
#ifndef setting_h
#define setting_h
#include <iostream>
#include <JuceHeader.h>
#include "ContextComponent.h"
namespace te = tracktion_engine;
class Setting : public juce::Component, public ContextComponent {
public:
    Setting();
    ~Setting() override;
    void paint(juce::Graphics &g) override;
    void resized() override;
    void setEdit (te::Edit *edit);
    void contextControl(const juce::MidiMessageMetadata &metadata) override;
    void displaySplashScreen();
private:
    te::Edit *edit;
};
#endif
