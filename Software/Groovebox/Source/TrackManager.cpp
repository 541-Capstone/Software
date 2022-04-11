#include "TrackManager.h"

TrackManager::TrackManager(std::shared_ptr<te::Edit> edit) {
    this->edit = edit;
    trackList = &edit->getTrackList();
	//Add all existing edit tracks to the 
	audioTrackList.add((te::AudioTrack*)trackList->objects[edit->getTrackList().objects.size() - 1]);
    activeTrackIndex = 0;
    activeTrack = audioTrackList[0];
}

TrackManager::~TrackManager() {

}

juce::Array<te::AudioTrack*> TrackManager::getAudioTrackList() {
    return audioTrackList;
}

te::AudioTrack* TrackManager::getActiveTrack() {
    return activeTrack;
}

int TrackManager::getActiveTrackIndex() {
    return activeTrackIndex;
}

int TrackManager::getNumTracks() {
    return audioTrackList.size();
}

void TrackManager::createTrack(int index) {
    //TODO: Implement
}

void TrackManager::createTrack() {
    edit->ensureNumberOfAudioTracks(audioTrackList.size() + 1);
    auto tracks = &edit->getTrackList();
    audioTrackList.add((te::AudioTrack*)tracks->objects[edit->getTrackList().objects.size() - 1]);
}

/*Copy the source MIDI buffer to the active track's input MIDI buffer,
* then tell the active track to check the buffer
*/
void TrackManager::addMidiToBuffer(const juce::MidiBuffer& buffer) {
    auto midiBuffer = midiBuffers[activeTrackIndex];
    midiBuffer->addEvents(buffer, 0, buffer.getNumEvents(), 0);
    //TODO: Should alert active plugin to check MIDI buffer
}

void TrackManager::setActiveTrack(int index) {
    jassert(index >= 0 && index < audioTrackList.size());

    activeTrack = audioTrackList[index];
    activeTrackIndex = index;
}
