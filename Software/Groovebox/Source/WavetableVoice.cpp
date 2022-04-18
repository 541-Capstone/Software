/*
  ==============================================================================

    WavetableVoice.cpp
    Created: 4 Apr 2022 7:58:11pm
    Author:  Dima

  ==============================================================================
*/

#include "WavetableVoice.h"

bool WavetableVoice::canPlaySound(juce::SynthesiserSound* sound) {
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void WavetableVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) {
    osc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    adsr.noteOn();
}
void WavetableVoice::stopNote(float velocity, bool allowTrailOff) {
    adsr.noteOff();
    
    if (!allowTrailOff || !adsr.isActive()) {
        clearCurrentNote();
    }
}
void WavetableVoice::controllerMoved(int controllerNumber, int newControllerValue) {

}

void WavetableVoice::prepareToPlay(double sampleRate, int samplesPerBlock) {
    adsr.setSampleRate(sampleRate);

    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;

    osc.prepare(spec);
    gain.prepare(spec);

    gain.setGainLinear(0.1f);

    adsrParams.attack = 0.8f;
    adsrParams.decay = 0.8f;
    adsrParams.sustain = 1.0f;
    adsrParams.release = 1.5f;

    adsr.setParameters(adsrParams);

    isPrepared = true;

}

void WavetableVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {
    jassert(isPrepared);

    if (!isVoiceActive()) {
        return;
    }

    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    synthBuffer.clear();

    juce::dsp::AudioBlock<float> audioBlock{ synthBuffer };
    osc.process(juce::dsp::ProcessContextReplacing<float> {audioBlock});
    gain.process(juce::dsp::ProcessContextReplacing<float> {audioBlock});

    adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumChannels());

    for (int i = 0; i < outputBuffer.getNumChannels(); i++) {
        outputBuffer.addFrom(i, startSample, synthBuffer, i, 0, numSamples);
        if (!adsr.isActive()) {
            clearCurrentNote();
        }
    }
}

void WavetableVoice::pitchWheelMoved(int newPitchWheelValue) {

}