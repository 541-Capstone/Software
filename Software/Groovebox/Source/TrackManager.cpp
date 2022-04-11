#include "TrackManager.h"

TrackManager::TrackManager(std::shared_ptr<te::Edit> edit) {
    this->edit = edit;
    trackList = &edit->getTrackList();
	//Add the default audio track to the list
    TrackWrapper tw{ (te::AudioTrack*)trackList->objects[trackList->objects.size() - 1], nullptr, nullptr };
	audioTrackList.add(std::make_shared<TrackWrapper>(tw));
    activeTrackIndex = 0;
}

TrackManager::~TrackManager() {

}

juce::Array<std::shared_ptr<TrackManager::TrackWrapper>> TrackManager::getAudioTrackList() {
    return audioTrackList;
}

std::shared_ptr<TrackManager::TrackWrapper> TrackManager::getActiveTrack() {
    return audioTrackList[activeTrackIndex];
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
    TrackWrapper tw{ (te::AudioTrack*)tracks->objects[edit->getTrackList().objects.size() - 1], nullptr, nullptr };
    audioTrackList.add(std::make_shared<TrackWrapper>(tw));
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
    activeTrackIndex = index;
}

void TrackManager::setSynth(te::Plugin* newSynth) {
    audioTrackList[activeTrackIndex]->synth = newSynth;
}


juce::String TrackManager::TrackWrapper::getName() {
    return track->getName();
}

te::AudioTrack* TrackManager::TrackWrapper::getTrack() {
    return track;
}