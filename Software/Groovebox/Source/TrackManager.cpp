#include "TrackManager.h"

TrackManager::TrackManager(std::shared_ptr<te::Edit> edit) {
    this->edit = edit;
    trackList = &edit->getTrackList();
	//Add the default audio track to the list

    std::shared_ptr<TrackWrapper> sp (new TrackWrapper{ (te::AudioTrack*)trackList->objects[trackList->objects.size() - 1],
        false, false});
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
        false,
        false});
    audioTrackList.add(std::move(sp));
}

/*Copy the source MIDI buffer to the active track's input MIDI buffer,
* then tell the active track to check the buffer
*/
void TrackManager::addMidiToBuffer(const juce::MidiMessage& msg, int sampleNumber, bool recording) {
    auto midiBuffer = audioTrackList[activeTrackIndex];
    //TODO: Should alert active plugin to check MIDI buffer
}

void TrackManager::setActiveTrack(int index) {
    //Check that the index is valid
    jassert(index >= 0 && index < audioTrackList.size());

    //Move active index
    activeTrackIndex = index;

    // Add the midi input devices to the new track
    auto& deviceManager = edit->engine.getDeviceManager();
    const auto& track = getActiveTrack()->getTrack();
    for (int i = 0; i < deviceManager.getNumInputDevices(); i++)
        if (auto dev = deviceManager.getMidiInDevice(i))
            for (auto instance : edit->getAllInputDevices()) {
                    instance->clearFromTracks();
                    instance->setTargetTrack(*track, 0, true);
                    instance->setRecordingEnabled(*track, true);
                }

    edit->restartPlayback();
}

std::shared_ptr<TrackManager::TrackWrapper> TrackManager::getTrackWrapper() {
    return audioTrackList[activeTrackIndex];
}

juce::String TrackManager::TrackWrapper::getName() {
    return track->getName();
}

te::AudioTrack* TrackManager::TrackWrapper::getTrack() {
    return track;
}