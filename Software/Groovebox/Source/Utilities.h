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

#ifdef __MACH__
    #define APATH "~/Documents/GitHub/GrooveBox/Software/Software/Software/Groovebox"
    #define EDITPATH "/edit/"
    #define WAVSPATH "/wavs/"
    #define EXPORTPATH "/exports/"
    #define AUDIO_FILES_APATH "~/Documents/GitHub/GrooveBox/Software/Software/Software/Groovebox/wavs/"
	#define EDIT_PATH "/edits/"

#endif

#ifdef _WIN64
    #define APATH "C:\\Users\\Dima\\Dev\\EECS542\\Software\\Software\\Groovebox"
    #define EDITPATH "\\edit\\"
    #define WAVSPATH "\\wavs\\"
    #define EXPORTPATH "\\exports\\"
	#define AUDIO_FILES_APATH "C:\\Users\\Dima\\Dev\\EECS542\\Clips\\"
    #define EDIT_PATH "\\edits"
#endif

#define UNIVERSAL_CC_CHANNEL  16
#define CONTEXTUAL_CC_CHANNEL 17
#define NOTE_CHANNEL          1

// Encoder values
#define ENC_1_CW	7
#define ENC_1_CCW	8
#define ENC_2_CW	9
#define ENC_2_CCW	10
#define ENC_3_CW	11
#define ENC_3_CCW	12
#define ENC_4_CW	13
#define ENC_4_CCW	14
#define ENC_5_CW	15
#define ENC_5_CCW	16
#define ENC_6_CW	17
#define ENC_6_CCW	18
#define ENC_7_CW	19
#define ENC_7_CCW	20
#define ENC_8_CW	21
#define ENC_8_CCW	22

// Contextual Command Values
#define CUT         1
#define PASTE       2
#define SAVE        3
#define LOAD        4
#define ADD         5
#define DELETE      6


namespace te = tracktion_engine;

//==============================================================================
namespace Helpers
{
	void insertClipFromFile(te::AudioTrack* track, te::TransportControl* transport, juce::String filePath);
    void insertClipToTrack(te::AudioTrack* track, te::TransportControl *transport, juce::String filePath);
    void insertClipFromJuceFile(te::AudioTrack *track, te::TransportControl *transport, juce::File file);
    void renderEditToFile(te::Edit *edit);

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

    enum class Encoders {
        CW1, CCW1, CW2, CCW2,
        CW3, CCW3, CW4, CCW4,
        NOTASSIGNED
    };

    Encoders getEncoderType(const juce::MidiMessage &message);

	ContextualCommands getContextualCmdType(const juce::MidiMessage &message);

	enum class UniversalCommands { Play=0, Pause, Record, Mute, Solo, Timeline, Synth, Settings, Fx, Metronome, OctaveUp, OctaveDown };

	UniversalCommands getUniversalCmdType(const juce::MidiMessage &message);
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

