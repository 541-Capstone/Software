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
	public  juce::ActionBroadcaster
{
public:
	MidiService();

	~MidiService();

	void addMessageToList(const juce::MidiMessage& message, const juce::String& source);

    void paint(juce::Graphics &g) override;
    
    void resized() override;
    
	void resize(juce::Rectangle<int> rect);

	void setMidiBuffer(juce::MidiBuffer* buffer);

	void setSampleRate(int newRate);
	void setSampleRateFromTransport(te::TransportControl& t);

	//void changeListenerCallback(juce::ChangeBroadcaster*) override;

private:

	//===============================================================================================
	// Variables from Juce MidiDemo
	//===============================================================================================
	juce::AudioDeviceManager deviceManager;
	juce::ComboBox midiInputList;
	juce::Label midiInputListLabel;
	int lastInputIndex = 0;
	bool isAddingFromMidiInput = false;

	//MIDI buffer to append incoming messages to
	juce::MidiBuffer* midiBuffer;
	int sampleRate;

	void setMidiInput(int index);
	void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;
	void postMessageToList(const juce::MidiMessage& message, const juce::String& source);

	juce::String getMidiMessageDescription(const juce::MidiMessage& m);

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

