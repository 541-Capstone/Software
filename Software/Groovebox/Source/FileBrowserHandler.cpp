/*
  ==============================================================================

    FileBrowserHandler.cpp
    Created: 20 Apr 2022 1:48:37pm
    Author:  Peter Tso

  ==============================================================================
*/
/*
#include "FileBrowserHandler.h"

template class FileBrowserHandler<te::Edit>;
template class FileBrowserHandler<juce::File>;
template class FileBrowserHandler<te::Clip>;

template <class T>
FileBrowserHandler<T>::FileBrowserHandler(){
    addAndMakeVisible(fileBrowser);
    fileBrowser.startThread(4);
    fileBrowser.addActionListener(this);
}

template <class T>
FileBrowserHandler<T>::~FileBrowserHandler(){
    
}

template <class T>
void FileBrowserHandler<T>::paint(juce::Graphics &g){
    g.setColour(cursorColor);
    g.fillRect(cursor);
}

template <class T>
void FileBrowserHandler<T>::resized(){
    const int browserHeight = numItemsOnScreen * fileBrowser.getItemHeight();
    fileBrowser.setBounds(cursorWidth, 0, this->getWidth()-cursorWidth, browserHeight);
}

template<class T>
void FileBrowserHandler<T>::setAction(std::function<T()> action){
    this->action = action;
}

template <class T>
void FileBrowserHandler<T>::setAllComponent(bool state){
    fileBrowser.setEnabled(state);
    fileBrowser.setVisible(state);
    fileBrowser.setAllComponents(state);
}

template <class T>
void FileBrowserHandler<T>::setDirectory(const juce::File &directory){
    fileBrowser.setDirectory(directory);
}

template <class T>
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

template <class T>
void FileBrowserHandler<T>::updateCursorLocation(){
    int maxHeight  = fileBrowser.getHeight();
    cursorHeight   = fileBrowser.getItemHeight();
    
    int newYCoord = cursorHeight * cursorLocation;
    if (newYCoord < maxHeight) {
        cursor.setBounds(0, cursorLocation * cursorHeight, cursorWidth*2, cursorHeight);
    }
}

template <class T>
T *FileBrowserHandler<T>::contextControl(const juce::MidiMessageMetadata &metadata){
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
}

template <class T>
void FileBrowserHandler<T>::scrollUp(){
    fileBrowser.scrollUp(scrollAmount);
}

template <class T>
void FileBrowserHandler<T>::scrollDown(){
    fileBrowser.scrollDown(scrollAmount);
}
*/
