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
    void resized() override;

    void contextControl(const juce::MidiMessageMetadata& metadata) override;

    void loadTrack(const TrackManager::TrackWrapper&);

    void cut();
    void paste();
    void save();
    void load();
    void add();
    void del();

    void setTrackManager(std::shared_ptr<TrackManager>);
    void setEdit(std::shared_ptr<te::Edit> edit);

private:

    std::shared_ptr<TrackManager> trackManager;
    std::shared_ptr<te::Edit> edit;
    IPlugin* activeSynth;
    juce::Label adsrLbl{ "adsrLbl", "ADSR Label" };
    juce::Rectangle<int> textBox;
    bool synthLoadedOnTrack;

    //Painting components
    juce::Rectangle<int> localBounds;
    juce::Rectangle<int> paramRect;
    juce::Rectangle<int> paramRectLower;
    juce::Rectangle<int> cells[8];

    int cellWidth;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Synth)
};
