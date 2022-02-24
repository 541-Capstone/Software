/*
  ==============================================================================

    graphicsHelper.h
    Created: 24 Feb 2022 12:10:19pm
    Author:  Peter Tso

  ==============================================================================
*/

#pragma once

#ifndef graphicsHelper_h
#define graphicsHelper_h

#include <iostream>
#include <JuceHeader.h>
#include <functional>

#include "viewObjects.h"
#include "Timeline.h"

class graphicsHelper {
public:
    graphicsHelper (juce::Graphics *g);
    
    ~graphicsHelper ();
    
    void draw ();
    
    void drawWaveform (std::unique_ptr<tracktion_engine::Edit> &edit);
    
private:
    juce::Graphics *g = nullptr;
};

#endif
