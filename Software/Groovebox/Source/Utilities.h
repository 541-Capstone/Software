/* 
 * File:    Utilities.h
 * Purpose: Provide universal utilities for the Groovebox
 * Author:  Dima Smith
 *
 */

#pragma once

/*======================================================
 * Defines
 *====================================================*/
#include <JuceHeader.h>
#include "FileManager.h"
#include "IPlugin.h"

#if __MACH__
#define LOG(x) std::cout<<x
#define TESTAUDIOPATH "~/Documents/test2.m4a"
#endif

#if _WIN64
#define TESTAUDIOPATH "C:\\Users\\Dima\\Dev\\EECS542\\Clips\\lemons-improved.wav"
#define LOG(x) DBG(x)
#endif

#define UNIVERSAL_CC_CHANNEL  16
#define CONTEXTUAL_CC_CHANNEL 17


namespace te = tracktion_engine;

//==============================================================================
namespace Helpers
{
	void insertClipFromFile(te::AudioTrack* track, te::TransportControl* transport, juce::String filePath);
    void insertClipToTrack(te::AudioTrack* track, te::TransportControl *transport, juce::String filePath);

	//===========================================================================
	/* 
	* The possible types of midi messages as we care about them
	* Note: A note signal, either NoteOn or NoteOff
	* Universal: A control signal that has the same effect no matter the context
	* Contextual: A control signal whose effect is determined by the current context
	*/
	enum class MessageType { Note, Universal, Contextual};

	MessageType getMidiMessageType(const juce::MidiMessage&);

	enum class ContextualCommands { Cut, Paste, Save, Load, Add, Delete, Encoder };

    enum class Encoders : unsigned int {
        CW1, CCW1, CW2, CCW2,
        CW3, CCW3, CW4, CCW4,
        NOTASSIGNED
    };

    Encoders getEncoderType(const juce::MidiMessage &);

	ContextualCommands getContextualCmdType(const juce::MidiMessage&);

	enum class UniversalCommands { Play=0, Pause, Record, Mute, Solo, Timeline, Synth, Settings, Fx, Metronome, OctaveUp, OctaveDown };
	UniversalCommands getUniversalCmdType(const juce::MidiMessage&);
	//===========================================================================
	/*
	* The possible types of plugins as we care about them
	* Synth: Any plugin that generates audio
	* Effect: Any plugin that passes audio through, with or without altering it
	*/
	enum class PluginType { Synth, Effect };

	PluginType getPluginType(const IPlugin&);

	juce::MidiBuffer* midiMessageArrayToMidiBuffer(const te::MidiMessageArray&);

	juce::String getMidiMessageDescription(const juce::MidiMessage&);
}

