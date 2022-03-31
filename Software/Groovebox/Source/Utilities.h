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
}

