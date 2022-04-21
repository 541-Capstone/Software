/*
  ==============================================================================

    FileBrowserHandler.cpp
    Created: 20 Apr 2022 1:48:37pm
    Author:  Peter Tso

  ==============================================================================
*/

#include "FileBrowserHandler.h"

template <typename T>
FileBrowserHandler<T>::FileBrowserHandler(){
    fileBrowser.startThread(4);
}

template <typename T>
FileBrowserHandler<T>::~FileBrowserHandler(){
    
}

template <typename T>
void FileBrowserHandler<T>::paint(juce::Graphics &g){
    
}

template<typename T>
T *FileBrowserHandler<T>::setAction(std::function<T()> action){
    
}

template <typename T>
void FileBrowserHandler<T>::setAllComponent(bool state){
    fileBrowser.setEnabled(state);
    fileBrowser.setVisible(state);
    fileBrowser.setAllComponents(state);
}

template <typename T>
void FileBrowserHandler<T>::setDirectory(const juce::File &directory){
    fileBrowser.setDirectory(directory);
}

template <typename T>
void FileBrowserHandler<T>::actionListenerCallback(const juce::String &message){
    int movedAmount = message.getIntValue();
    const int newCursorLocation = movedAmount + cursorLocation;
    if (newCursorLocation < 0 || newCursorLocation >= 10) {
        return;
    }
    else {
        cursorLocation = newCursorLocation;
        updateCursorLocation();
    }
}

template <typename T>
void FileBrowserHandler<T>::updateCursorLocation(){
    int maxHeight  = fileBrowser.getHeight();
    cursorHeight   = fileBrowser.getItemHeight();
    
    int newYCoord = cursorHeight * cursorLocation;
    if (newYCoord < maxHeight) {
        cursor.setBounds(0, cursorLocation * cursorHeight, cursorWidth*2, cursorHeight);
    }
}

