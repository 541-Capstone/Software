/*
  ==============================================================================

    Setting.cpp
    Created: 11 Apr 2022 1:31:29pm
    Author:  Peter Tso

  ==============================================================================
*/

#include "Setting.h"

Setting::Setting(){
    buttonSize = this->getBounds();
    //buttonSize.removeFromTop(this->getX()/3);
    //buttonSize.removeFromBottom(this->getX()/3);
    //buttonSize.removeFromLeft(this->getY()/3);
    //buttonSize.removeFromRight(this->getY()/3);
    
    edit = nullptr;
    /* set buttons */
    
    /* add and make visible all buttons
       will be removed later, but
       usefull for testing */
    addAndMakeVisible(loadEdit);
    addAndMakeVisible(loadWav);
    
    start.setBounds(this->getWidth()/2-50, this->getHeight()/2-50, 100, 100);
    addAndMakeVisible(start);
    
    /* hide all buttons */
    start.setBounds(this->getWidth()/2-50, this->getHeight()/2-50, 100, 100);
    setAllComponents(false);
}

Setting::~Setting(){
    
}

void Setting::paint(juce::Graphics &g){
    if (!firstStart) {
        
    }
}

void Setting::resized(){
    
}

void Setting::setEdit(te::Edit *edit) {
    this->edit = edit;
}

/* This is where the menu is called from */
void Setting::contextControl(const juce::MidiMessageMetadata &metadata) {
    const juce::MidiMessage message = metadata.getMessage();
    Helpers::ContextualCommands cmd = Helpers::getContextualCmdType(message);
    
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
                break;
            case Helpers::Encoders::CCW1:
                /* rotate counter clockwise */
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
}

void Setting::setStartFunction(std::function<void ()> func) {
    onStartUpFunction = func;
}

void Setting::toggleFirstStartToFalse(){
    firstStart = false;
}

void Setting::loadEditFromFile(){
    /* check if valid file name (not "") */
    if (filename == "" || filename == " ") {
        std::cout<<"Filename not set!\n";
        return;
    }
    
    /* now, check if file exists */
    
    
    /* load into edit with function defined in MainComponent */
    loadFromFileLambda();
    
}

void Setting::saveEditToFile(){
    
    
    /* save edit with function defined in MainComponent */
    saveToFileLambda();
}

void Setting::setLoadEditFunction(std::function<void ()> func){
    loadFromFileLambda = func;
}

void Setting::setSaveEditFunction(std::function<void ()> func){
    saveToFileLambda = func;
}
