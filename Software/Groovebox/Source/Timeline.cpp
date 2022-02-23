/*
  ==============================================================================

    Timeline.cpp
    Created: 10 Feb 2022 12:18:14pm
    Author:  Peter Tso

  ==============================================================================
*/

#include "Timeline.h"

Timeline_t::Timeline_t(int x, int y, int sc) {
    window[0] = x;
    window[1] = y;
    windowCenter = window[0] / 2;
    
    scale = sc;
    controlImageHeightpx = controlImageHeightpx / scale;
    controlImageWidthpx = controlImageWidthpx / scale;
    
    setupButtonImages ();
    
    myObjects.inclBtns = true;
    myObjects.inclLbls = false;
    
    myObjects.btns.push_back(&play);
    myObjects.btns.push_back(&pause);
    myObjects.btns.push_back(&record);
}

Timeline_t::~Timeline_t(){
    // do nothing for now!
}

viewObjects* Timeline_t::getObjects() {
    return &myObjects;
}

bool Timeline_t::assignFunctionToObjects(std::initializer_list<std::function<void()>> list) {
    for (auto elem : list) {
        funcs.push_back(elem);
    }
    return true;
}

void Timeline_t::onClick (juce::Button *btn) {
    int i = 0;
    for (auto b : myObjects.btns) {
        if (b == btn) { funcs[i](); return; }
        i++;
    }
}

std::function<void(juce::Graphics*)> Timeline_t::drawState () {
    std::function<void(juce::Graphics*)> ffsetupState = [&](juce::Graphics *g)->void {
        g->setColour(bg_color);
        g->fillRect (0, 0, window[0], 40 + controlImageHeightpx);
    };
    
    return ffsetupState;
}

void Timeline_t::setupButtonImages() {
    juce::Colour zeroAlpha = juce::Colour::fromRGBA(0x00, 0x00, 0x00, 0x00);
    juce::Colour darken = juce::Colour::fromRGBA(0xFF, 0x00, 0x00, 128);
    
    const int y_spacing = 20;
    const int x_spacing = 20;
    const int leftmost = windowCenter - (1.5 * controlImageWidthpx) - x_spacing;
    
    play.setImages(true,
                   true,
                   true,
                   playImage,
                   100,
                   zeroAlpha,
                   playHoverImage,
                   100,
                   darken,
                   playImage,
                   100,
                   zeroAlpha);

    play.setBounds(leftmost, y_spacing, controlImageWidthpx, controlImageHeightpx);
    
    record.setImages(true,
                    true,
                    true,
                    recordImage,
                    100,
                    zeroAlpha,
                    recordHoverImage,
                    100,
                    zeroAlpha,
                    recordImage,
                    100,
                    zeroAlpha);
    
    record.setBounds(leftmost + x_spacing + controlImageWidthpx, y_spacing, controlImageWidthpx, controlImageHeightpx);
    
    pause.setImages(true,
                    true,
                    true,
                    pauseImage,
                    100,
                    zeroAlpha,
                    pauseHoverImage,
                    100,
                    zeroAlpha,
                    pauseImage,
                    100,
                    zeroAlpha);
    
    pause.setBounds(leftmost + (2 * x_spacing) + (2 * controlImageWidthpx), y_spacing, controlImageWidthpx, controlImageHeightpx);
    
}
