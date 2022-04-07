/*
  ==============================================================================

    Waveforms.cpp
    Created: 14 Mar 2022 2:47:40pm
    Author:  Peter Tso

  ==============================================================================
*/

#include "Waveforms.h"

Waveforms::Waveforms () {
    waveformManagers = nullptr;
    edit = nullptr;
    setup();
}

Waveforms::~Waveforms () {
    clean();
}

void Waveforms::paint(juce::Graphics &g) {
    g.fillAll(juce::Colours::blue);
}

void Waveforms::resized() {
    return;
}

void Waveforms::setEdit(te::Edit *edit) {
    this->edit = edit;
}

void Waveforms::clean() {
    if (waveformManagers == nullptr) return;
    for (int i = 0; i < numTracks; ++i) {
        for (int j = 0; j < numClipsPerTrack; ++j) {
            delete waveformManagers[i][j];
            waveformManagers[i][j] = nullptr;
        }
        delete[]waveformManagers[i];
        waveformManagers[i] = nullptr;
    }
    delete[]waveformManagers;
}

void Waveforms::setup() {
    waveformManagers = new WaveformManager**[numTracks];
    for (int i = 0; i < numTracks; ++i) {
        waveformManagers[i] = new WaveformManager*[numClipsPerTrack];
        for (int j = 0; j < numClipsPerTrack; ++j) {
            waveformManagers[i][j] = new WaveformManager();
        }
    }
}

void Waveforms::showEdit() {
    if (edit == nullptr) {
        printf("No edit loaded\n");
        return;
    }
    auto audioTracks = te::getAudioTracks(*edit);
    int numAudioTracks = audioTracks.size();
    if (numAudioTracks > numTracks) numAudioTracks = numTracks;
    if (numAudioTracks <= 0) return;
    this->setBounds(this->getX(), this->getY(), this->getWidth(), heightPerTrack * numAudioTracks);
    for (int i = 0; i < numAudioTracks; ++i) {
        auto clips = audioTracks[i]->getClips();
        int numClips = clips.size();
        if (numClips > numClipsPerTrack) numClips = numClipsPerTrack;
        if (numClips > 0) {
            for (int k = 0; k < numClips; ++k) {
                auto clip = clips[k];
                if (clip->isMidi()) {
                    /* do midi rendering */
                    double clip_start = clip->getPosition().getStart() * timeScale;
                    double clip_length = clip->getPosition().getLength() * timeScale;
                    
                    

                }
                else {
                    juce::File file = clip->getSourceFileReference().getFile();
                    juce::URL url(file);
                    
                    double clip_start = clip->getPosition().getStart() * timeScale;
                    double clip_length = clip->getPosition().getLength() * timeScale;
                    
                    waveformManagers[i][k]->setBounds(clip_start, i * heightPerTrack, clip_length, heightPerTrack);
                    
                    addAndMakeVisible(waveformManagers[i][k]);
                    if (randomIsEnabled){
                        waveformManagers[i][k]->setForegroundColor(randomColor());
                        waveformManagers[i][k]->setBackgroundColor(randomColor());
                    }
                    waveformManagers[i][k]->showAudioResource(url);
                }
            }
        }
    }
    
}

void Waveforms::setColorRandomizer(bool set) {
    randomIsEnabled = set;
}

juce::Colour Waveforms::randomColor(){
    int limit = 256;
    auto& random = juce::Random::getSystemRandom();
    return juce::Colour(random.nextInt(limit), random.nextInt(limit), random.nextInt(limit));
}

void Waveforms::actionListenerCallback(const juce::String &message) {
    
}
