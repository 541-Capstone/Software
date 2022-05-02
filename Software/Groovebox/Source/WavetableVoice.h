/*
  ==============================================================================

    WavetableVoice.h
    Created: 4 Apr 2022 7:58:11pm
    Author:  Dima

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "WavetableSound.h"
#include "Utilities.h"
namespace te = tracktion_engine;
class WavetableVoice : public juce::SynthesiserVoice {
public:
    //Container for all voice parameters
    struct VoiceParams {
        te::ExpEnvelope& ampAdsr;
        te::ExpEnvelope::Parameters& ampParams;
        te::Oscillator::Waves& waveShape;
    };
    //==============================================
    // SynthesizerVoice virtual functions
    //==============================================
    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTrailOff) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
    void pitchWheelMoved(int newPitchWheelValue) override;

    //==============================================
    // Custom functions
    //==============================================
    WavetableVoice(VoiceParams p);

private:
    te::Oscillator osc;
    te::ExpEnvelope& ampAdsr;
    te::ExpEnvelope::Parameters& ampParams;
    //juce::dsp::Oscillator<float> osc{ [](float x) {return std::sin(x); }};
    juce::dsp::Gain<float> gain;
    juce::AudioBuffer<float> synthBuffer;
    te::Oscillator::Waves& waveShape;

    //Track if a note is currently playing, so adsr can be reset
    bool isPlaying;

    bool isPrepared { false };

    // Updates all adsr parameters after flushing out samples. Should only be accessed by updateParams call
    void updateParams(int numSamples);

};
