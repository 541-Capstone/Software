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
#include "Scrollable.h"

class Timeline : public juce::ActionBroadcaster, public juce::Component, public juce::Timer, public juce::Button::Listener {
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
    Timeline();
    ~Timeline();
    viewObjects* getObjects();
    void paint(juce::Graphics &g) override;
    void resized() override;
    void setCurrentTrackPtr (int *currentTrack);
    void setAudioTrackList(std::vector<te::Track*> *newTracks);
    void setEdit(te::Edit *edit);
    void timerCallback()override;
    void setMainComponentPtr(juce::Component *component);
    void addActionListener(juce::ActionListener *listener);
    void removeActionListener(juce::ActionListener *listener);
    void removeAllActionListener();
    void sendActionMessage(const juce::String &message) const;
    void setupFunctions(std::vector<std::function<void()> > vec);
    void onClick(juce::Button *button);
    void buttonClicked(juce::Button *button) override;
    void setAllComponents(bool state);
// private member variables go here
private:
    
    /* Objects */
    te::TransportControl *transport;
    TrackManager *trackManager;
    viewObjects timelineObjects;
    te::Edit *edit;
    juce::Component *mainComponent;
    
    /* Buttons */
    juce::TextButton playBtn{ "play" };
    juce::TextButton pauseBtn{ "pause" };
    juce::TextButton recordBtn{ "record" };
    juce::TextButton addTrackBtn{ "Add Track" };
    juce::TextButton leftBtn{ "<<<" };
    juce::TextButton rightBtn{ ">>>" };
    
    /* Function vector */
    std::vector<std::function<void() > > funcs;
    
    /* remove later */
    juce::TextButton addClip{"Add Clip"};

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
    void addAudioTrack();
    void nextTrack();
    void prevTrack();
    void addClipToTrack();

    /* this is the absolute path of Groovebox */
    const std::string apath = APATH;
    
    /* For waveforms */
    juce::Rectangle<int> waveform_window = juce::Rectangle<int>();
    
    /* For waveform views */
    Waveforms waveforms;
    Cursors cursors;
    Scrollable scrollable;
    
    void setupWaveformDisplay();
    void redrawWaveform();
};

#endif
