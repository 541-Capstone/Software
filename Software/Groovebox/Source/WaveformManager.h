/*
  ==============================================================================

    WaveformManager.h
    Created: 13 Mar 2022 6:13:31pm
    Author:  Peter Tso

  ==============================================================================
*/

#pragma once
#ifndef waveformmanager_h
#define waveformmanager_h
#include <JuceHeader.h>
#include <iostream>
#include <functional>
#include "AudioVisualizer.h"
namespace te = tracktion_engine;
class WaveformManager : public juce::Component, private juce::ChangeListener {
public:
    WaveformManager ();
    ~WaveformManager () override;
    void paint (juce::Graphics &g) override;
    void resized () override;
    void passEdit(te::Edit *edit);
    void loadFromTrackOrURL (bool load);
    void showAudioResource (juce::URL resource);
    void showAudioResource (te::AudioTrack *track);
    
private:
    juce::AudioFormatManager formatManager;
    juce::URL currentAudioFile;
    juce::AudioTransportSource transportSource;
    std::unique_ptr<juce::AudioFormatReaderSource> currentAudioFileSource;
    std::unique_ptr<AudioVisualizer> thumbnail;
    te::Edit *edit;
    bool loadFrom = false; // false is URL, true is AudioTrack
    
    juce::Colour default_color = juce::Colours::black;
    
    void changeListenerCallback (juce::ChangeBroadcaster *source) override;
    bool loadURLIntoTransport (const juce::URL &audioURL);
    void updateFollowTransportState ();
};
#endif
