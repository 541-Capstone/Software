#include "MidiService.h"

MidiService::MidiService(te::Engine& engine, std::shared_ptr<juce::MidiBuffer> buffer) : teDeviceManager(engine.getDeviceManager()) {
    sampleRate = teDeviceManager.getSampleRate();
    midiBuffer = buffer;
    enableAllDevices();

    //Set up callbacks using plain juce
    auto midiInputs = juce::MidiInput::getAvailableDevices();

    for (auto dev : midiInputs) {
        if (!juceDeviceManager.isMidiInputDeviceEnabled(dev.identifier))
            juceDeviceManager.setMidiInputDeviceEnabled(dev.identifier, true);
        juceDeviceManager.addMidiInputDeviceCallback(dev.identifier, this);
    }
    
    
}

void MidiService::enableAllDevices() {
    for (int i = 0; i < teDeviceManager.getNumMidiInDevices(); i++) {
        auto dev = teDeviceManager.getMidiInDevice(i);
        dev->setEnabled(true);
        dev->setEndToEndEnabled(true);
             
    }
}


void MidiService::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) {
    postMessageToList(message, source->getName());
}

void MidiService::postMessageToList(const juce::MidiMessage& message, const juce::String& source)
{
    (new IncomingMessageCallback(this, message, source))->post();
}

void MidiService::addMessageToList(const juce::MidiMessage& message, const juce::String& source)
{
    auto time = message.getTimeStamp();

    auto hours = ((int)(time / 3600.0)) % 24;
    auto minutes = ((int)(time / 60.0)) % 60;
    auto seconds = ((int)time) % 60;
    auto millis = ((int)(time * 1000.0)) % 1000;

    auto timecode = juce::String::formatted("%02d:%02d:%02d.%03d",
        hours,
        minutes,
        seconds,
        millis);

    juce::String midiMessageString(timecode + "  -  " + message.getDescription() + " (" + source + ")");
    LOG(midiMessageString);
    if (midiBuffer != nullptr) {
        double timestamp = message.getTimeStamp();
        int sampleNumber = (int)(timestamp * sampleRate);
        midiBuffer->addEvent(message, sampleNumber);
        sendActionMessage("MIDI");
    }

}

void IncomingMessageCallback::messageCallback() {
    if (owner != nullptr)
        owner->MidiService::addMessageToList(message, source);
}

void MidiService::setMidiBuffer(std::shared_ptr<juce::MidiBuffer> buffer) {
    midiBuffer = buffer;
}

MidiService::~MidiService() {

}
