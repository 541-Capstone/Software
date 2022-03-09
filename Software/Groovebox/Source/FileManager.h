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


#ifdef __MACH__
#define TESTAUDIOPATH "~/Documents/test.m4a"
#endif

#ifdef _WIN64
#define TESTAUDIOPATH "C:\\Users\\Dima\\Dev\\EECS542\\Clips\\lemons-improved.wav"
#endif

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
