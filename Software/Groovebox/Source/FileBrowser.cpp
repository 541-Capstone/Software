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
    directoryList.setDirectory(juce::File::getSpecialLocation(juce::File::userDesktopDirectory), true, true);
    fileTreeComp.setTitle("Files");
    fileTreeComp.setColour(juce::FileTreeComponent::backgroundColourId, juce::Colours::wheat);
    fileTreeComp.addListener(this);
    getNumberofItems();
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
    const int newItemSelection = amount + itemSelection;
    if (newItemSelection < 0 || newItemSelection >= numItems) return;
    juce::File file = directoryList.getFile(newItemSelection);
    fileTreeComp.setSelectedFile(file);
    auto treeItem = fileTreeComp.getItemOnRow(newItemSelection);
    fileTreeComp.scrollToKeepItemVisible(treeItem);
    std::cout<<"filename: "<<file.getFileName()<<'\n';
    std::cout<<"Index: "<<newItemSelection<<'\n';
    itemSelection = newItemSelection;
    getNumberofItems();
    std::string action = std::to_string(amount);
    sendActionMessage(action);
}

void FileBrowser::scrollDown(const int amount){
    scrollUp(-amount);
}

void FileBrowser::actionListenerCallback(const juce::String &message){
    
}

void FileBrowser::showFiles(){
    
}

void FileBrowser::setDirectory(const juce::File &directory){
    directoryList.setDirectory(directory, true, true);
}

void FileBrowser::setAllComponents(bool state){
    fileTreeComp.setEnabled(state);
    fileTreeComp.setVisible(state);
}

void FileBrowser::startThread(const int threadID){
    thread.startThread(threadID);
}

/* do something when selection has changed */
void FileBrowser::selectionChanged(){
    LOG("SelectionChanged!\n");
    getNumberofItems();
}

void FileBrowser::fileClicked(const juce::File &file, const juce::MouseEvent &event){
    
}

void FileBrowser::fileDoubleClicked(const juce::File &file){
    
}

void FileBrowser::browserRootChanged(const juce::File &newRoot){
    
}

void FileBrowser::getNumberofItems(){
    numItems = fileTreeComp.getNumRowsInTree();
    std::cout<<"\nnumber of items inside tree: "<<numItems<<'\n';
}

int FileBrowser::getItemIndex()const{
    return itemSelection;
}

int FileBrowser::getItemHeight()const{
    return fileTreeComp.getItemHeight();
}
