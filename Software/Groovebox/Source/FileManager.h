/*
  ==============================================================================

    FileManager.h
    Created: 3 Mar 2022 11:21:08am
    Author:  Peter Tso

  ==============================================================================
*/

#pragma once
#ifndef filemanager_h
#define filemanager_h

#include <stdio.h>
#include <iostream>
#include <JuceHeader.h>
#include <functional>

#include "Utilities.h"

namespace te = tracktion_engine;
class FileManager {
public:
    FileManager ();
    
    ~FileManager ();
    
    te::Edit* getEdit();
    
    void setEdit (te::Edit *edit);

private:
    te::Edit *edit;
};

#endif
