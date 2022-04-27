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
	struct TrackWrapper {
		te::AudioTrack* track;
		bool hasSynth, hasEffects;
		juce::String getName();
		te::AudioTrack* getTrack();
	};

	TrackManager(std::shared_ptr<te::Edit> edit);

	~TrackManager();

	juce::Array<std::shared_ptr<TrackWrapper>> getAudioTrackList();

	std::shared_ptr<TrackWrapper> getActiveTrack();

	int getActiveTrackIndex();

	int getNumTracks();

	void createTrack(int index);

	void createTrack();

	void addMidiToBuffer(const juce::MidiMessage&, int sampleNumber, bool recording);

	void setActiveTrack(int index);

	std::shared_ptr<TrackWrapper> getTrackWrapper();

private:
	// The edit that the manager is attached to
	std::shared_ptr<te::Edit> edit;
	//Iterable list of all tracks in the edit
	te::TrackList* trackList;
	//List of audio tracks in the Session - only includes MIDI and WAV tracks
	juce::Array<std::shared_ptr<TrackWrapper>> audioTrackList;

	int activeTrackIndex;

	//Plugins on a track

};