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

void Waveforms::paint(Graphics &g) {
    g.fillAll(juce::Colours::blue);
    /*
    if (edit == nullptr) return;
    double playPosition = edit->getTransport().getCurrentPosition() * timeScale;
    juce::Rectangle<int> cursor = juce::Rectangle<int>();
    cursor.setBounds(playPosition, 0, cursor_width, this->getHeight());
    g.setColour(juce::Colours::white);
    g.fillRect(cursor);
    */
    return;
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
    this->setBounds(this->getX(), this->getY(), this->getWidth(), 128 * numAudioTracks);
    for (int i = 0; i < numAudioTracks; ++i) {
        auto clips = audioTracks[i]->getClips();
        int numClips = clips.size();
        if (numClips > numClipsPerTrack) numClips = numClipsPerTrack;
        if (numClips > 0) {
            for (int k = 0; k < numClips; ++k) {
                auto clip = clips[k];
                juce::File file = clip->getSourceFileReference().getFile();
                juce::URL url(file);
                
                double clip_start = clip->getPosition().getStart() * timeScale;
                double clip_length = clip->getPosition().getLength() * timeScale;
                
                waveformManagers[i][k]->setBounds(clip_start, i * 128, clip_length, 128);
                
                addAndMakeVisible(waveformManagers[i][k]);
                waveformManagers[i][k]->showAudioResource(url);
            }
        }
    }
}
