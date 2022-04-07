/*
 * Author: Dmitri Smith
 * Purpose: Wrapper class to manage Edit tracks and provide functionality to manipulate them
 * 
 */


#pragma once

#include <JuceHeader.h>
#include <functional>

#include "Utilities.h"

class TrackManager {
public:
	TrackManager(std::shared_ptr<te::Edit> edit);

	~TrackManager();

	juce::Array<te::AudioTrack*> getAudioTrackList();

	te::AudioTrack* getActiveTrack();

	int getActiveTrackIndex();

	int getNumTracks();

	void createTrack(int index);

	void createTrack();

	void addMidiToBuffer(const juce::MidiBuffer&);

	void setActiveTrack(int index);

private:
	// The edit that the manager is attached to
	std::shared_ptr<te::Edit> edit;
	//Iterable list of all tracks in the edit
	te::TrackList* trackList;
	//List of audio tracks in the Session - only includes MIDI and WAV tracks
	juce::Array<te::AudioTrack*> audioTrackList;

	// List of track midi buffers
	juce::Array<std::shared_ptr<juce::MidiBuffer>> midiBuffers;

	int activeTrackIndex;
	te::AudioTrack* activeTrack;

};
