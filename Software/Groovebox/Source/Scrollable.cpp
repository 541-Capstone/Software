/*
  ==============================================================================

    Scrollable.cpp
    Created: 16 Mar 2022 6:52:05pm
    Author:  Peter Tso

  ==============================================================================
*/

#include "Scrollable.h"

Scrollable::Scrollable(){
    edit = nullptr;
}

Scrollable::~Scrollable(){
    
}

void Scrollable::paint(juce::Graphics &g) {
    if (edit == nullptr) return;
    g.setColour(cursorColor);
    juce::Rectangle<int> vertBar = juce::Rectangle<int>();
    juce::Rectangle<int> horiBar = juce::Rectangle<int>();
    
    int numTracks = getNumberOfTracks();
    double length = getLongestTrackLength();
    int right = this->getWidth() - verticalScrollBarWidth;
    int bottom = this->getHeight() - horiztonalScrollBarHeight;
    int width = getWidth();
    double relativeWidth = width * width / (double)length;
    vertBar.setBounds(right, yOffset, verticalScrollBarWidth, numTracks * trackHeight);
    horiBar.setBounds(xOffset, bottom, relativeWidth, horiztonalScrollBarHeight);
    g.fillRect(vertBar);
    g.fillRect(horiBar);
}

void Scrollable::resized(){
    repaint();
}

void Scrollable::defineScrollByRect(juce::Rectangle<int> window) {
    this->setBounds(window);
}

void Scrollable::setEdit(te::Edit *edit) {
    this->edit = edit;
}

void Scrollable::setTrackHeight(int height) {
    trackHeight = height;
}

double Scrollable::getLongestTrackLength(){
    auto tracks = te::getAudioTracks(*edit);
    double length = 0.0f;
    for (auto track: tracks){
        if (length < track->getLength()){
            length = track->getLength();
        }
    }
    return length;
}

int Scrollable::getNumberOfTracks(){
    auto tracks = te::getAudioTracks(*edit);
    return tracks.size();
}

void Scrollable::scrollXAmount(int x) {
    xOffset += x;
    repaint();
}

void Scrollable::scrollYAmount(int y) {
    yOffset += y;
    repaint();
}

