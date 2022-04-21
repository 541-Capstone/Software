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
#include <functional>
#include "ContextComponent.h"
#include "Utilities.h"
#include "FileBrowser.h"

namespace te = tracktion_engine;
class Setting : public ContextComponent {
public:
    Setting();
    ~Setting() override;
    void paint(juce::Graphics &g) override;
    void resized() override;
    void setEdit (te::Edit *edit);
    void contextControl(const juce::MidiMessageMetadata &metadata) override;
    void displaySplashScreen();
    void setAllComponents(bool state);
    void setStartFunction(std::function<void()> func);
    void toggleFirstStartToFalse();
    void setLoadEditFunction(std::function<void(std::string)> func);
    void setSaveEditFunction(std::function<void(std::string)> func);
    
private:
    te::Edit *edit;
    bool firstStart = true;
    std::function<void()> onStartUpFunction;
    
    // File components/functions
    std::function<void(std::string)> loadFromFileLambda;
    std::function<void(std::string)> saveToFileLambda;
    void loadEditFromFile();
    void saveEditToFile();
    std::string filename = "";
    FileBrowser browser;
    const int numItemsOnScreen = 10;
    
    // button sizes
    juce::TextButton loadEdit {"load edit from file"};
    juce::TextButton loadWav  {"load .wav from file"};
    juce::TextButton start {"Start!"};
    const int bsize = 100;
    const int half  = bsize/2;
    
    //Curosr Variables and Functions
    const int scrollAmount = 1;
    int cursorLocation = 0;
    int cursorHeight = 0;
    const int cursorWidth  = 10;
    juce::Rectangle<int> cursor;
    juce::Colour cursorColor = juce::Colours::red;
    void updateCursorLocation();
};
#endif
