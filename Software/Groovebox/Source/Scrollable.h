/*
  ==============================================================================

    Scrollable.h
    Created: 16 Mar 2022 6:52:05pm
    Author:  Peter Tso

  ==============================================================================
*/

#pragma once
#ifndef scrollable_h
#define scrollable_h
#include <iostream>
#include <JuceHeader.h>
namespace te = tracktion_engine;
class Scrollable : public juce::Component {
public:
    Scrollable();
    ~Scrollable() override;
    void paint(juce::Graphics &g) override;
    void resized() override;
    void setEdit(te::Edit *edit);
    void defineScrollByRect(juce::Rectangle<int> window);
    void scrollXAmount(int x);
    void scrollYAmount(int y);
    void setTrackHeight(int height);
private:
    const int horiztonalScrollBarHeight = 4;
    const int verticalScrollBarWidth = 4;
    int xOffset = 0;
    int yOffset = 0;
    int trackHeight = 64;
    juce::Colour cursorColor = juce::Colours::lightblue;
    te::Edit *edit;
    double getLongestTrackLength();
    int getNumberOfTracks();
};
#endif
