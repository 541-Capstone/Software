//
//  TrackWindow.h
//  Groovebox
//
//  Created by Peter Tso on 1/27/22.
//

#ifndef TrackWindow_h
#define TrackWindow_h

#include <iostream>
#include <functional>
#include <JuceHeader.h>
#include <vector>
#include <functional>

class TrackWindow : public juce::AudioAppComponent, public::JUCEApplication {
public:
    
    /**
     * @brief: This is the constructor for the TrackWindow component
     */
    TrackWindow();
    
    /**
     * @brief: This is the destructor for the TrackWindow component
     */
    ~TrackWindow() override;
    
    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

    void timerCallback() override;
    
    enum TrackStates{
        Selected,
        Not_Selected
    };
    
private:
    
    auto bg_color = juce::Colours::white;
    
};


#endif /* TrackWindow_h */
