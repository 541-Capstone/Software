/*
  ==============================================================================

    Setting.cpp
    Created: 11 Apr 2022 1:31:29pm
    Author:  Peter Tso

  ==============================================================================
*/

#include "Setting.h"

Setting::Setting(){
    edit = nullptr;
    /* set buttons */
    
    /* add and make visible all buttons
       will be removed later, but
       usefull for testing */
    addAndMakeVisible(loadEdit);
    addAndMakeVisible(loadWav);
    
    /* set size of buttons */
    start.setBounds(this->getWidth()/2-half, this->getHeight()/2-half, bsize, bsize);
    addAndMakeVisible(start);
    
    
    std::function<void()> testFunctionScrollUp = [&]()->void{
        browser.scrollUp(scrollAmount);
    };
    
    std::function<void()> testFunctionScrollDown = [&]()->void{
        browser.scrollDown(scrollAmount);
    };
    
    loadEdit.onClick = testFunctionScrollUp;
    loadWav.onClick = testFunctionScrollDown;
    
    /* hide all buttons */
    setAllComponents(false);
    
    addAndMakeVisible(browser);
    browser.startThread(3);
}

Setting::~Setting(){
    
}

void Setting::paint(juce::Graphics &g){
    if (!firstStart) {
        
    }
}

void Setting::resized(){
    browser.setBounds(0, 0, this->getWidth(), this->getHeight()/2);
    
    loadEdit.setBounds(this->getWidth()/2-bsize, this->getHeight()-bsize, bsize, bsize);
    
    loadWav.setBounds(this->getWidth()/2, this->getHeight()-bsize, bsize, bsize);
}

void Setting::setEdit(te::Edit *edit) {
    this->edit = edit;
    browser.setEdit(edit);
}

/* This is where the menu is called from */
void Setting::contextControl(const juce::MidiMessageMetadata &metadata) {
    const juce::MidiMessage message = metadata.getMessage();
    Helpers::ContextualCommands cmd = Helpers::getContextualCmdType(message);
    
    /* as a check, DO NOT DO ANYTHING if browser
       component is set to not enabled */
    if (!browser.isEnabled()) {
        std::cout<<"This is an error. This should not happen.";
        std::cout<<" From Setting context!\n";
        return;
    }
    
    /* do stuff with encoders */
    if (cmd == Helpers::ContextualCommands::Encoder) {
        Helpers::Encoders enc = Helpers::getEncoderType(message);
        if (enc == Helpers::Encoders::NOTASSIGNED) return;
        
        /*
         * This will be used to increment
         * through file system.
         */
        switch (enc) {
            case Helpers::Encoders::CW1:
                /* rotate clockwise */
                browser.scrollUp(scrollAmount);
                break;
            case Helpers::Encoders::CCW1:
                /* rotate counter clockwise */
                browser.scrollDown(scrollAmount);
                break;
            default:
                /* do nothing */
                break;
        }
        
    }
    /* else it is other than encoder */
    else {
        switch (cmd) {
            case Helpers::ContextualCommands::Load:
                /* load */
                loadEditFromFile();
                break;
            case Helpers::ContextualCommands::Save:
                /* save */
                saveEditToFile();
                break;
            default:
                /* do nothing */
                break;
        }
    }
}

void Setting::displaySplashScreen(){
    addAndMakeVisible(start);
    start.setEnabled(true);
    start.setVisible(true);
    start.setBounds(this->getWidth()/2-50, this->getHeight()/2-50, 100, 100);
    start.onClick = onStartUpFunction;
}

void Setting::setAllComponents(bool state){
    loadEdit.setEnabled(state);
    loadEdit.setVisible(state);
    loadWav.setEnabled(state);
    loadWav.setVisible(state);
    browser.setAllComponents(state);
    browser.setEnabled(state);
    browser.setVisible(state);
}

void Setting::setStartFunction(std::function<void ()> func) {
    onStartUpFunction = func;
}

void Setting::toggleFirstStartToFalse(){
    firstStart = false;
    start.setEnabled(false);
    start.setVisible(false);
}

void Setting::loadEditFromFile(){
    /* check if valid file name (not "") */
    if (filename == "" || filename == " ") {
        std::cout<<"Filename not set!\n";
        return;
    }
    
    /* now, check if file exists */
    
    
    /* load into edit with function defined in MainComponent */
    loadFromFileLambda(filename);
    
}

void Setting::saveEditToFile(){
    
    
    /* save edit with function defined in MainComponent */
    saveToFileLambda(filename);
}

void Setting::setLoadEditFunction(std::function<void (std::string)> func){
    loadFromFileLambda = func;
}

void Setting::setSaveEditFunction(std::function<void (std::string)> func){
    saveToFileLambda = func;
}
