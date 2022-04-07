/*
  ==============================================================================

    AudioVisualizer.h
    Created: 13 Mar 2022 5:38:47pm
    Author:  Peter Tso

  ==============================================================================
*/

#pragma once
#ifndef audiovisualizer_h
#define audiovisualizer_h
#include <JuceHeader.h>
#include <functional>
namespace te = tracktion_engine;
class AudioVisualizer : public juce::Component, public juce::ChangeListener, public  juce::ChangeBroadcaster, private juce::ScrollBar::Listener, private juce::Timer {
public:
    AudioVisualizer (juce::AudioFormatManager& formatManager,
                     juce::AudioTransportSource& source);
    ~AudioVisualizer() override;
    
    void setURL (const juce::URL& url);
    void setTrack (te::AudioTrack *track);
    void loadFromTrackOrURL (bool load);
    void setRange (juce::Range<double> newRange);
    void paint (juce::Graphics &g) override;
    void resized () override;
    void changeListenerCallback (juce::ChangeBroadcaster*) override;
    juce::URL getLastDroppedFile() const noexcept;
    void setFollowsTransport (bool shouldFollow);
    void setThumbnailBounds (juce::Rectangle<int> bounds);
    void setWaveformColor (juce::Colour color);
    void setBackgroundColor (juce::Colour color);
    void isMIDI (bool isMidi);
    
private:
    juce::AudioTransportSource &transportSource;
    juce::AudioThumbnailCache thumbnailCache {5};
    juce::AudioThumbnail thumbnail;
    juce::ScrollBar scrollbar { false };
    juce::Range<double> visibleRange;
    bool isFollowingTransport = false;
    bool loadFrom = false;  // false is URL, true is AudioTrack
    te::AudioTrack *track;
    juce::URL lastfileDropped;
    juce::Rectangle<int> clipBounds = juce::Rectangle<int>();
    
    juce::Colour background_color = juce::Colours::black;
    juce::Colour waveform_background = juce::Colours::violet;
    juce::Colour waveform_color = juce::Colours::white;
    
    void scrollBarMoved (juce::ScrollBar *scrollBarThatHasMoved, double newRangeStart) override;
    void timerCallback ()override;
    bool isMidi = false;
};

#endif
