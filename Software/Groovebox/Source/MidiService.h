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

class MidiService : public juce::Component,
	private juce::Timer,
	private juce::MidiKeyboardState::Listener,
	private juce::MidiInputCallback,
	private juce::AsyncUpdater
{
public:
	MidiService();

	~MidiService();

private:
	juce::MidiDeviceInfo deviceInfo;
	std::unique_ptr<juce::MidiInput> inDevice;
	std::unique_ptr<juce::MidiOutput> outDevice;

	//===============================================================================================
	// Variables from Juce MidiDemo
	//===============================================================================================
	juce::Label incomingMidiLabel{ "Incoming Midi Label", "Received MIDI messages:" };
	juce::Label outgoingMidiLabel{ "Outgoing Midi Label", "Play the keyboard to send MIDI messages..." };
	juce::MidiKeyboardState keyboardState;
	juce::MidiKeyboardComponent midiKeyboard;
	juce::TextEditor midiMonitor{ "MIDI Monitor" };

	juce::CriticalSection midiMonitorLock;
	juce::Array<juce::MidiMessage> incomingMessages;

	//===============================================================================================
	// Functions from Juce MidiDemo
	//===============================================================================================
	void addLabelAndSetStyle(juce::Label& label);
};
