/*
  ==============================================================================

    Synth.cpp
    Created: 19 Apr 2022 10:54:12pm
    Author:  Dima

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Synth.h"

//==============================================================================
Synth::Synth()
{
    synthLoadedOnTrack = false;
    activeSynth = nullptr;
    addingSynth = false;
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
}

Synth::~Synth()
{
}

void Synth::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    int cellWidth = getLocalBounds().getWidth() / 4;
    juce::Rectangle<int> localBounds = getLocalBounds();
    juce::Rectangle<int> paramRect = localBounds.removeFromTop(localBounds.getHeight() * 0.75);
    g.drawText ("Synth", localBounds,
                juce::Justification::centred, true);   // draw some placeholder text
    if (synthLoadedOnTrack) {
        for (int i = 0; i < 4; i++) {
            juce::Rectangle<int> cellRect = paramRect.removeFromLeft(cellWidth);
            auto param = activeSynth->getParameterValue(i);
            juce::String paramStr = param.name + ":\n" + (juce::String)param.value;
            g.drawText(paramStr, cellRect, juce::Justification::centred, true);
        }
    }
    
}

void Synth::paintSynthLoaded(juce::Graphics&) {

}

void Synth::paintNoSynthLoaded(juce::Graphics&) {

}

void Synth::paintSelectionDialogue(juce::Graphics&) {

}

void Synth::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    int cellWidth = getLocalBounds().getWidth();
    juce::Rectangle<int> localBounds = getLocalBounds();
    juce::Rectangle<int> paramRect = localBounds.removeFromTop(localBounds.getHeight() * 0.25);
}

void Synth::contextControl(const juce::MidiMessageMetadata& metadata) {
    auto message = metadata.getMessage();
    if (message.getControllerNumber() != CONTEXTUAL_CC_CHANNEL) { return; }
    if (message.getControllerValue() < ENC_1_CW) {
        /*======================
        * 1: Cut
        * 2: Paste
        * 3: Save
        * 4: Load
        * 5: Add
        * 6: Delete
        =======================*/
        switch (message.getControllerValue()) {
            case 1: cut(); break;
            case 2: paste(); break;
            case 3: save(); break;
            case 4: load(); break;
            case 5: add(); break;
            case 6: del(); break;
            default: break;
        }
        return;
    }
    activeSynth->contextControl(metadata);
}

void Synth::loadTrack(const TrackManager::TrackWrapper& tw) {
    synthLoadedOnTrack = tw.hasSynth;
    if (synthLoadedOnTrack) {
        auto plugins = tw.track->pluginList.getPlugins();
        if (auto* synth = dynamic_cast<IPlugin*>(plugins[0].get())) {
            activeSynth = synth;
        }
        else {
            activeSynth = nullptr;
        }
    }

    
}

void Synth::cut() {}
void Synth::paste() {}
void Synth::save() {}
void Synth::load() {}
void Synth::add() {}
void Synth::del() {}
