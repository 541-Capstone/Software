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

te::Edit* FileManager::getEdit() {
    return edit;
}

void FileManager::setEdit(te::Edit *edit) {
    this->edit = edit;
}
