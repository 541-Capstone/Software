/*
  ==============================================================================

    AudioVisualizer.cpp
    Created: 13 Mar 2022 5:38:47pm
    Author:  Peter Tso

  ==============================================================================
*/

#include "AudioVisualizer.h"

AudioVisualizer::AudioVisualizer (juce::AudioFormatManager& formatManager, juce::AudioTransportSource& source) : transportSource(source), thumbnail(512, formatManager, thumbnailCache) {
    thumbnail.addChangeListener(this);
    
    addAndMakeVisible(scrollbar);
    scrollbar.setRangeLimits(visibleRange);
    scrollbar.setAutoHide(false);
    scrollbar.addListener(this);
    scrollbar.setEnabled(false);
    scrollbar.setVisible(false);
    
    clipBounds = getLocalBounds();
    
    track = nullptr;
}

AudioVisualizer::~AudioVisualizer () {
    scrollbar.removeListener(this);
    thumbnail.removeChangeListener(this);
}

void AudioVisualizer::setRange(juce::Range<double> newRange) {
    visibleRange = newRange;
    scrollbar.setCurrentRange(visibleRange);
    updateMouseCursor();
    repaint();
}

void AudioVisualizer::setURL (const juce::URL& url) {
    juce::InputSource *inputSource = nullptr;
    if (url.isLocalFile()) {
        inputSource = new juce::FileInputSource(url.getLocalFile());
    }
    else {
        if (inputSource == nullptr) {
            inputSource = new juce::URLInputSource  (url);
        }
    }
    
    if (inputSource != nullptr) {
        thumbnail.setSource(inputSource);
        juce::Range<double> newRange (0.0, thumbnail.getTotalLength());
        scrollbar.setRangeLimits(newRange);
        setRange(newRange);
        
        startTimerHz (40);
    }
}

void AudioVisualizer::paint(juce::Graphics &g) {
    g.fillAll(background_color);
    g.setColour(waveform_color);
    
    if (isMidi) {
        auto area = getLocalBounds();
        area.removeFromTop(scrollbar.getHeight() + 4);
        
        juce::Rectangle<int> line;
        line.removeFromTop(this->getHeight()/1.5);
        line.removeFromBottom(this->getHeight()/1.5);
        
        g.drawRect(line);
        return;
    }
    
    if (thumbnail.getTotalLength() > 0) {
        auto thumbArea = getLocalBounds ();
        thumbArea.removeFromTop(scrollbar.getHeight() + 4);
        if (loadFrom) {
            thumbnail.drawChannels(g, clipBounds, visibleRange.getStart(), visibleRange.getEnd(), 1.0f);
        }
        else {
            thumbnail.drawChannels(g, thumbArea.reduced(2), visibleRange.getStart(), visibleRange.getEnd(), 1.0f);
        }
    }
    else {
        g.setFont(14.0f);
        g.drawFittedText("No audio Selected / Not working!", getLocalBounds(), juce::Justification::centred, 2);
    }
}

void AudioVisualizer::resized() {
    scrollbar.setBounds (getLocalBounds().removeFromBottom(14).reduced(2));
}

void AudioVisualizer::changeListenerCallback(juce::ChangeBroadcaster * dont_care) {
    repaint();
}

void AudioVisualizer::scrollBarMoved(juce::ScrollBar *scrollBarThatHasMoved, double newRangeStart) {
    
}

void AudioVisualizer::timerCallback() {
    
}

juce::URL AudioVisualizer::getLastDroppedFile() const noexcept {
    return lastfileDropped;
}

void AudioVisualizer::setFollowsTransport(bool shouldFollow) {
    isFollowingTransport = shouldFollow;
}

void AudioVisualizer::setTrack(te::AudioTrack *track) {
    this->track = track;
}

void AudioVisualizer::loadFromTrackOrURL(bool load) {
    loadFrom = load;
}

void AudioVisualizer::setThumbnailBounds(juce::Rectangle<int> bounds) {
    clipBounds = bounds;
}

void AudioVisualizer::setWaveformColor(juce::Colour color) {
    waveform_color = color;
}

void AudioVisualizer::setBackgroundColor(juce::Colour color) {
    background_color = color;
}

void AudioVisualizer::setisMIDI(bool isMidi){
    this->isMidi = isMidi;
}
