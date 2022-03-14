/*
  ==============================================================================

    Timeline.h
    Created: 10 Feb 2022 12:18:22pm
    Author:  Peter Tso

  ==============================================================================
*/

#pragma once
#ifndef timeline_h
#define timeline_h

#include <iostream>
#include <functional>
#include <JuceHeader.h>
#include <cstdarg>
#include "viewObjects.h"
#include "TrackManager.h"
#include "FileManager.h"
#include "Utilities.h"
#include "Waveforms.h"
#include "Cursors.h"

class Timeline_t : juce::ActionBroadcaster, public juce::Component {
public:
    
    /**
     * @brief This is the constructor for Timeline_t
     *
     * @param x This is the number of pixels (width)
     * @param y This is the number of pixels (height)
     * @param sc This is the scale (usually 1)
     * @param numTracks pointer to the number of tracks currently in the session
     * @param currentTrack Pointer to the index of the current track
     *
     */
    Timeline_t(int x, int y, int sc, int *numTracks);
    
    /**
     * @brief This is the Timeline_t class destructor
     *
     */
    ~Timeline_t();
    
    /**
     * @brief This returns all objects in Timeline_t
     *
     */
    viewObjects* getObjects();
    
    /**
     * @brief executes the function at that button if possible
     *
     * @param *btn This is the button to click
     *
     */
    void onClick (juce::Button *btn);
    
    /**
     * @pre Must have same number of functions as buttons
     * @brief This varadic function assigns lambda functions to objects in Timeline_t
     *
     * @param list This is a initalizer list of functions.
     *
     */
    bool assignFunctionToObjects (std::initializer_list<std::function<void()>> list);
    
    /**
     * @brief This function assigns a single function to a certain button
     *
     * @param *btn This is the button pointer
     * @param func This is the function to assign
     * @return true Returns true when successful
     * @return false Returns false when not successful
     *
     */
    bool assignFuncToBtn (juce::Button *btn, std::function<void()> func);
    
    void paint(juce::Graphics &g) override;

    /**
     * @brief This returns drawing routine for Timeline_t
     *
     * @return std::function<void(juce::Graphics *g)> This is a function that takes in the juce::Graphics *g pointer. Use this function in MainComponent.
     */
    void resized() override;
    
    void setCurrentTrackPtr (int *currentTrack);

    void setAudioTrackList(std::vector<te::Track*> *newTracks);
    
    void setEdit(te::Edit *edit);
    
// private member variables go here
private:
    int windowCenter = 0;
    int scale = 0;
    int controlImageHeightpx = 150;
    int controlImageWidthpx = 300;
    juce::Rectangle<int> paintWindow = juce::Rectangle<int>();
    
    /* Objects */
    te::TransportControl* transport;
    TrackManager* trackManager;
    viewObjects timelineObjects;
    std::vector<std::function<void()>> funcs;
    te::Edit *edit;
    
    /* Buttons */
    juce::TextButton playBtn{ "play" };
    juce::TextButton pauseBtn{ "pause" };
    juce::TextButton recordBtn{ "record" };
    juce::TextButton addTrackBtn{ "Add Track" };
    juce::TextButton leftBtn{ "<<<" };
    juce::TextButton rightBtn{ ">>>" };

    /* Labels */
    juce::Label statusLbl{ "statusLbl", "Status" };
    juce::Label trackCountLbl{ "trackCountLbl", "Track count" };

    /* Colours */
    juce::Colour bg_color = juce::Colour(64, 64, 64);
    juce::Colour alt_color = juce::Colour(0, 0, 0);
    
    /* Number of tracks pointer */
    int *numTracks;
    
    /* current track pointer */
    int *currentTrack;
    
    /* audio track list */
    std::vector<te::Track*> *audioTrackList = nullptr;

    /* broadcast functions*/
    void play();
    void pause();
    void record();
    
    /* draw once flag */
    bool draw_once_for_testing;

    /* this is the absolute path of Groovebox */
    const std::string apath = APATH;
    
    /* For waveforms */
    juce::Rectangle<int> waveform_window = juce::Rectangle<int>();
    
    void setupButtonImages ();
};

#endif
