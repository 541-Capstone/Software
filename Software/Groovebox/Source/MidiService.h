/*
 * MidiService.h
 * Purpose: Manage MIDI devices, 
 * catch MIDI messages and pass them off to the edit
 * Author: Dmitri Smith
 *
 */

#pragma once

#include <JuceHeader.h>
#include <functional>

#include "Utilities.h"

class MidiService : public juce::Component,
	private juce::MidiInputCallback,
	private juce::MidiKeyboardState::Listener
{
public:
	MidiService();

	~MidiService();

	void addMessageToList(const juce::MidiMessage& message, const juce::String& source);

	std::function<void(juce::Graphics*, te::Edit*)> paint();

	void resize(juce::Rectangle<int> rect);

private:

	//===============================================================================================
	// Variables from Juce MidiDemo
	//===============================================================================================
	juce::AudioDeviceManager deviceManager;
	juce::ComboBox midiInputList;
	juce::Label midiInputListLabel;
	int lastInputIndex = 0;
	bool isAddingFromMidiInput = false;

	juce::MidiKeyboardState keyboardState;
	juce::MidiKeyboardComponent keyboardComponent{ keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard };

	void setMidiInput(int index);
	void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;
	void postMessageToList(const juce::MidiMessage& message, const juce::String& source);

	juce::String getMidiMessageDescription(const juce::MidiMessage& m);

	void handleNoteOn(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
	void handleNoteOff(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;

};

class IncomingMessageCallback : public juce::CallbackMessage
{
public:
	IncomingMessageCallback(MidiService* o, const juce::MidiMessage& m, const juce::String& s)
		: owner(o), message(m), source(s)
	{}

	void messageCallback() override;


	juce::Component::SafePointer<MidiService> owner;
	//automatically becomes null if the component is deleted. [must be component type]
	juce::MidiMessage message;
	juce::String source;
};

