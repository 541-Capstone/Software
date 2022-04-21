/*
  ==============================================================================

    FileBrowserHandler.h
    Created: 20 Apr 2022 1:48:37pm
    Author:  Peter Tso

  ==============================================================================
*/

#pragma once
#include <iostream>
#include <functional>
#include <JuceHeader.h>
#include "FileBrowser.h"
#include "ContextComponent.h"
namespace te = tracktion_engine;
template <typename T>
class FileBrowserHandler : public juce::Component, public juce::ActionListener, public ContextComponent {
public:
    FileBrowserHandler();
    ~FileBrowserHandler() override;
    void paint(juce::Graphics &g) override;
    T *setAction(std::function<T()> action);
    void setAllComponent(bool state);
    void setDirectory(const juce::File &directory);
    void actionListenerCallback(const juce::String &message) override;
    void contextControl(const juce::MidiMessageMetadata &metadata) override;
private:
    std::function<T()> action;
    FileBrowser fileBrowser;
    
    const int scrollAmount = 1;
    int cursorLocation     = 0;
    int cursorHeight       = 0;
    const int cursorWidth  = 10;
    juce::Rectangle<int> cursor;
    juce::Colour cursorColor = juce::Colours::red;
    
    void updateCursorLocation();
    
};
