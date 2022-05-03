/*
  ==============================================================================

    FileBrowser.h
    Created: 14 Apr 2022 11:14:21am
    Author:  Peter Tso

  ==============================================================================
*/

#pragma once
#ifndef filebrowser_h
#define filebrowser_h
#include <iostream>
#include <JuceHeader.h>
#include "Utilities.h"

class FileBrowser : public juce::Component, public juce::ActionListener, private juce::FileBrowserListener, public juce::ActionBroadcaster {
public:
    FileBrowser();
    ~FileBrowser() override;
    void paint(juce::Graphics &g) override;
    void setEdit(te::Edit *edit);
    void actionListenerCallback (const juce::String &message) override;
    void scrollUp(const int amount);
    void scrollDown(const int amount);
    void showFiles();
    void setAllComponents(bool state);
    void startThread(const int threadID);
    void resized() override;
    void setDirectory(const juce::File &directory);
    int getItemIndex()const;
    int getItemHeight()const;
    juce::File getFileAtIndex();
    void updateFileBrowser();
private:
    void selectionChanged() override;
    void fileClicked(const juce::File &file, const juce::MouseEvent &event) override;
    void fileDoubleClicked (const juce::File &file) override;
    void browserRootChanged (const juce::File &newRoot) override;
    void getNumberofItems();
    void setToFirstIndexOnUpdate();
    te::Edit *edit;
    juce::TimeSliceThread thread {"file browser"};
    juce::DirectoryContentsList directoryList {nullptr, thread};
    juce::FileTreeComponent fileTreeComp {directoryList};
    int numItems = 0;
    int itemSelection = 0;
    juce::File currentFile, dir;
};
#endif