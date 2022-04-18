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

class WavetableVoice : public juce::SynthesiserVoice {
public:
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

private:
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;

    juce::dsp::Oscillator<float> osc{ [](float x) {return std::sin(x); }};
    juce::dsp::Gain<float> gain;
    juce::AudioBuffer<float> synthBuffer;

    bool isPrepared { false };
};