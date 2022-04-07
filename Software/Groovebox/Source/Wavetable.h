/*
  ==============================================================================

    Wavetable.h
    Created: 4 Apr 2022 3:13:52pm
    Author:  Dima

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Utilities.h"
#include "IPlugin.h"
#include "WavetableSound.h"
#include "WavetableVoice.h"

//==============================================================================
/*
*/
class Wavetable  : public juce::Component,
    public te::Plugin
{
public:
    static const char* getPluginName();
    inline static const char* xmlTypeName;

    // Pass in a reference to the engine so that we can register this plugin with the plugin manager
    Wavetable(te::PluginCreationInfo info);
    ~Wavetable() override;

    void setSampleRate(int newSampleRate);

    void paint (juce::Graphics&) override;
    void resized() override;

    /*=================================================================================
    * te::Plugin overrides
    ================================================================================*/
    juce::String getName() override;
    juce::String getPluginType() override;
    bool needsConstantBufferSize() override;
    juce::String getSelectableDescription() override;
    void initialise(const te::PluginInitialisationInfo&) override;
    void deinitialise() override;

    //Where we fill our buffer with data
    void applyToBuffer(const te::PluginRenderContext& fc) override;

    void addMessageToBuffer(const juce::MidiMessage&, int sampleNumber);

    void setMidiBuffer(std::shared_ptr<juce::MidiBuffer>);
    void removeMidiBuffer();

    void handleMidiEvent(juce::MidiMessage msg, int sampleNumber, bool record);


private:
    juce::Array<float> wavetable;
    double sampleRate = 48000.0;

    juce::Synthesiser synth;

    std::shared_ptr<juce::MidiBuffer> midiBuffer;
    Helpers::PluginType pluginType = Helpers::PluginType::Synth;

    //Value tree
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Wavetable)
};
