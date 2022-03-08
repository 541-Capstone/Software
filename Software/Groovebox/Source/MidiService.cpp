#include "MidiService.h"

MidiService::MidiService() {

}

MidiService::~MidiService() {

}

void MidiService::addLabelAndSetStyle(juce::Label& label)
{
    label.setFont(juce::Font(15.00f, juce::Font::plain));
    label.setJustificationType(juce::Justification::centredLeft);
    label.setEditable(false, false, false);
    label.setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label.setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    addAndMakeVisible(label);
}

void MidiService::handleIncomingMidiMessage(juce::MidiInput* /*source*/, const juce::MidiMessage& message)
{
    // This is called on the MIDI thread
    const juce::ScopedLock sl(midiMonitorLock);
    incomingMessages.add(message);
    triggerAsyncUpdate();
}

void MidiService::handleAsyncUpdate()
{
    // This is called on the message loop
    juce::Array<juce::MidiMessage> messages;

    {
        const juce::ScopedLock sl(midiMonitorLock);
        messages.swapWith(incomingMessages);
    }

    juce::String messageText;

    for (auto& m : messages)
        messageText << m.getDescription() << "\n";

    midiMonitor.insertTextAtCaret(messageText);
}

void MidiService::sendToOutputs(const juce::MidiMessage& msg)
{
    for (auto midiOutput : midiOutputs)
        if (midiOutput->outDevice.get() != nullptr)
            midiOutput->outDevice->sendMessageNow(msg);
}