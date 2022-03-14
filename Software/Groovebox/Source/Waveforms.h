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
#include <stdio.h>
#include <unistd.h>
namespace te = tracktion_engine;
class Waveforms : public juce::Component {
public:
    Waveforms ();
    ~Waveforms() override;
    void paint(juce::Graphics &g) override;
    void showEdit ();
    void resized () override;
    void setEdit (te::Edit *edit);
private:
    WaveformManager ***waveformManagers = nullptr;
    const int cursor_width = 5;
    const double timeScale = 2.0f;
    const int numClipsPerTrack = 5;
    const int numTracks = 5;
    te::Edit *edit;
    void clean();
    void setup();
};
#endif
