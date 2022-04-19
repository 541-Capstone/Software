/*
  ==============================================================================

    Wavetable.cpp
    Created: 4 Apr 2022 3:13:52pm
    Author:  Dima

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Wavetable.h"
#include "WavetableEditor.h"

//==============================================================================
Wavetable::Wavetable(te::PluginCreationInfo info) : te::Plugin(info)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    //paramState = juce::AudioProcessorValueTreeState(*this, getUndoManager(), "Parameters", createParameters());
}

Wavetable::~Wavetable()
{
    notifyListenersOfDeletion();
}

void Wavetable::setSampleRate(int newSampleRate) {
    sampleRate = newSampleRate;
}

//const juce::String Wavetable::getName() const           { return getPluginName(); }
juce::String Wavetable::getName()                       { return getPluginName(); }
//juce::StringArray Wavetable::getAlternateDisplayNames() const { return juce::StringArray((juce::String)"WT"); }
juce::String Wavetable::getPluginType()                 { return xmlTypeName; }
bool Wavetable::needsConstantBufferSize()               { return false; }
juce::String Wavetable::getSelectableDescription()      { return getName(); }

void Wavetable::initialise(const te::PluginInitialisationInfo& info)    {
    sampleRate = info.sampleRate;
    synth.setCurrentPlaybackSampleRate(sampleRate);
    synth.addSound(new WavetableSound());
    synth.addVoice(new WavetableVoice(ampAdsr));

    for (int i = 0; i < synth.getNumVoices(); i++) {
        if (auto voice = dynamic_cast<WavetableVoice*>(synth.getVoice(i))) {
            voice->prepareToPlay(sampleRate, info.blockSizeSamples);
        }
    }
}
void Wavetable::deinitialise() {}

//Functions similarly to the processBlock of a standalone plugin

void Wavetable::applyToBuffer(const te::PluginRenderContext& fc) {
    if (fc.destBuffer != nullptr)
    {
        // First handle MIDI messages
        // Handle all notes first
        if (fc.bufferForMidiMessages != nullptr)
            if (fc.bufferForMidiMessages->isAllNotesOff)
                synth.allNotesOff(NOTE_CHANNEL, true);
        // Chop the buffer in 32 sample blocks so modulation is smooth
        int todo = fc.bufferNumSamples;
        int pos = fc.bufferStartSample;

        while (todo > 0)
        {
            int thisBlock = std::min(32, todo);

            te::AudioScratchBuffer workBuffer(2, thisBlock);
            workBuffer.buffer.clear();

            juce::MidiBuffer midi;
            if (fc.bufferForMidiMessages != nullptr)
            {
                for (auto m : *fc.bufferForMidiMessages)
                {
                    int midiPos = int(m.getTimeStamp());
                    if (midiPos >= pos && midiPos < pos + thisBlock)
                        midi.addEvent(m, midiPos - pos);
                }
            }
            //Then process synth
            applyToBuffer(workBuffer.buffer, midi);

            //Copy buffer into output
            if (fc.destBuffer->getNumChannels() == 1)
            {
                fc.destBuffer->copyFrom(0, pos, workBuffer.buffer, 0, 0, thisBlock);
            }
            else
            {
                fc.destBuffer->copyFrom(0, pos, workBuffer.buffer, 0, 0, thisBlock);
                fc.destBuffer->copyFrom(1, pos, workBuffer.buffer, 1, 0, thisBlock);
            }

            //Start on next block
            todo -= thisBlock;
            pos += thisBlock;
        }
    }
    
   

    
}

void Wavetable::applyToBuffer(juce::AudioBuffer<float>&buffer, juce::MidiBuffer & midi)
{
    // update buffer params;
    // Render synth into working buffer
    synth.renderNextBlock(buffer, midi, 0, buffer.getNumSamples());

}


void Wavetable::handleMidiEvent(juce::MidiMessage msg, int sampleNumber, bool record) {
    Helpers::MessageType type = Helpers::getMidiMessageType(msg);
    LOG(Helpers::getMidiMessageDescription(msg) + "\n");
    if (type == Helpers::MessageType::Note) {
        
        
        if (record) {
            addMessageToBuffer(msg, sampleNumber);
        }
    }
}

//juce::AudioProcessorValueTreeState& Wavetable::getValueTreeState() {
//    return paramState;
//}

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

void Wavetable::setAmpAdsr(te::ExpEnvelope adsr) {
    ampAdsr = adsr;
}

te::ExpEnvelope& Wavetable::getAmpsAdsr() {
    return ampAdsr;
}