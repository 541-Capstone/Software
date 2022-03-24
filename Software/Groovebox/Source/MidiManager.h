/*
 * Author: Dmitri Smith
 * File: MidiManager.h
 * Purpose: Translate MIDI messages into actions
 *
 */

#pragma once

#include <stdio.h>
#include <JuceHeader.h>
#include <functional>
#include <math.h>
#include "MidiService.h"

#include "Utilities.h"

class MidiManager : public juce::ActionListener,
	public juce::ActionBroadcaster,
	public juce::Component
{
public:
	MidiManager(int rate);
	~MidiManager();
	void setMidiBuffer(juce::MidiBuffer* buffer);
	void setSampleRate(int newRate);
	void setSampleRateFromTransport(te::TransportControl& t);
private:
	juce::MidiBuffer* midiBuffer;
	int sampleRate;
	MidiService* midiService;

	void actionListenerCallback(const juce::String& message) override;



};
