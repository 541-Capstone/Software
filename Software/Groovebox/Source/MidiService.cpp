#include "MidiService.h"

MidiService::MidiService() {
    addAndMakeVisible(midiInputList);
    midiInputList.setTextWhenNoChoicesAvailable("No MIDI Inputs Enabled");
    auto midiInputs = juce::MidiInput::getAvailableDevices();

    juce::StringArray midiInputNames;

    for (auto input : midiInputs) {
        midiInputNames.add(input.name);
        LOG("MIDI Device detected: " + input.name);
    }

    midiInputList.addItemList(midiInputNames, 1);
    midiInputList.onChange = [this] { setMidiInput(midiInputList.getSelectedItemIndex()); };

    //// find the first enabled device and use that by default
    //for (auto input : midiInputs)
    //{
    //    if (deviceManager.isMidiInputDeviceEnabled(input.identifier))
    //    {
    //        setMidiInput(midiInputs.indexOf(input));
    //        /*break;*/
    //    }
    //}

    //// if no enabled devices were found just use the first one in the list
    //if (midiInputList.getSelectedId() == 0)
    //    setMidiInput(0);
    LOG(midiInputs[1].name);
    setMidiInput(1);


    midiInputList.onChange = [this] { setMidiInput(midiInputList.getSelectedItemIndex()); };

    addAndMakeVisible(keyboardComponent);
    keyboardState.addListener(this);
}

void MidiService::setMidiInput(int index)
{
    auto list = juce::MidiInput::getAvailableDevices();

    deviceManager.removeMidiInputDeviceCallback(list[lastInputIndex].identifier, this);

    auto newInput = list[index];

    if (!deviceManager.isMidiInputDeviceEnabled(newInput.identifier))
        deviceManager.setMidiInputDeviceEnabled(newInput.identifier, true);

    deviceManager.addMidiInputDeviceCallback(newInput.identifier, this);
    midiInputList.setSelectedId(index + 1, juce::dontSendNotification);

    lastInputIndex = index;
}

void MidiService::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) {
    const juce::ScopedValueSetter<bool> scopedInputFlag(isAddingFromMidiInput, true);
    keyboardState.processNextMidiEvent(message);
    //LOG("MIDI Event occured; message: " + message.getDescription());
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

    //auto description = getMidiMessageDescription(message);

    juce::String midiMessageString(timecode + "  -  " + message.getDescription() + " (" + source + ")"); // [7]
    LOG(midiMessageString);
}

void MidiService::handleNoteOn(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) {
    if (!isAddingFromMidiInput)
    {
        auto m = juce::MidiMessage::noteOn(midiChannel, midiNoteNumber, velocity);
        m.setTimeStamp(juce::Time::getMillisecondCounterHiRes() * 0.001);
        postMessageToList(m, "On-Screen Keyboard");
    }
}

void MidiService::handleNoteOff(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) {
    if (!isAddingFromMidiInput)
    {
        auto m = juce::MidiMessage::noteOff(midiChannel, midiNoteNumber);
        m.setTimeStamp(juce::Time::getMillisecondCounterHiRes() * 0.001);
        postMessageToList(m, "On-Screen Keyboard");
    }
}


juce::String MidiService::getMidiMessageDescription(const juce::MidiMessage& m)
{
    if (m.isNoteOn())           return "Note on " + juce::MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3);
    if (m.isNoteOff())          return "Note off " + juce::MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3);
    if (m.isProgramChange())    return "Program change " + juce::String(m.getProgramChangeNumber());
    if (m.isPitchWheel())       return "Pitch wheel " + juce::String(m.getPitchWheelValue());
    if (m.isAftertouch())       return "After touch " + juce::MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3) + ": " + juce::String(m.getAfterTouchValue());
    if (m.isChannelPressure())  return "Channel pressure " + juce::String(m.getChannelPressureValue());
    if (m.isAllNotesOff())      return "All notes off";
    if (m.isAllSoundOff())      return "All sound off";
    if (m.isMetaEvent())        return "Meta event";

    if (m.isController())
    {
        juce::String name(juce::MidiMessage::getControllerName(m.getControllerNumber()));

        if (name.isEmpty())
            name = "[" + juce::String(m.getControllerNumber()) + "]";

        return "Controller " + name + ": " + juce::String(m.getControllerValue());
    }

    return juce::String::toHexString(m.getRawData(), m.getRawDataSize());
}

void MidiService::paint(juce::Graphics &g) {
    g.fillAll(juce::Colours::grey);
}

void MidiService::resized(){
    repaint();
}

void MidiService::resize(juce::Rectangle<int> rect) {
    juce::Rectangle<int> area = rect;
    /*
    * juce::AudioDeviceManager deviceManager;
	juce::ComboBox midiInputList;
	juce::Label midiInputListLabel;
    juce::MidiKeyboardComponent keyboardComponent
    */

    juce::FlexBox renderBox{ juce::FlexBox::Direction::column,
                           juce::FlexBox::Wrap::noWrap,
                           juce::FlexBox::AlignContent::flexStart,
                           juce::FlexBox::AlignItems::flexStart,
                           juce::FlexBox::JustifyContent::flexEnd
    };

    renderBox.items.add(midiInputListLabel); 
    renderBox.items.add(midiInputList);
    renderBox.items.add(keyboardComponent);
    renderBox.performLayout(area);




}

void IncomingMessageCallback::messageCallback() {
    if (owner != nullptr)
        owner->MidiService::addMessageToList(message, source);
}

MidiService::~MidiService() {

}
