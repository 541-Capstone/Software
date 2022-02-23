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
#include <JuceHeader.h>
#include <cstdarg>
#include "viewObjects.h"

class Timeline_t {
public:
    
    /**
     * @brief This is the constructor for Timeline_t
     *
     * @param x This is the number of pixels (width)
     * @param y This is the number of pixels (height)
     * @param sc This is the scale (usually 1)
     *
     */
    Timeline_t(int x, int y, int sc);
    
    /**
     * @brief This is the Timeline_t class destructor
     *
     */
    ~Timeline_t();
    
    /**
     * @brief This returns all objects in Timeline_t
     *
     */
    viewObjects* getObjects();
    
    /**
     * @brief executes the function at that button if possible
     *
     * @param *btn This is the button to click
     *
     */
    void onClick (juce::Button *btn);
    
    /**
     * @pre Must have same number of functions as buttons
     * @brief This varadic function assigns lambda functions to objects in Timeline_t
     *
     * @param list This is a initalizer list of functions.
     *
     */
    bool assignFunctionToObjects (std::initializer_list<std::function<void()>> list);
    
    /**
     * @brief This function assigns a single function to a certain button
     *
     * @param *btn This is the button pointer
     * @param func This is the function to assign
     * @return true Returns true when successful
     * @return false Returns false when not successful
     *
     */
    bool assignFuncToBtn (juce::Button *btn, std::function<void()> func);
    
    /**
     * @brief This returns drawing routine for Timeline_t
     *
     * @return std::function<void(juce::Graphics *g)> This is a function that takes in the juce::Graphics *g pointer. Use this function in MainComponent.
     */
    std::function<void(juce::Graphics *g)> drawState();
    
// private member variables go here
private:
    int window[2] = {0, 0};
    int windowCenter = 0;
    int scale = 0;
    int controlImageHeightpx = 150;
    int controlImageWidthpx = 300;
    
    viewObjects myObjects;
    std::vector<std::function<void()>> funcs;
    
    const std::string apath = "~/Documents/GitHub/Software/Software/Groovebox";
    
    /* path to play button */
    std::string fplayImageDefault = apath+"/TimelineControlButtonFiles/playButtonImage.png";
    std::string fpauseImageDefault = apath+"/TimelineControlButtonFiles/pauseButtonImage.png";
    std::string frecordImageDefault = fplayImageDefault;
    
    std::string fplayImageHover = fplayImageDefault;
    std::string fpauseImageHover = fplayImageDefault;
    std::string frecordImageHover = fplayImageDefault;
    
    /* buttons themselves */
    juce::ImageButton play {"play"};
    juce::ImageButton pause {"pause"};
    juce::ImageButton record {"record"};
    
    juce::Colour bg_color = juce::Colours::pink;
    
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
    
    void setupButtonImages ();
};

#endif
