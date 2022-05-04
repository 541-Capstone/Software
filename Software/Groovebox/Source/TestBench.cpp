/*
  ==============================================================================

    TestBench.cpp
    Created: 4 May 2022 12:36:13am
    Author:  Peter Tso

  ==============================================================================
*/

#include "TestBench.h"

TestBench::TestBench(){}

TestBench::~TestBench(){}

void TestBench::runtests(){
    juce::String editpath = juce::String(APATH)+juce::String(EDITPATH);
    juce::String audiopath = AUDIO_FILES_APATH;
    juce::String tempfilename = editpath+Helpers::getFormattedDate();
    juce::String testwavfilename = audiopath+TESTFILENAME;
    const juce::File tempfile(tempfilename);
    tempfile.create();
    
    DBG("Running Test Bench.\n");
    
    /* Create an empty edit */
    edit = te::createEmptyEdit(engine, tempfile);
    trackManager = std::make_unique<TrackManager>(edit);
    assert(edit != nullptr);
    DBG("00-Empty edit created.\n");
    
    /* populate with a single track */
    trackManager->createTrack();
    assert(te::getAudioTracks(*edit).size() == 2);
    DBG("01-Added a track.\n");
    
    /* switch active tracks */
    auto trr = trackManager->getActiveTrack()->getTrack();
    trackManager->setActiveTrack(1);
    assert(trr != trackManager->getActiveTrack()->getTrack());
    trr = trackManager->getActiveTrack()->getTrack();
    trackManager->setActiveTrack(0);
    assert(trr != trackManager->getActiveTrack()->getTrack());
    DBG("02-Tested switching tracks.\n");
    
    
    /* add a clip to track */
    Helpers::insertClipToTrack(trackManager->getActiveTrack()->getTrack(), &edit->getTransport(), testwavfilename);
    juce::File testifFileexists(testwavfilename);
    if (testifFileexists.existsAsFile()){
        assert(trackManager->getActiveTrack()->getTrack()->getClips().size() != 0);
    }
    else {
        DBG("Warning: File does not exist.\n");
    }
    DBG("03-Added clips.\n");
    
    /* Save edit */
    juce::String savefilename = editpath+Helpers::getFormattedDate();
    saveEdit(savefilename);
    juce::File testfile0(savefilename);
    assert(testfile0.existsAsFile());
    DBG("04-Saved edit.\n");
    
    /* load empty edit */
    loadEdit("");
    trackManager = std::make_unique<TrackManager>(edit);
    
    /* load just saved edit */
    loadEdit(savefilename);
    trackManager = std::make_unique<TrackManager>(edit);
    assert(te::getAudioTracks(*edit).size() == 2);
    DBG("05-Loaded edit.\n");
    
    DBG("Finished all tests.\n");
}


