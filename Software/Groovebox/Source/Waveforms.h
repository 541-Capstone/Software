/*
  ==============================================================================

    Waveforms.h
    Created: 14 Mar 2022 2:47:40pm
    Author:  Peter Tso

  ==============================================================================
*/

#pragma once
#ifndef waveforms_h
#define waveforms_h
#include <iostream>
#include <JuceHeader.h>
#include "WaveformManager.h"
#include "TrackManager.h"
#include <stdio.h>
#ifdef __MACH__
#include <unistd.h>
#endif

#ifdef _WIN64
#include <io.h>
#endif
namespace te = tracktion_engine;
class Waveforms : public juce::Component, public juce::ActionListener, public juce::ActionBroadcaster {
public:
    Waveforms ();
    ~Waveforms() override;
    void paint(juce::Graphics &g) override;
    void showEdit ();
    void resized () override;
    void setEdit (te::Edit *edit);
    void setColorRandomizer(bool set);
    void actionListenerCallback (const juce::String &message) override;
    void scrollAmount(int ss);
    void setTrackManager(std::shared_ptr<TrackManager>);
private:
    WaveformManager ***waveformManagers = nullptr;
    const int cursor_width = 5;
    const double timeScale = 4.0f;
    const int height_scale = 2;
    const int numClipsPerTrack = 25;
    const int numTracks = 5;
    const int heightPerTrack = 64 * height_scale;
    te::Edit *edit;
    std::shared_ptr<TrackManager> trackManager;
    juce::Array<te::AudioTrack*> returnValidAudioTracks();
    bool randomIsEnabled = false;
    void clean();
    void setup();
    void quickClean();
    void drawWaveform (te::Clip *clip, int i, int k);
    void drawLine (te::Clip *clip, int i, int k);
    juce::Colour randomColor();
    int scrollAmt = 0;
    bool utilize_alt_color = false;
};
#endif
