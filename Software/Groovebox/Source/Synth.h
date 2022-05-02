/*
  ==============================================================================

    Synth.h
    Created: 19 Apr 2022 10:54:12pm
    Author:  Dima

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ContextComponent.h"
#include "Utilities.h"
#include "ContextComponent.h"
#include "TrackManager.h"

//==============================================================================
/*
*/
class Synth  : public ContextComponent
{
public:
    Synth();
    ~Synth() override;

    void paint (juce::Graphics&) override;
    void paintSynthLoaded(juce::Graphics&);
    void paintNoSynthLoaded(juce::Graphics&);
    void paintSelectionDialogue(juce::Graphics&);
    void resized() override;

    void contextControl(const juce::MidiMessageMetadata& metadata) override;

    void loadTrack(const TrackManager::TrackWrapper&);

    void cut();
    void paste();
    void save();
    void load();
    void add();
    void del();

private:

    IPlugin* activeSynth;
    std::vector<juce::Rectangle<int>> paramCells;
    bool synthLoadedOnTrack, addingSynth;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Synth)
};
