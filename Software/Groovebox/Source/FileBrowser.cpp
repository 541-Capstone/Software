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
    dir = juce::File::getSpecialLocation(juce::File::userDesktopDirectory);
    directoryList.setDirectory(dir, true, true);
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
    numItems = directoryList.getNumFiles();
    if (newItemSelection < 0 || newItemSelection >= numItems) {
        DBG("Invalid index!\n");
        return;
    }
    juce::File file = directoryList.getFile(newItemSelection);
    currentFile = file;
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
    dir = directory;
    updateFileBrowser();
    setToFirstIndexOnUpdate();
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
    currentFile = fileTreeComp.getSelectedFile();
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
    std::cout<<"Filebrowser item index: "<<itemSelection<<'\n';
    return itemSelection;
}

int FileBrowser::getItemHeight()const{
    return fileTreeComp.getItemHeight();
}

juce::File FileBrowser::getFileAtIndex(){
    return currentFile;
}

void FileBrowser::updateFileBrowser(){
    /* lousy way of updating, but
       it works for now */
    directoryList.setDirectory(juce::File::getSpecialLocation(juce::File::userDesktopDirectory), true, true);
    directoryList.setDirectory(dir, true, true);
}

void FileBrowser::setToFirstIndexOnUpdate(){
    if (directoryList.getNumFiles() == 0) {
        DBG("setToFirstIndexOnUpdate: 0 files\n");
    }
    juce::File file = directoryList.getFile(0);
    DBG("setToFirstIndexOnUpdate success\n");
    fileTreeComp.setSelectedFile(file);
    DBG("Current File: "+file.getFullPathName().toStdString()+"\n");
    currentFile = file;
}
