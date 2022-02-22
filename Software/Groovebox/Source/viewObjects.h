/*
  ==============================================================================

    viewObjects.h
    Created: 10 Feb 2022 12:10:58pm
    Author:  Peter Tso

  ==============================================================================
*/

#pragma once

#ifndef viewobjects_h
#define viewobjects_h

#include <JuceHeader.h>
#include <vector>

// For now the viewObjects only includes ImageButtons and labels

/**
 * @brief viewObjects contains multiple objects to draw
          should be passed to paint() in MainComponent
 */
struct viewObjects {
    bool inclBtns;
    bool inclLbls;
    std::vector<juce::ImageButton*> btns;
    std::vector<juce::Label*> lbls;
};

#endif
