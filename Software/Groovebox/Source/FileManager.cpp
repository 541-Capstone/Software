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
    
    /* cast Track* to ClipTrack* */
    tracktion_engine::AudioTrack *audiotrack = (tracktion_engine::AudioTrack*)track;
    /* insert audio file as clip */
    auto clip = audiotrack->insertWaveClip(file.getFileNameWithoutExtension(), file, { {0.0, audioFile.getLength()}, 0.0 }, false);
    
    audiotrack->setName(file.getFileNameWithoutExtension());
    
    std::cout<<"Clip added: "<<clip->getName()<<'\n';
    
}

tracktion_engine::Edit* FileManager::getEdit() {
    return edit;
}

void FileManager::setEdit(tracktion_engine::Edit *edit) {
    this->edit = edit;
}
