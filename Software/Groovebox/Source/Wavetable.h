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
class Wavetable : public juce::Component,
    public te::Plugin
    //public juce::AudioProcessor
{
    using VTS = juce::AudioProcessorValueTreeState;
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
    * juce::AudioProcessor overrides
    ================================================================================*/
//    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
//    void releaseResources() override;
//
//#ifndef JucePlugin_PreferredChannelConfigurations
//    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
//#endif
//
//    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
//
//    juce::AudioProcessorEditor* createEditor() override;
//    bool hasEditor() const override;
//
//    //==============================================================================
//    const juce::String getName() const override;
//    juce::StringArray getAlternateDisplayNames() const override;
//
//    bool acceptsMidi() const override;
//    bool producesMidi() const override;
//    bool isMidiEffect() const override;
//    double getTailLengthSeconds() const override;
//
//    //==============================================================================
//    int getNumPrograms() override;
//    int getCurrentProgram() override;
//    void setCurrentProgram(int index) override;
//    const juce::String getProgramName(int index) override;
//    void changeProgramName(int index, const juce::String& newName) override;
//
//    //==============================================================================
//    void getStateInformation(juce::MemoryBlock& destData) override;
//    void setStateInformation(const void* data, int sizeInBytes) override;

    void addMessageToBuffer(const juce::MidiMessage&, int sampleNumber);

    void setMidiBuffer(std::shared_ptr<juce::MidiBuffer>);
    void removeMidiBuffer();

    void handleMidiEvent(juce::MidiMessage msg, int sampleNumber, bool record);

    juce::AudioProcessorValueTreeState& getValueTreeState();

private:
    juce::Array<float> wavetable;
    double sampleRate = 48000.0;

    juce::Synthesiser synth;

    std::shared_ptr<juce::MidiBuffer> midiBuffer;
    Helpers::PluginType pluginType = Helpers::PluginType::Synth;

    void applyToBuffer(juce::AudioBuffer<float>&, juce::MidiBuffer&);

    //Value tree
    //VTS paramState;
    VTS::ParameterLayout createParameters();

    //Parameters
    /*juce::AudioParameterFloat attack;
    juce::AudioParameterFloat sustain;
    juce::AudioParameterFloat decay;
    juce::AudioParameterFloat release;
    juce::AudioParameterInt   oscillator;*/

    enum Oscillators {Sine, Saw, Square };
    int numOscillators = 3; //Enums don't have a length method...
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Wavetable)
};
