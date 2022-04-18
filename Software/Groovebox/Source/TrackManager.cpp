#include "TrackManager.h"

TrackManager::TrackManager(std::shared_ptr<te::Edit> edit) {
    this->edit = edit;
    trackList = &edit->getTrackList();
	//Add the default audio track to the list

    std::shared_ptr<TrackWrapper> sp (new TrackWrapper{ (te::AudioTrack*)trackList->objects[trackList->objects.size() - 1],
        std::make_unique<juce::MidiBuffer>(), nullptr, nullptr});
	audioTrackList.add(std::move(sp));
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
    std::shared_ptr<TrackWrapper> sp ( new TrackWrapper { (te::AudioTrack*)tracks->objects[edit->getTrackList().objects.size() - 1],
        std::make_unique<juce::MidiBuffer>(), nullptr, nullptr});
    audioTrackList.add(std::move(sp));
}

/*Copy the source MIDI buffer to the active track's input MIDI buffer,
* then tell the active track to check the buffer
*/
void TrackManager::addMidiToBuffer(const juce::MidiMessage& msg, int sampleNumber, bool recording) {
    auto midiBuffer = audioTrackList[activeTrackIndex];
    midiBuffer->addMidiMessage(msg, sampleNumber);
    //TODO: Should alert active plugin to check MIDI buffer
}

void TrackManager::setActiveTrack(int index) {
    jassert(index >= 0 && index < audioTrackList.size());
    activeTrackIndex = index;
}

void TrackManager::setSynth(std::unique_ptr<IPlugin> newSynth) {
    audioTrackList[activeTrackIndex]->synth = std::move(newSynth);
}


juce::String TrackManager::TrackWrapper::getName() {
    return track->getName();
}

te::AudioTrack* TrackManager::TrackWrapper::getTrack() {
    return track;
}

void TrackManager::TrackWrapper::addMidiMessage(const juce::MidiMessage& msg, int sampleNumber) {
    midiBuffer->addEvent(msg, sampleNumber);
}