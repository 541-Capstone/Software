/*
  ==============================================================================

    FileManager.cpp
    Created: 3 Mar 2022 11:21:08am
    Author:  Peter Tso

  ==============================================================================
*/

#include "FileManager.h"

FileManager::FileManager () {
}

/* do nothing for now !*/
FileManager::~FileManager () {
    
}

void FileManager::loadAudioAsClip(juce::String filename, tracktion_engine::Track *track) {
    /* get the audio file */
    juce::File file (filename);
    LOG("File path: " + filename + 
        "\nFile Type: " + typeid(file).name());
    /* create audio file */
    tracktion_engine::AudioFile audioFile {track->edit.engine, file};
    
    /* cast Track* to AudioTrack* */
    tracktion_engine::AudioTrack *audioTrack = (tracktion_engine::AudioTrack*)track;
    /* insert audio file as clip */
    const auto audioFileSampleRate{ audioFile.getSampleRate() };
    const auto sampleRate{ edit->getTransport().engine.getDeviceManager().getSampleRate() };
 
    if (audioFileSampleRate != sampleRate)
    {
        LOG("Import Audio Error:\n\tFileSampleRate= " + juce::String(audioFileSampleRate) + "\n\tEditSampleRate= " + juce::String(sampleRate));
    }

    auto clip = audioTrack->insertWaveClip(file.getFileNameWithoutExtension(), file, { {0.0, audioFile.getLength()}, 0.0 }, false);
    if (clip != nullptr) {
        clip->setAutoPitch(false);
        clip->setAutoTempo(false);
        clip->setTimeStretchMode(te::TimeStretcher::defaultMode);
        const auto start{ edit->getTransport().getCurrentPosition()};
        clip->setStart(start, true, true);
    }
    
    audioTrack->setName(file.getFileNameWithoutExtension());


    
    //LOG("Clip added: " + clip->getName());
    
}

tracktion_engine::Edit* FileManager::getEdit() {
    return edit;
}

void FileManager::setEdit(tracktion_engine::Edit *edit) {
    this->edit = edit;
}
