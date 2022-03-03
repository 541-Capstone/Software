/*
 * Author: Dmitri Smith
 * Purpose: Wrapper class to manage Edit tracks and provide functionality to manipulate them
 * 
 */


#pragma once

#include <JuceHeader.h>
#include <functional>

namespace te = tracktion_engine;

class TrackManager {
public:
	TrackManager(te::Edit* edit);

	~TrackManager();

	std::vector<te::Track*> getTrackList();

	void addAudioTrack();
private:
	// The edit that the manager is attached to
	std::unique_ptr<te::Edit> edit;
	//Iterable list of all tracks in the edit
	te::TrackList* trackList;
	//List of audio tracks in the Session - only includes MIDI and WAV tracks
	std::vector<te::Track*> audioTrackList;
	

};
