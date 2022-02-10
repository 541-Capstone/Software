/*
  ==============================================================================

    Timeline.cpp
    Created: 10 Feb 2022 12:18:14pm
    Author:  Peter Tso

  ==============================================================================
*/

#include "Timeline.h"

Timeline::Timeline(int x, int y) {
    window[0] = x;
    window[1] = y;
    
    juce::ImageButton play {"play"};
    myObjects.btns.push_back(play);
}

Timeline::~Timeline(){
    
}

