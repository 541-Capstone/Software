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

namespace te = tracktion_engine;

//==============================================================================
namespace Helpers
{
	void insertClipFromFile(te::AudioTrack* track, te::TransportControl* transport, juce::String filePath);
}
