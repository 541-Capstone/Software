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
    // Fill the waves vector with the possible waves we can use
    // We don't want to keep 'none' as a voice right now so we start the loop from 1
    // There are five other kinds of waves built into te::Oscillator that we can use
    for (int i = 1; i < 6; i++) {
        waves.push_back(static_cast<te::Oscillator::Waves>(i));
    }
    std::function<float()> func[] = {
        [&]() { return ampParams.attack; },
        [&]() { return ampParams.decay; },
        [&]() { return ampParams.sustain; },
        [&]() { return ampParams.release; },
        [&]() { for (int i = 0; i < waves.size(); i++) { if (waveShape == waves.at(i)) { return (float)i; } } return 0.0f; },
        [&]() { return 0.0f; },
        [&]() { return 0.0f; },
        [&]() { return 0.0f; }
    };
    contextParams.push_back({ "Attack", func[0]});
    contextParams.push_back({ "Decay", func[1]});
    contextParams.push_back({ "Sustain", func[2]});
    contextParams.push_back({ "Release", func[3]});
    contextParams.push_back({ "Wave", func[4]});
    contextParams.push_back({ "Attack", func[5]});
    contextParams.push_back({ "Attack", func[6]});
    contextParams.push_back({ "Attack", func[7]});
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

    ampParams.attack = 0.25f;
    ampParams.decay = 0.25f;
    ampParams.sustain = 0.05f;
    ampParams.release = 0.01f;


    synth.setCurrentPlaybackSampleRate(sampleRate);
    synth.addSound(new WavetableSound());
    synth.addVoice(new WavetableVoice({ ampAdsr, ampParams, waveShape }));

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


void Wavetable::contextControl(const juce::MidiMessageMetadata& metadata) {
    auto msg = metadata.getMessage();
    if (!msg.isController() || msg.getControllerNumber() != CONTEXTUAL_CC_CHANNEL) {
        return;
    }
    int value = msg.getControllerValue();
    switch (value)
    {
    case ENC_1_CW:  incrementAttack(0.1f);   break;
    case ENC_1_CCW: incrementAttack(-0.1f);  break;
    case ENC_2_CW:  incrementDecay(0.1f);    break;
    case ENC_2_CCW: incrementDecay(-0.1f);   break;
    case ENC_3_CW:  incrementSustain(0.1f);  break;
    case ENC_3_CCW: incrementSustain(-0.1f); break;
    case ENC_4_CW:  incrementRelease(0.1f);  break;
    case ENC_4_CCW: incrementRelease(-0.1f); break;
    case ENC_5_CW:  changeWave(true);        break;
    case ENC_5_CCW: changeWave(false);       break;

    default:
        return;
    }
    return;
}

IPlugin::Parameter Wavetable::getParameterValue(int index) {
    ParamMap& pm = contextParams.at(index);
    return { pm.name, pm.getValue()};
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

void Wavetable::incrementAttack(float amt) {
    if (ampParams.attack + amt > 1.0f || ampParams.attack + amt < 0.0f) { return; }
    ampParams.attack += amt;
    ampAdsr.setParameters(ampParams);
}

void Wavetable::incrementDecay(float amt) {
    if (ampParams.decay + amt > 1.0f || ampParams.decay + amt < 0.0f) { return; }
    ampParams.decay += amt;
    ampAdsr.setParameters(ampParams);
}

void Wavetable::incrementSustain(float amt) {
    if (ampParams.sustain + amt > 1.0f || ampParams.sustain + amt < 0.0f) { return; }
    ampParams.sustain += amt;
    ampAdsr.setParameters(ampParams);
}

void Wavetable::incrementRelease(float amt) {
    if (ampParams.release + amt > 1.0f || ampParams.release + amt < 0.0f) { return; }
    ampParams.release += amt;
    ampAdsr.setParameters(ampParams);
}

void Wavetable::changeWave(bool next) {
    if (next) {
        if (waveShape == waves.back()) {
            waveShape = waves.at(0);
        }
        else {
            for (int i = 0; i < waves.size() - 1; i++) {
                if (waves.at(i) == waveShape) {
                    waveShape = waves.at(i + 1);
                    break;
                }
            }
        }
    }
    else {
        if (waveShape == waves.at(0)) {
            waveShape = waves.at(waves.size() - 1);
        }
        else {
            for (int i = 1; i < waves.size(); i++) {
                if (waves.at(i) == waveShape) {
                    waveShape = waves.at(i - 1);
                    break;
                }
            }
        }
    }
}

