#pragma once

#include <stdio.h>
#include <JuceHeader.h>
#include <functional>
#include <math.h>

// Groovebox files
#include "Utilities.h"
#include "Timeline.h"
#include "TrackManager.h"
#include "FileManager.h"
#include "AudioVisualizer.h"
#include "WaveformManager.h"
#include "Waveforms.h"
#include "Cursors.h"
#include "MidiService.h"
#include "Setting.h"
#include "Wavetable.h"
#include "Delay.h"
#include "Synth.h"
#include "Effects.h"
/* The universal state should reside in the MainComponent.
 All other states and/or windows (such as plugins, etc...) should
 reside within their own source code/header file
*/

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::Button::Listener, 
    public juce::AudioAppComponent, 
    public juce::MultiTimer,
    public juce::ActionListener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;
    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    
    void buttonClicked(juce::Button* button) override;
    
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    
    void timerCallback(int id) override;

private:
    //==============================================================================
    
    //-------------------------------------------------------------
    
    /* Graphical properties below! */
    
    // This sets the background color
    juce::Colour background_color = juce::Colour(40,40,40);
    
    // Default color
    juce::Colour defaultColor = juce::Colours::violet;
    
    // This sets the screen size
    const int scale = 2;
    const int window[2] = {1600 / scale, 900 / scale};
    
    //-------------------------------------------------------------
    
    int frameInterval = 16; // 16 millisecond per frame time
    double timeCount  = 0.0f;  // counts the time in seconds

    int sampleRate; //Engine sample rate in Hz

    // Shared MIDI buffer for reading inputs
    std::shared_ptr<juce::MidiBuffer> inputMidiBuffer;
    int inputPrevSampleNumber; //Integer sample number of previously processed sample, used for clearing buffer
    // This manages MIDI inputs
    std::unique_ptr<MidiService> midiService;
    
    /* This contains the states needed to switch between timeline, mixer, etc. */
    enum class WindowStates {
        TrackView,
        Mixer,
        Synthesizer,
        Effects,
        Settings
    };
    
    /* This is the state of the views */
    WindowStates WState = WindowStates::TrackView;

    //Container for referencing all of our renderable contexts
    std::vector<std::unique_ptr<ContextComponent>> windows;
    //THe currently selected context
    int activeWindowIndex = 0;
    
    /* This contains the states needed to play and pause */
    enum class PlayStates {
        Play,
        Pause,
        Record,
    };
    
    enum class PlayStyle : int {
        Solo = 0,
        Mult
    };
    
    /* This is the state of the playing */
    PlayStates PState = PlayStates::Pause;
    PlayStyle  PStyle = PlayStyle::Mult;
    
    //=================================================================
    /**
     * The variables below are for timeline view
     */
    /* These are the play and pause buttons */
    juce::ImageButton playBtn;
    juce::ImageButton pauseBtn;
    juce::ImageButton recordBtn;
    
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
    std::shared_ptr<te::Edit> edit;
    
    /**
     * @brief This is the file location of edits. Just concat the location to load.
     */
    const std::string editPath = apath + "/edits/";
    
    /**
     * @brief This is the vector for storing audio track pointers. Should be handed over to states
     */
    std::shared_ptr<TrackManager> trackManager;
    
    /**
     * @brief This is the maximum number of tracks (-1 for no limit)
     */
    int maxNumTracks;

    bool isPlaying;
    
    /* these are the state objects */
    Timeline timeline;
    Setting setting;
    Synth synthWindow;
    Effects effects;

    FileManager fileManager;
    tracktion_engine::SelectionManager selectionmanager {engine};
    
    /* State functions below! */
    
    /**
     * @brief The setupTrackView function sets up the track view buttons
     */
    void setupTrackView();
    
    void setMaxTracks(int n);
    
    /**
     * @brief This disables and hides all buttons
     */
    void disableAllButtons();
    
    void disableAllStates();
    
    /**
     * @brief Enables buttons based on state
     */
    void enableButtonAtCurrentState();

    //Possible race condition in here
    void actionListenerCallback(const juce::String& message) override;
    
    /**
     * TODO: Create a button/method for allowing user to call loadEdit
     * loadEdit loads an edit from the filename. If the file doesn't exist, then it creates a empty edit
     */
    
    // Function to load the edit
    void loadEdit(std::string filename);
    void saveEdit(std::string filename);
    // temporary button for loading (can get rid later for phy buttons !)
    juce::ImageButton load;
    
    /* core features should be added in MainComponent for all state objects to use. If a button or other componnent in the state object needs to use a core feature, call viewObjects* the_state_t::getObjects (), and pass function there */
    // Common functions for playing, recording, pausing, etc.
    
    /* testing purposes, remove and put in different file later.... Preferrably Timeline */
    //WaveformManager waveformManager;
    //Waveforms waveforms;
    //Cursors cursor;
    
    //Begins playing the track from the current playhead position
    void play();
    
    //Pauses the palyhead at its current position
    void pause();
    
    //Begins recording, and begins playing if not already playing
    void record();

    //Sets the current audio Track to the next in line if possible
    void nextTrack();

    //Sets the current audio Track to the previous in line if possible
    void prevTrack();

    //Creates a new audio track at the end of the list if possible
    void createAudioTrack();
    
    // adds a clip to an existing audio track
    void addClipToTrack();
    
    // Solo, play just the active track
    void solo();

    //Mute the current track
    void mute();
    
    /* do something with universal controls */
    void universalControls(const juce::MidiMessageMetadata &metadata);
    
    ContextComponent *currentComponent = nullptr;
    
    void setupSetting();
    void setupTimeline();

    void setupExample();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
