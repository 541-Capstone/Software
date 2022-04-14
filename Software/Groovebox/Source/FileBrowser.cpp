/*
  ==============================================================================

    FileBrowser.cpp
    Created: 14 Apr 2022 11:14:21am
    Author:  Peter Tso

  ==============================================================================
*/

#include "FileBrowser.h"

FileBrowser::FileBrowser(){
    edit = nullptr;
    addAndMakeVisible(fileTreeComp);
    directoryList.setDirectory(juce::File::getSpecialLocation(juce::File::userHomeDirectory), true, true);
    fileTreeComp.setTitle("Files");
    fileTreeComp.setColour(juce::FileTreeComponent::backgroundColourId, juce::Colours::wheat);
    fileTreeComp.addListener(this);
}

FileBrowser::~FileBrowser(){
    edit = nullptr;
}

void FileBrowser::paint(juce::Graphics &g){
    
}

void FileBrowser::resized(){
    fileTreeComp.setBounds(this->getBounds());
}

void FileBrowser::setEdit(te::Edit *edit){
    this->edit = edit;
}

void FileBrowser::scrollUp(const int amount){
    
}

void FileBrowser::scrollDown(const int amount){
    
}

void FileBrowser::actionListenerCallback(const juce::String &message){
    
}

void FileBrowser::showFiles(){
    
}

void FileBrowser::setAllComponents(bool state){
    fileTreeComp.setEnabled(state);
    fileTreeComp.setVisible(state);
}

void FileBrowser::startThread(const int threadID){
    thread.startThread(threadID);
}

void FileBrowser::selectionChanged(){
    
}

void FileBrowser::fileClicked(const juce::File &file, const juce::MouseEvent &event){
    
}

void FileBrowser::fileDoubleClicked(const juce::File &file){
    
}

void FileBrowser::browserRootChanged(const juce::File &newRoot){
    
}
