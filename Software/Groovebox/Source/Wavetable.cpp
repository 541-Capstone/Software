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
    synth.addVoice(new WavetableVoice());

    for (int i = 0; i < synth.getNumVoices(); i++) {
        if (auto voice = dynamic_cast<WavetableVoice*>(synth.getVoice(i))) {
            voice->prepareToPlay(sampleRate, info.blockSizeSamples);
        }
    }
}
void Wavetable::deinitialise() {}

//void Wavetable::prepareToPlay(double sampleRate, int samplesPerBlock) {}
//void Wavetable::releaseResources() {}
//#ifndef JucePlugin_PreferredChannelConfigurations
//bool Wavetable::isBusesLayoutSupported(const BusesLayout& layouts) const {
//    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
//        return false;
//
//    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
//        return false;
//
//    return true;
//}
//#endif
//
//void Wavetable::processBlock(juce::AudioBuffer<float>& ab, juce::MidiBuffer& mb) {
//    //applyToBuffer(ab, mb);
//}

//juce::AudioProcessorEditor* Wavetable::createEditor() { return new WavetableEditor(*this); }
//bool Wavetable::hasEditor() const { return true; }

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
            //synth.renderNextBlock(*fc.destBuffer, *midiBuffer, 0, fc.destBuffer->getNumSamples());
        }
    }
    
   

    
}

void Wavetable::applyToBuffer(juce::AudioBuffer<float>&buffer, juce::MidiBuffer & midi)
{
    // update buffer params;
    // Render synth into working buffer
    synth.renderNextBlock(buffer, midi, 0, buffer.getNumSamples());

}

//bool Wavetable::acceptsMidi() const
//{
//    return true;
//}
//
//bool Wavetable::producesMidi() const
//{
//    return true;
//}
//
//bool Wavetable::isMidiEffect() const
//{
//    return false;
//}
//
//double Wavetable::getTailLengthSeconds() const
//{
//    return 0.0;
//}
//
//int Wavetable::getNumPrograms()
//{
//    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
//                // so this should be at least 1, even if you're not really implementing programs.
//}
//
//int Wavetable::getCurrentProgram()
//{
//    return 0;
//}
//
//void Wavetable::setCurrentProgram(int index)
//{
//}
//
//const juce::String Wavetable::getProgramName(int index)
//{
//    return {};
//}
//
//void Wavetable::changeProgramName(int index, const juce::String& newName)
//{
//}
//
//void Wavetable::getStateInformation(juce::MemoryBlock& destData) {}
//void Wavetable::setStateInformation(const void* data, int sizeInBytes) {}


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

Wavetable::VTS::ParameterLayout Wavetable::createParameters() {
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    //Parameters: Oscillator - int
    // Attack - float
    // Decay - float
    // Sustain - float
    // Release - float

    params.push_back(std::make_unique<juce::AudioParameterInt>("OSCILLATOR", "Oscillator", 0, 2, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", juce::NormalisableRange<float> {0.1f, 1.0f, 0.1f}, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", juce::NormalisableRange<float> {0.1f, 1.0f, 0.1f}, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", juce::NormalisableRange<float> {0.1f, 1.0f, 0.1f}, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", juce::NormalisableRange<float> {0.1f, 3.0f, 0.1f}, 0.4f));

    return { params.begin(), params.end() };
}