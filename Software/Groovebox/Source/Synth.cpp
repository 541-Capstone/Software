/*
  ==============================================================================

    Synth.cpp
    Created: 19 Apr 2022 10:54:12pm
    Author:  Dima

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Synth.h"
#include "Wavetable.h"

//==============================================================================
Synth::Synth()
{
    synthLoadedOnTrack = false;
    edit = nullptr;
    trackManager = nullptr;
    activeSynth = nullptr;
    /*for (int i = 0; i < 8; i++) {
        lbls.push_back(new juce::Label( "Param" + (juce::String)i, "Param" ));
        addAndMakeVisible(*lbls[i]);
    }*/
    addAndMakeVisible(adsrLbl);
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
}

Synth::~Synth()
{
}

void Synth::setTrackManager(std::shared_ptr<TrackManager> tm) {
    trackManager = tm;
}
void Synth::setEdit(std::shared_ptr<te::Edit> e) {
    edit = e;
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
    g.setFont (32.0f);
    
    if (synthLoadedOnTrack) {
        for (int i = 0; i < 8; i++) {
            auto param = activeSynth->getParameterValue(i);
            juce::String paramStr = param.name + ":\n" + (juce::String)param.value;
            g.drawText(paramStr, cells[i], juce::Justification::centred, true);
        }
        /*g.drawText("Synth", localBounds,
            juce::Justification::horizontallyCentred, false);*/
    }
    else {
        g.drawText("No Synth loaded, press 'Add' to load", localBounds,
            juce::Justification::horizontallyCentred, false);   // draw some placeholder text
    }

    
    
}

void Synth::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    localBounds = getLocalBounds();
    paramRect = getLocalBounds().removeFromTop(localBounds.getHeight() * 0.8);
    paramRectLower = paramRect.removeFromBottom(paramRect.getHeight() * 0.5);

    cellWidth = getLocalBounds().getWidth() / 4;

    for (int i = 0; i < 8; i++) {
        auto& rectToUse = (i < 4) ? paramRect : paramRectLower;
        cells[i] = rectToUse.removeFromLeft(cellWidth);
    }
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
void Synth::add() {
    if (activeSynth == nullptr) {
        if (auto synth = dynamic_cast<Wavetable*> (edit->getPluginCache().createNewPlugin(Wavetable::xmlTypeName, {}).get()))
        {
            if (auto t = trackManager->getActiveTrack()) {
                t->getTrack()->pluginList.insertPlugin(*synth, 0, nullptr);
                if (IPlugin* s = dynamic_cast<IPlugin*>(synth)) {
                    activeSynth = s;
                    t->hasSynth = true;
                    synthLoadedOnTrack = true;
                }
                
            }
        }
    }
}
void Synth::del() {
    auto track = trackManager->getActiveTrack();
    if (track->hasSynth) {
        auto plugins = track->getTrack()->pluginList.getPlugins();
        plugins[0]->removeFromParent();
        track->hasSynth = false;
        activeSynth = nullptr;
        synthLoadedOnTrack = false;
    }
}
