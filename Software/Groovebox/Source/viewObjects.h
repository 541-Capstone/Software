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

#ifdef __MACH__
    #define APATH "~/Documents/GitHub/Software/Software/Groovebox"
#endif

#ifdef _WIN64
    #define APATH "C:\\Users\\Dima\\Dev\\EECS542\\Software\\Software\\Groovebox"
#endif

#include <JuceHeader.h>
#include <vector>

// For now the viewObjects only includes ImageButtons and labels

/**
 * @brief viewObjects contains multiple objects to draw
          should be passed to paint() in MainComponent.
          Can be expanded on (and should be) to add more components without breaking compatatbility with older code.
 */
struct viewObjects {
    bool inclBtns;
    bool inclLbls;
    std::vector<juce::Button*> btns;
    std::vector<juce::Label*> lbls;
};

#endif
