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
    
    /* create audio file */
    tracktion_engine::AudioFile audioFile {edit->engine, file};
    
    /* cast Track* to AudioTrack* */
    tracktion_engine::AudioTrack *audioTrack = (tracktion_engine::AudioTrack*)track;
    /* insert audio file as clip */
    auto clip = audioTrack->insertWaveClip(file.getFileNameWithoutExtension(), file, { {0.0, audioFile.getLength()}, 0.0 }, false);
    
    audioTrack->setName(file.getFileNameWithoutExtension());
    
    LOG("Clip added: " + clip->getName());
    
}

tracktion_engine::Edit* FileManager::getEdit() {
    return edit;
}

void FileManager::setEdit(tracktion_engine::Edit *edit) {
    this->edit = edit;
}
