#pragma once

#include <stdio.h>
#include <JuceHeader.h>
#include <functional>
#include <math.h>

#include "graphicsHelper.h"

//#include "Utilities.h"

#define DB 1
#if DB
#define LOG(x) std::cout<<x
#else
#define LOG(x)
#endif


/* The universal state should reside in the MainComponent.
 All other states and/or windows (such as plugins, etc...) should
 reside within their own source code/header file
*/

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::Button::Listener, public juce::AudioAppComponent, public juce::Timer
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;
    //==============================================================================
    void paint (juce::Graphics& g) override;

    
    void buttonClicked(juce::Button* button) override;
    
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    
    void timerCallback() override;
    
    
    

private:
    //==============================================================================
    
    //-------------------------------------------------------------
    
    /* Graphical properties below! */
    
    // This sets the background color
    juce::Colour background_color = juce::Colours::white;
    
    // Default color
    juce::Colour defaultColor = juce::Colours::violet;
    
    // This sets the screen size
    const int scale = 2;
    const int window[2] = {1600 / scale, 900 / scale};
    
    const int windowCenter = window[0] / 2;
    
    //-------------------------------------------------------------
    
    int frameInterval = 16; // 16 millisecond per frame time
    double timeCount     = 0.0f;  // counts the time in seconds
    
    /* This contains the states needed to switch between timeline, mixer, etc. */
    enum WindowStates {
        Timeline,
        Mixer,
        Generator,
        Effects,
        Settings
    };
    
    /* This is the state of the views */
    WindowStates WState = WindowStates::Timeline;
    
    /* This contains the states needed to play and pause */
    enum PlayStates {
        Play,
        Pause,
        Record
    };
    
    /* This is the state of the playing */
    PlayStates PState = PlayStates::Pause;
    
    //=================================================================
    /**
     * The variables below are for timeline view
     */
    /* These are the play and pause buttons */
    juce::ImageButton play;
    juce::ImageButton pause;
    juce::ImageButton record;
    
    const int controlImageHeightpx = 150 / scale;
    const int controlImageWidthpx = 300 / scale;
    
    /**
     * TODO: Change the absolute path when implementing on Pi4
     */
    
    /* absolute path to code, change absolute path when needed */
    const std::string apath = APATH;
    
    juce::Label timelineLoadedLabel {"No edit loaded!"};
    
    //=================================================================
    
    
    /**
     * @brief This is the tracktion engine object.
     */
    tracktion_engine::Engine engine{
      ProjectInfo::projectName
    };
    
    /**
     * @brief This is the edit pointer. This will be used to load/used to edit
     */
    std::unique_ptr<tracktion_engine::Edit> edit;
    
    /**
     * @brief This is the file location of edits. Just concat the location to load.
     */
    const std::string editPath = apath + "/edits/";
    
    /**
     * @brief This is the total number of tracks available. The total number of tracks
     is the total number of audio tracks and the total number of MIDI tracks.
     */
    int numTracks;
    
    /**
     * @brief This is the number of audio tracks present.
     */
    int numAudioTracks;
    
    /**
     * @brief This is the number of MIDI tracks.
     */
    int numMIDItracks;
    
    /**
     * @brief This is the maximum number of tracks (-1 for no limit)
     */
    int maxNumTracks;
    
    bool isplaying;
    
    /* State functions below! */
    
    /**
     * @brief The setupTrackView function sets up the track view buttons
     * @param fst True when it is the first time setting up track view
     */
    void setupTrackView(bool fst);
    
    void setNumberofTracks(int n);
    
    /**
     * @brief This disables and hides all buttons
     */
    void disableAllButtons();
    
    /**
     * @brief Enables buttons based on state
     */
    void enableButtonAtCurrentState();
    
    /**
     * TODO: Create a button/method for allowing user to call fload
     * Fload loads an edit from the filename. If the file doesn't exist, then it creates a empty edit
     */
    
    // Function to load the edit
    void fload();
    
    // temporary button for loading (can get rid later for phy buttons !)
    juce::ImageButton load;
    
    /* core features should be added in MainComponent for all state objects to use. If a button or other componnent in the state object needs to use a core feature, call viewObjects* the_state_t::getObjects (), and pass function there */
    // Common functions for playing, recording, pausing, etc.
    void fplay();
    
    void fpause();
    
    void frecord();
    
    // objects for states
    
    /* this is the timeline object */
    Timeline_t tO {window[0], window[1], scale};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
