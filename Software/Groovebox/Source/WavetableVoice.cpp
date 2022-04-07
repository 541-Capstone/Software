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

    isPrepared = true;

}

void WavetableVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {
    jassert(isPrepared);

    juce::dsp::AudioBlock<float> audioBlock{ outputBuffer };
    osc.process(juce::dsp::ProcessContextReplacing<float> {audioBlock});
    gain.process(juce::dsp::ProcessContextReplacing<float> {audioBlock});

    adsr.applyEnvelopeToBuffer(outputBuffer, startSample, numSamples);
}

void WavetableVoice::pitchWheelMoved(int newPitchWheelValue) {

}