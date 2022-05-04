/*
  ==============================================================================

    WavetableVoice.cpp
    Created: 4 Apr 2022 7:58:11pm
    Author:  Dima

  ==============================================================================
*/

#include "WavetableVoice.h"

WavetableVoice::WavetableVoice(VoiceParams p) 
    : ampAdsr(p.ampAdsr), ampParams(p.ampParams), waveShape(p.waveShape) {
    isPlaying = false;
}

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
        ampAdsr.setParameters(ampParams);
        //Turn adsr(s) back on
        ampAdsr.noteOn();
        //Set oscillator note and restart
        osc.setNote(midiNoteNumber);
        osc.start();
    }
    
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
 
}
void WavetableVoice::controllerMoved(int controllerNumber, int newControllerValue) {

}

void WavetableVoice::prepareToPlay(double sampleRate, int samplesPerBlock) {
    ampAdsr.setSampleRate(sampleRate);

    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;

    osc.setSampleRate(sampleRate);
    gain.prepare(spec);

    gain.setGainLinear(0.2f);

    ampAdsr.setSampleRate(sampleRate);
    ampAdsr.setParameters(ampParams);

    osc.setWave(waveShape);

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

void WavetableVoice::updateParams(int numSamples) {
    ampAdsr.setParameters(ampParams);
}

void WavetableVoice::refreshOscillators() {
    osc.setWave(waveShape);
}