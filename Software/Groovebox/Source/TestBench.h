/*
  ==============================================================================

    TestBench.h
    Created: 4 May 2022 12:36:13am
    Author:  Peter Tso

  ==============================================================================
*/

#pragma once
#include <iostream>
#include <JuceHeader.h>
#include <functional>
#include "Utilities.h"
#include "TrackManager.h"

#define TESTFILENAME "test.wav"

namespace te = tracktion_engine;
class TestBench {
public:
    TestBench();
    ~TestBench();
    void runtests();
private:
    /* Load and save functions
       copied from MainComponent.cpp */
    void loadEdit(juce::String filename){
        juce::File editFile(filename);
        auto& dm = engine.getDeviceManager().deviceManager;

        auto deviceManagerSetup = dm.getAudioDeviceSetup();
        deviceManagerSetup.sampleRate = 44100;
        dm.setAudioDeviceSetup(deviceManagerSetup, true);
        //dm.initialise(2, 2, 0, true, juce::String(), &deviceManagerSetup);
        LOG((juce::String)engine.getDeviceManager().getSampleRate());
        
        // get the edit if it exists
        if (editFile.existsAsFile()) {
            DBG("load edit called");
            edit = std::move(te::loadEditFromFile(engine, editFile));
        }
        // else create a new, empty edit if it doesn't exist
        else{
            edit = std::move(te::createEmptyEdit(engine, editFile));
        }
    };
    void saveEdit(juce::String filename){
        /* make sure that there is a filename present */
        if (filename.length() == 0) {
            std::cout<<"no filename!\n";
            return;
        }
        juce::File savefile(filename);
        auto res = savefile.create();
        
        // Write to file
        std::cout<<"Save to "<<savefile.getFullPathName()<<'\n';
        te::Edit *e = edit.get();
        te::EditFileOperations edo(*e);
        edo.writeToFile(savefile, false);
    };
    te::Engine engine{"Test Bench"};
    std::shared_ptr<te::Edit> edit;
    std::shared_ptr<TrackManager> trackManager;
};
