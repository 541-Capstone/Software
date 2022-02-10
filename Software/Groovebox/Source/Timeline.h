/*
  ==============================================================================

    Timeline.h
    Created: 10 Feb 2022 12:18:22pm
    Author:  Peter Tso

  ==============================================================================
*/

#pragma once
#ifndef timeline_h
#define timeline_h

#include <iostream>
#include <functional>
#include "viewObjects.h"

class Timeline {
public:
    
    Timeline(int x, int y);
    
    ~Timeline();
    
    viewObjects getObjects()const;
    
    std::function<void(viewObjects)> setupState()const;
    
// private member variables go here
private:
    int window[2] = {0, 0};
    
    viewObjects myObjects;
    
    const std::string apath = "~/Documents/GitHub/Software/Software/Groovebox";
    
    /* path to play button */
    std::string fplayImageDefault = apath+"/TimelineControlButtonFiles/playButtonImage.png";
    std::string fpauseImageDefault = apath+"/TimelineControlButtonFiles/pauseButtonImage.png";
    std::string frecordImageDefault = fplayImageDefault;
    
    std::string fplayImageHover = fplayImageDefault;
    std::string fpauseImageHover = fplayImageDefault;
    std::string frecordImageHover = fplayImageDefault;
    
    /* Create the images */
    
    /**
     * TODO: Here's a list of the following images
     * =-=-=-=-=-= Currently Implemented =-=-=-=-=-=
     * PlayImageDefault
     * RecordImageDefault
     *
     * =-=-=-=-=-= Need Implementation =-=-=-=-=-=
     * PauseImageDefault
     *
     *
     * The hover and onclick images can inheret same path as default
     * If doing this, then setImage with darken instead of zeroAlpha as color
     */
    
    // The playImage contains the image for the playbutton at default
    juce::Image playImage = juce::ImageFileFormat::loadFrom(juce::File(fplayImageDefault));
    
    // The pauseImage contains the image for the pauseButton at default
    juce::Image pauseImage = juce::ImageFileFormat::loadFrom(juce::File(fpauseImageDefault));
    
    // The recordImage contains the image for the recordButton at default
    juce::Image recordImage = ImageFileFormat::loadFrom(juce::File(frecordImageDefault));
    
    // Contains the image for playButton when hovering
    juce::Image playHoverImage = juce::ImageFileFormat::loadFrom(juce::File(fplayImageHover));
    
    // Contains the image for pauseButton when hovering
    juce::Image pauseHoverImage = juce::ImageFileFormat::loadFrom(juce::File(fpauseImageHover));
    
    // Contains the image for recordButton when hovering
    juce::Image recordHoverImage = juce::ImageFileFormat::loadFrom(juce::File(frecordImageHover));
};

#endif
