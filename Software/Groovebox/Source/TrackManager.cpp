#include "TrackManager.h"

TrackManager::TrackManager(te::Edit* edit) {
	this->edit = edit;
	trackList = edit->getTrackList();
	//Add all existing edit tracks to the 
	audioTrackList.push_back(edit->getTrackList().objects[edit->getTrackList().objects.size() - 1]);
}

TrackManager::~TrackManager() {

}

std::vector<te::Track*> TrackManager::getTrackList() {

}

void TrackManager::addAudioTrack() {

}