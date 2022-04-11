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
}

Setting::~Setting(){
    
}

void Setting::paint(juce::Graphics &g){
    
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
    
}
