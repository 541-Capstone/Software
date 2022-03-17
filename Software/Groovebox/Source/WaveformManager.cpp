/*
  ==============================================================================

    WaveformManager.cpp
    Created: 13 Mar 2022 6:13:31pm
    Author:  Peter Tso

  ==============================================================================
*/

#include "WaveformManager.h"

WaveformManager::WaveformManager(){
    formatManager.registerBasicFormats();
    thumbnail.reset(new AudioVisualizer(formatManager, transportSource));
    addAndMakeVisible(thumbnail.get());
    thumbnail->addChangeListener(this);
    edit = nullptr;
    
}

WaveformManager::~WaveformManager(){
    transportSource.setSource(nullptr);
    thumbnail->removeChangeListener(this);
}

void WaveformManager::showAudioResource(juce::URL resource) {
    if (loadURLIntoTransport(resource))
        currentAudioFile = std::move (resource);
    thumbnail->setWaveformColor(fgColor);
    thumbnail->setBackgroundColor(bgColor);
    thumbnail->setURL(currentAudioFile);
}

bool WaveformManager::loadURLIntoTransport(const juce::URL &audioURL) {
    transportSource.stop();
    transportSource.setSource(nullptr);
    currentAudioFileSource.reset();
    
    juce::AudioFormatReader* reader = nullptr;
    if (audioURL.isLocalFile())
        reader = formatManager.createReaderFor(audioURL.getLocalFile());
    else {
        if (reader == nullptr) {
            reader = formatManager.createReaderFor (audioURL.createInputStream (URL::InputStreamOptions (URL::ParameterHandling::inAddress)));
        }
    }
    
    if (reader != nullptr) {
        currentAudioFileSource.reset(new juce::AudioFormatReaderSource(reader, true));
        
        transportSource.setSource(currentAudioFileSource.get(), 0, nullptr, reader->sampleRate);
        
        return true;
    }
    return false;
    
}

void WaveformManager::changeListenerCallback(ChangeBroadcaster *source) {
    if (source == thumbnail.get())
        showAudioResource(juce::URL (thumbnail->getLastDroppedFile()));
}

void WaveformManager::paint (juce::Graphics &g) {
    g.setColour(default_color);
    g.fillAll();
}

void WaveformManager::resized() {
    thumbnail->setBounds(getLocalBounds());
}


void WaveformManager::updateFollowTransportState() {
    thumbnail->setFollowsTransport(true);
}

void WaveformManager::passEdit(te::Edit *edit) {
    this->edit = edit;
}

void WaveformManager::loadFromTrackOrURL(bool load) {
    this->loadFrom = load;
}

void WaveformManager::showAudioResource(te::AudioTrack *track) {
    thumbnail->loadFromTrackOrURL(false);
    auto clips = track->getClips();
    double clip_length = 0.0f;
    double track_length = track->getLength();
    double clip_offset = 0.0f;
    juce::Rectangle<int> clipWindow = juce::Rectangle<int>();
    clipWindow.setY(thumbnail->getY());
    clipWindow.setHeight(thumbnail->getY());
    int thumbnail_width = thumbnail->getWidth();
    for (auto clip: clips) {
        te::SourceFileReference *ref = &clip->getSourceFileReference();
        juce::URL url_file(ref->getFile());
        
        clip_length = clip->getPosition().getLength();
        clip_offset = clip->getPosition().offset;
        clipWindow.setWidth((int)(clip_length/track_length) * thumbnail_width);
        clipWindow.setX(thumbnail->getX() + (int)(clip_offset/track_length)*thumbnail_width);
        
        
        thumbnail->setThumbnailBounds(clipWindow);
        if (loadURLIntoTransport(url_file)) {
            thumbnail->setURL(url_file);
        }
    }
}

void WaveformManager::showAudioResource(te::Edit *p_edit) {
    auto audioTracks = te::getAudioTracks(*p_edit);
    int numTracks = audioTracks.size();
    juce::Rectangle<int> trackWindow = juce::Rectangle<int>();
    trackWindow.setBounds(0, 0, trackWindowWidth, trackWindowHeight);
    this->setBounds(this->getX(), this->getY(), this->getWidth(), trackWindow.getHeight() * numTracks);
    thumbnail->loadFromTrackOrURL(true);
    for (auto track: audioTracks) {
        auto clips = track->getClips();
        double clip_length = 0.0f;
        double clip_offset = 0.0f;
        juce::Rectangle<int>clipWindow = juce::Rectangle<int>();
        clipWindow.setY(trackWindow.getY());
        clipWindow.setHeight(trackWindow.getHeight());
        for (auto clip: clips) {
            te::SourceFileReference *ref = &clip->getSourceFileReference();
            juce::URL url_file(ref->getFile());
            
            clip_length = clip->getPosition().getEnd();
            clip_offset = clip->getPosition().offset;
            clipWindow.setWidth((int)(clip_length/timeScale));
            clipWindow.setX(0 + (int)(clip_offset/timeScale));
            
            thumbnail->setThumbnailBounds(clipWindow);
            if (loadURLIntoTransport(url_file)) {
                thumbnail->setURL(url_file);
            }
        }
        
        trackWindow.setY(trackWindow.getY() + trackWindow.getHeight());
    }
}

void WaveformManager::setBackgroundColor(juce::Colour color) {
    if (!bgcolorChanged) bgColor = color;
    bgcolorChanged = true;
}

void WaveformManager::setForegroundColor(juce::Colour color) {
    if(!fgcolorChanged) fgColor = color;
    fgcolorChanged = true;
}
