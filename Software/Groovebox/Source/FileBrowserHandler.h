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
#include "Utilities.h"

namespace te = tracktion_engine;

template <class T>
class FileBrowserHandler : public juce::Component, public juce::ActionListener {
public:
    FileBrowserHandler() {
        addAndMakeVisible(fileBrowser);
        fileBrowser.startThread(4);
        fileBrowser.addActionListener(this);
    };
    ~FileBrowserHandler() override {};
    void paint(juce::Graphics &g) override {
        g.setColour(cursorColor);
        g.fillRect(cursor);
    };
    void resized() override {
        const int browserHeight = numItemsOnScreen * fileBrowser.getItemHeight();
        fileBrowser.setBounds(cursorWidth, 0, this->getWidth()-cursorWidth, browserHeight);
    };
    void setAction(std::function<T*()> action) {
        this->action = action;
    };
    void setAllComponent(bool state) {
        fileBrowser.setEnabled(state);
        fileBrowser.setVisible(state);
        fileBrowser.setAllComponents(state);
    };
    void setDirectory(const juce::File &directory) {
        fileBrowser.setDirectory(directory);
    };
    void actionListenerCallback(const juce::String &message) override {
        int movedAmount = message.getIntValue();
        const int newCursorLocation = movedAmount + cursorLocation;
        if (newCursorLocation < 0 || newCursorLocation >= 10) {
            return;
        }
        else {
            cursorLocation = newCursorLocation;
            updateCursorLocation();
        }
    };
    T *contextControl(const juce::MidiMessageMetadata &metadata) {
        juce::MidiMessage message = metadata.getMessage();
        Helpers::ContextualCommands cmd = Helpers::getContextualCmdType(message);
        
        T *ret = nullptr;
        if (!fileBrowser.isEnabled()) {
            std::cout<<"File browser is not enabled!\n";
            return nullptr;
        }
        
        if (cmd == Helpers::ContextualCommands::Encoder) {
            Helpers::Encoders enc = Helpers::getEncoderType(message);
            if (enc == Helpers::Encoders::NOTASSIGNED) {
                return nullptr;
            }
            switch (enc) {
                case Helpers::Encoders::CW1:
                    fileBrowser.scrollUp(scrollAmount);
                    break;
                case Helpers::Encoders::CCW1:
                    fileBrowser.scrollDown(scrollAmount);
                    break;
                default:
                    
                    break;
            }
        }
        else {
            switch (cmd) {
                case Helpers::ContextualCommands::Add:
                    ret = action();
                    break;
                default:
                    break;
            }
        }
        return ret;
    };
    
    juce::File getFileAtIndex(){
        return fileBrowser.getFileAtIndex();
    };
    
    // Functions to be removed later
    void scrollUp() {
        fileBrowser.scrollUp(scrollAmount);
    };
    void scrollDown() {
        fileBrowser.scrollDown(scrollAmount);
    };
    
    T *doAction(){
        return action();
    };
    
    void updateFileBrowser(){
        fileBrowser.updateFileBrowser();
    };
    
private:
    std::function<T*()> action;
    FileBrowser fileBrowser;
    const int numItemsOnScreen = 10;
    
    const int scrollAmount = 1;
    int cursorLocation     = 0;
    int cursorHeight       = 0;
    const int cursorWidth  = 10;
    juce::Rectangle<int> cursor;
    juce::Colour cursorColor = juce::Colours::red;
    
    void updateCursorLocation() {
        int maxHeight  = fileBrowser.getHeight();
        cursorHeight   = fileBrowser.getItemHeight();
        
        int newYCoord = cursorHeight * cursorLocation;
        if (newYCoord < maxHeight) {
            cursor.setBounds(0, cursorLocation * cursorHeight, cursorWidth*2, cursorHeight);
        }
    };
    
};
