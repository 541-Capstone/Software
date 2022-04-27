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
#include "ContextComponent.h"
#include "FileBrowserHandler.h"

class Timeline : public juce::ActionBroadcaster, public juce::ActionListener, public juce::Component, public juce::Timer, public juce::Button::Listener, public ContextComponent {
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
    void setTrackManager(std::shared_ptr<TrackManager>);
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
    void actionListenerCallback (const juce::String &message) override;
    /* This assumes that the type of message is CONTEXTUAL and is for TIMELINE */
    void contextControl(const juce::MidiMessageMetadata &metadata) override;
    Waveforms *getWaveformPtr();
    
// private member variables go here
private:
    
    /* Objects */
    te::TransportControl *transport;
    std::shared_ptr<TrackManager> trackManager;
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
    
    /* values to scroll */
    /* the controller value sets what encoder to use */
    const int scrollControllerValueX = 3;
    const int scrollControllerValueY = 4;
    
    /* the scroll scale is used for fine-tuning the
       rate of scrolling */
    const int scrollScale             = 5;
    const int scrollAmt               = 10;
    
    void setupWaveformDisplay();
    void redrawWaveform();
    
    /* File browser Variables */
    const int scrollAmount = 1;
    int cursorLocation     = 0;
    int cursorHeight       = 0;
    const int cursorWidth  = 10;
    juce::Rectangle<int> cursor;
    juce::Colour cursorColor = juce::Colours::red;
    FileBrowserHandler<juce::File> fileBrowserHandler;
    juce::File dir;
    
};

#endif
