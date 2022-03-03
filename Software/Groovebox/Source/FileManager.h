/*
  ==============================================================================

    FileManager.h
    Created: 3 Mar 2022 11:21:08am
    Author:  Peter Tso

  ==============================================================================
*/

#pragma once
#ifndef filemanager_h
#define filemanager_h

#include <stdio.h>
#include <iostream>
#include <JuceHeader.h>
#include <functional>

class FileManager {
public:
    FileManager ();
    
    ~FileManager ();
    
    void loadAudioAsClip (juce::String filename, tracktion_engine::Track *track);
    
    tracktion_engine::Edit* getEdit();

private:
    std::unique_ptr<tracktion_engine::Edit> edit;
    
};

#endif
