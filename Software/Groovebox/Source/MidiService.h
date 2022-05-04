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

class MidiService :
	public  juce::Component,
	private juce::MidiInputCallback,
	public  juce::ActionBroadcaster
{
public:
	MidiService(te::Engine& engine, std::shared_ptr<juce::MidiBuffer> buffer);

	~MidiService();

	void addMessageToList(const juce::MidiMessage& message, const juce::String& source);

	void setMidiBuffer(std::shared_ptr<juce::MidiBuffer> buffer);

	void setDevicesToTrack(const te::AudioTrack& track);

private:

	//===============================================================================================
	// Variables from Juce MidiDemo
	//===============================================================================================

	//MIDI buffer to append incoming messages to
	std::shared_ptr<juce::MidiBuffer> midiBuffer;
	int sampleRate;


	te::DeviceManager& teDeviceManager;
	juce::AudioDeviceManager juceDeviceManager;

	void enableAllDevices();
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

	//automatically becomes null if the component is deleted. [must be component type]
	juce::Component::SafePointer<MidiService> owner;
	juce::MidiMessage message;
	juce::String source;
};

