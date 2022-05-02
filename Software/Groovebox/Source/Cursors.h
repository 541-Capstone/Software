/*
  ==============================================================================

    Cursors.h
    Created: 14 Mar 2022 4:12:44pm
    Author:  Peter Tso

  ==============================================================================
*/

#pragma once
#ifndef cursors_h
#define cursors_h
#include<iostream>
#include<JuceHeader.h>
namespace te = tracktion_engine;
class Cursors : public juce::Component {
public:
    Cursors();
    ~Cursors() override;
    void paint(juce::Graphics &g) override;
    void resized() override;
    void setEdit(te::Edit *edit);
    void defineCursorByRect(juce::Rectangle<int> window);
private:
    const int cursor_width = 1;
    const double timeScale = 4.0f;
    juce::Colour cursor_color = juce::Colours::white;
    te::Edit *edit;
};
#endif
