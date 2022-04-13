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
