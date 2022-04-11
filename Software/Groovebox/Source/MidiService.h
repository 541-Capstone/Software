/*
 * MidiService.h
 * Purpose: Manage MIDI devices, 
 * catch MIDI messages and pass them off to the edit
 * Author: Dmitri Smith
 *
 */

#pragma once

#include <functional>

#include "Utilities.h"

class MidiService : public juce::Component,
	private juce::MidiInputCallback,
	public  juce::ActionBroadcaster
{
public:
	MidiService(int sampleRate, std::shared_ptr<juce::MidiBuffer> midiBuffer);

	~MidiService();

	void addMessageToList(const juce::MidiMessage& message, const juce::String& source);

    void paint(juce::Graphics &g) override;
    
    void resized() override;
    
	void resize(juce::Rectangle<int> rect);

	void setSampleRate(int newRate);
	void setSampleRateFromTransport(te::TransportControl& t);

	void setMidiBuffer(std::shared_ptr<juce::MidiBuffer> buffer);

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
	std::shared_ptr<juce::MidiBuffer> midiBuffer;
	int sampleRate;

	void setMidiInput(int index);
	void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;
	void postMessageToList(const juce::MidiMessage& message, const juce::String& source);

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

