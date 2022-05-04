/*
  ==============================================================================

    Delay.h
    Created: 27 Apr 2022 11:14:52pm
    Author:  Dima

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Utilities.h"
#include "IPlugin.h"

class Delay : public te::Plugin,
    public IPlugin {
public:
    Delay(te::PluginCreationInfo info);
    ~Delay() override;

    static const char* getPluginName();
    inline static const char* xmlTypeName = "GrooveboxDelay";

    juce::String getName() override;
    juce::String getPluginType() override;
    bool needsConstantBufferSize() override;
    juce::String getSelectableDescription() override;
    void initialise(const te::PluginInitialisationInfo&) override;
    void deinitialise() override;

    //Where we fill our buffer with data
    void applyToBuffer(const te::PluginRenderContext& fc) override;

    /*=================================================================================
    * IPlugin overrides
    ================================================================================*/
    // Maps contextual control messages to wavetable functions
    void contextControl(const juce::MidiMessageMetadata& metadata) override;
    IPlugin::Parameter getParameterValue(int index) override;



private:
    int sampleRate;
    juce::AudioBuffer<float> delayBuffer;
    int writePosition{ 0 }; //"where is our audio currently?"
    void fillBuffer(int channel, int bufferSize, int delayBufferSize, float* channelData);
    void readFromBuffer(juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& delayBuffer, int channel, int bufferSize, int delayBufferSize);

};