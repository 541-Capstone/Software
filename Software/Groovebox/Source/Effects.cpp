/*
  ==============================================================================

    Effects.cpp
    Created: 4 May 2022 3:37:11am
    Author:  Dima

  ==============================================================================
*/

#include "Effects.h"

Effects::Effects() {
    activeEffect = nullptr;
    edit = nullptr;
    trackManager = nullptr;
    effectLoadedOnTrack = false;
}

Effects::~Effects() {}

void Effects::setTrackManager(std::shared_ptr<TrackManager> tm) {
    trackManager = tm;
}
void Effects::setEdit(std::shared_ptr<te::Edit> e) {
    edit = e;
}

void Effects::paint(juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component



    g.setColour(juce::Colours::white);
    g.setFont(32.0f);

    if (effectLoadedOnTrack) {
        for (int i = 0; i < 8; i++) {
            auto param = activeEffect->getParameterValue(i);
            juce::String paramStr = param.name + ":\n" + (juce::String)param.value;
            g.drawText(paramStr, cells[i], juce::Justification::centred, true);
        }
        /*g.drawText("Effects", localBounds,
            juce::Justification::horizontallyCentred, false);*/
    }
    else {
        g.drawText("No Effects loaded, press 'Add' to load", localBounds,
            juce::Justification::horizontallyCentred, false);   // draw some placeholder text
    }



}

void Effects::resized()
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

void Effects::contextControl(const juce::MidiMessageMetadata& metadata) {
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
    activeEffect->contextControl(metadata);
}

void Effects::loadTrack(const TrackManager::TrackWrapper& tw) {
    effectLoadedOnTrack = tw.hasEffects;
    if (effectLoadedOnTrack) {
        auto plugins = tw.track->pluginList.getPlugins();
        if (auto* effect = dynamic_cast<IPlugin*>(*plugins.end())) {
            activeEffect = effect;
        }
        else {
            activeEffect = nullptr;
        }
    }
}

void Effects::cut() {}
void Effects::paste() {}
void Effects::save() {}
void Effects::load() {}
void Effects::add() {
    if (activeEffect == nullptr) {
        if (auto effect = dynamic_cast<Delay*> (edit->getPluginCache().createNewPlugin(Delay::xmlTypeName, {}).get()))
        {
            if (auto t = trackManager->getActiveTrack()) {
                t->getTrack()->pluginList.insertPlugin(*effect, 1, nullptr);
                if (IPlugin* e = dynamic_cast<IPlugin*>(effect)) {
                    activeEffect = e;
                    t->hasEffects = true;
                    effectLoadedOnTrack = true;
                }

            }
        }
    }
}
void Effects::del() {
    auto track = trackManager->getActiveTrack();
    if (track->hasEffects) {
        auto plugin = *track->getTrack()->pluginList.getPlugins().end();
        plugin->removeFromParent();
        track->hasEffects = false;
        activeEffect = nullptr;
        effectLoadedOnTrack = false;
    }
}