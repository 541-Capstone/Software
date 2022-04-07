/*
  ==============================================================================

    Wavetable.cpp
    Created: 4 Apr 2022 3:13:52pm
    Author:  Dima

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Wavetable.h"

//==============================================================================
Wavetable::Wavetable(te::PluginCreationInfo info) : te::Plugin(info)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    auto um = getUndoManager();
}

Wavetable::~Wavetable()
{
    notifyListenersOfDeletion();
}

void Wavetable::setSampleRate(int newSampleRate) {
    sampleRate = newSampleRate;
}

juce::String Wavetable::getName()                       { return getPluginName(); }
juce::String Wavetable::getPluginType()                 { return xmlTypeName; }
bool Wavetable::needsConstantBufferSize()               { return false; }
juce::String Wavetable::getSelectableDescription()      { return getName(); }

void Wavetable::initialise(const te::PluginInitialisationInfo& info)    {
    sampleRate = info.sampleRate;
    synth.setCurrentPlaybackSampleRate(sampleRate);
    synth.addSound(new WavetableSound());
    synth.addVoice(new WavetableVoice());

    for (int i = 0; i < synth.getNumVoices(); i++) {
        if (auto voice = dynamic_cast<WavetableVoice*>(synth.getVoice(i))) {
            voice->prepareToPlay(sampleRate, info.blockSizeSamples);
        }
    }
}
void Wavetable::deinitialise() {}


//Functions similarly to the processBlock of a standalone plugin

void Wavetable::applyToBuffer(const te::PluginRenderContext& fc) {

    for (int channel = 0; channel < fc.destBuffer->getNumChannels(); ++channel)
    {
        auto dest = fc.destBuffer->getWritePointer(channel);

        //Fill output buffer here
        /*for (int i = 0; i < fc.bufferNumSamples; ++i)
            dest[i] = std::tanh(gainValue * dest[i]);*/
        for (int i = 0; i < fc.bufferNumSamples; i++) {
            
        }

        //Check if user has updated settings
        for (int i = 0; i < synth.getNumVoices(); i++) {
            if (auto voice = dynamic_cast<juce::SynthesiserVoice*>(synth.getVoice(i))) {
                //Update settings here
                //ADSR
            }
        }

        synth.renderNextBlock(*fc.destBuffer, *midiBuffer, 0, fc.destBuffer->getNumSamples());
    }

    
}


void Wavetable::handleMidiEvent(juce::MidiMessage msg, int sampleNumber, bool record) {
    Helpers::MessageType type = Helpers::getMidiMessageType(msg);
    if (type == Helpers::MessageType::Note) {
        
        
        if (record) {
            addMessageToBuffer(msg, sampleNumber);
        }
    }
}

void Wavetable::addMessageToBuffer(const juce::MidiMessage& msg, int sampleNumber) {
    midiBuffer->addEvent(msg, sampleNumber);
}

void Wavetable::setMidiBuffer(std::shared_ptr<juce::MidiBuffer> buffer) {
    midiBuffer = buffer;
}

void Wavetable::removeMidiBuffer() {
    midiBuffer = nullptr;
}

void Wavetable::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("Wavetable", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void Wavetable::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

const char* Wavetable::getPluginName() { return NEEDS_TRANS("Wavetable"); }