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
    const juce::File file (filename);
    
    tracktion_engine::AudioFile audioFile (edit->engine, file);
    
    tracktion_engine::ClipTrack* cliptrack = (tracktion_engine::ClipTrack *)track;
    
    cliptrack->insertWaveClip(file.getFileNameWithoutExtension(), file, { {0.0, audioFile.getLength()}, 0.0 }, false);
    
}

tracktion_engine::Edit* FileManager::getEdit() {
    return edit.get();
}
