/*
  ==============================================================================

    Effects.h
    Created: 4 May 2022 3:37:11am
    Author:  Dima

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ContextComponent.h"
#include "Utilities.h"
#include "ContextComponent.h"
#include "TrackManager.h"
#include "Delay.h"

//==============================================================================
/*
*/
class Effects : public ContextComponent
{
public:
    Effects();
    ~Effects() override;

    void paint(juce::Graphics&) override;
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
    IPlugin* activeEffect;
    juce::Label adsrLbl{ "adsrLbl", "ADSR Label" };
    juce::Rectangle<int> textBox;
    bool effectLoadedOnTrack;

    //Painting components
    juce::Rectangle<int> localBounds;
    juce::Rectangle<int> paramRect;
    juce::Rectangle<int> paramRectLower;
    juce::Rectangle<int> cells[8];

    int cellWidth;
    //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Effect)
};