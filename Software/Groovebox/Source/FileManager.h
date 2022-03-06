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

#define TESTAUDIOPATH "~/Documents/test.m4a"

class FileManager {
public:
    FileManager ();
    
    ~FileManager ();
    
    void loadAudioAsClip (juce::String filename, tracktion_engine::Track *track);
    
    tracktion_engine::Edit* getEdit();
    
    void setEdit (tracktion_engine::Edit *edit);

private:
    std::unique_ptr<tracktion_engine::Edit> edit;
    
};

#endif
