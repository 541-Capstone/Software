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
#include "ISynthVoice.h"

//==============================================================================
/*
*/
class Wavetable : public juce::Component,
    public te::Plugin,
    public IPlugin
    //public juce::AudioProcessor
{
    //using VTS = juce::AudioProcessorValueTreeState;
public:
    static const char* getPluginName();
    inline static const char* xmlTypeName = "Wavetable";

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



    bool takesMidiInput() override { return true; }
    bool takesAudioInput() override { return false; }
    bool isSynth() override { return true; }
    bool producesAudioWhenNoAudioInput() override { return true; }

    /*=================================================================================
    * IPlugin overrides
    ================================================================================*/
    // Maps contextual control messages to wavetable functions
    void contextControl(const juce::MidiMessageMetadata& metadata) override;

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

    void applyToBuffer(juce::AudioBuffer<float>&, juce::MidiBuffer&);

    te::ExpEnvelope ampAdsr;
    te::ExpEnvelope::Parameters ampParams;
    te::Oscillator::Waves waveShape{ te::Oscillator::Waves::sine };
    std::vector<te::Oscillator::Waves> waves;

    //=========================================================
    // Contextual input functions
    //=========================================================

    void incrementAttack(float amount);
    void incrementDecay(float amount);
    void incrementSustain(float amount);
    void incrementRelease(float amount);
    //Change wave shape. Pass "true" to pick next value in enum, "false" for previous
    void changeWave(bool next);

    // Updates all adsr parameters after flushing out samples. Should only be accessed by updateParams call
    void updateParams(int numSamples);
    void setVoice(const te::Oscillator::Waves& newWave);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Wavetable)
};
