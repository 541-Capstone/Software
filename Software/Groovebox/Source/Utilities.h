/* 
 * File:    Utilities.h
 * Purpose: Provide universal utilities for the Groovebox
 * Author:  Dima Smith
 *
 */

#pragma once

#include <JuceHeader.h>
#include "FileManager.h"

#if __MACH__
#define LOG(x) std::cout<<x
#else
#define LOG(x) DBG(x)
#endif

#ifdef __MACH__
#define TESTAUDIOPATH "~/Documents/test2.m4a"
#endif

#ifdef _WIN64
#define TESTAUDIOPATH "C:\\Users\\Dima\\Dev\\EECS542\\Clips\\lemons-improved.wav"
#endif

namespace te = tracktion_engine;

//==============================================================================
namespace Helpers
{
	void insertClipFromFile(te::AudioTrack* track, te::TransportControl* transport, juce::String filePath);
    void insertClipToTrack(te::AudioTrack* track, te::TransportControl *transport, juce::String filePath);
}
