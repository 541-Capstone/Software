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

}
void WavetableVoice::stopNote(float velocity, bool allowTrailOff) {

}
void WavetableVoice::controllerMoved(int controllerNumber, int newControllerValue) {

}
void WavetableVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {

}

void WavetableVoice::pitchWheelMoved(int newPitchWheelValue) {

}