#pragma once

#include <JuceHeader.h>
#include "gbx_comp/test.hpp"
#include <functional>
//#include "Utilities.h"

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
    
    /* This contains the states needed to switch between track view, etc. */
    enum WindowStates {
        TrackView,
        Settings
    };
    
    /* This is the state of the views */
    WindowStates WState = WindowStates::TrackView;
    
    /* This contains the states needed to play and pause */
    enum PlayStates {
        Play,
        Pause,
        Record
    };
    
    /* This is the state of the playing */
    PlayStates PState = PlayStates::Pause;
    
    /* These are the play and pause buttons */
    juce::ImageButton play;
    juce::ImageButton pause;
    juce::ImageButton record;
    
    /* Create the images */
    juce::Image playImage {juce::Image::RGB, 500, 500, true};
    juce::Image pauseImage {juce::Image::RGB, 500, 500, true};
    juce::Image recordImage {juce::Image::RGB, 500, 500, true};
    
    juce::Image playHoverImage {juce::Image::RGB, 500, 500, true};
    juce::Image pauseHoverImage {juce::Image::RGB, 500, 500, true};
    juce::Image recordHoverImage {juce::Image::RGB, 500, 500, true};
    
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
    
    //-------------------------------------------------------------
    
    /* Graphical properties below! */
    
    // This sets the background color
    juce::Colour background_color = juce::Colours::grey;
    
    // This sets the screen size
    const int window[2] = {800, 600};
    
    //-------------------------------------------------------------
    
    /* State functions below! */
    
    /**
     * @brief The setupTrackView function sets up the track view buttons
     * @param fst True when it is the first time setting up track view
     */
    void setupTrackView(bool fst);
    
    void setNumberofTracks();
    
    void load();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
