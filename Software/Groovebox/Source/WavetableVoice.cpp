/*
  ==============================================================================

    WavetableVoice.cpp
    Created: 4 Apr 2022 7:58:11pm
    Author:  Dima

  ==============================================================================
*/

#include "WavetableVoice.h"

WavetableVoice::WavetableVoice(te::ExpEnvelope& amp) : ampAdsr(amp) {}

bool WavetableVoice::canPlaySound(juce::SynthesiserSound* sound) {
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void WavetableVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) {
    if (isPlaying){
        stopNote(0.0f, true);
    }
    else {
        isPlaying = true;
        //Reset adsr(s)
        ampAdsr.reset();
        //Update adsr parameters
        //Turn adsr(s) back on
        ampAdsr.noteOn();
        //Set oscillator note and restart
        osc.setNote(midiNoteNumber);
        osc.start();
    }
    /*osc.setNote(midiNoteNumber);
    osc.start();
    ampAdsr.noteOn();*/
    
}
void WavetableVoice::stopNote(float velocity, bool allowTrailOff) {
    if (allowTrailOff)
    {
        ampAdsr.noteOff();
        isPlaying = false;
    }
    else
    {
        ampAdsr.reset();
        clearCurrentNote();
        isPlaying = false;
    }

    //ampAdsr.noteOff();
 
}
void WavetableVoice::controllerMoved(int controllerNumber, int newControllerValue) {

}

void WavetableVoice::prepareToPlay(double sampleRate, int samplesPerBlock) {
    adsr.setSampleRate(sampleRate);

    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;

    osc.setSampleRate(sampleRate);
    gain.prepare(spec);

    gain.setGainLinear(0.1f);

    te::ExpEnvelope::Parameters ampParams;
    ampParams.attack = 0.1f;
    ampParams.decay = 0.1f;
    ampParams.sustain = 0.1f;
    ampParams.release = 0.1f;

    ampAdsr.setSampleRate(sampleRate);
    ampAdsr.setParameters(ampParams);

    isPrepared = true;

}

void WavetableVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {
    jassert(isPrepared);

    if (!isVoiceActive()) {
        return;
    }

    if (!ampAdsr.isActive()) {
        isPlaying = false;
    }

    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    synthBuffer.clear();

    osc.process(synthBuffer,startSample,numSamples);
    juce::dsp::AudioBlock<float> audioBlock{ synthBuffer };
    gain.process(juce::dsp::ProcessContextReplacing<float> {audioBlock});

    ampAdsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumChannels());

    for (int i = 0; i < outputBuffer.getNumChannels(); i++) {
        outputBuffer.addFrom(i, startSample, synthBuffer, i, 0, numSamples);
        if (!ampAdsr.isActive()) {
            clearCurrentNote();
        }
    }
}

void WavetableVoice::pitchWheelMoved(int newPitchWheelValue) {

}