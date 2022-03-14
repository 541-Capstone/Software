/*
  ==============================================================================

    Cursors.cpp
    Created: 14 Mar 2022 4:12:44pm
    Author:  Peter Tso

  ==============================================================================
*/

#include "Cursors.h"

Cursors::Cursors(){
    edit = nullptr;
}

Cursors::~Cursors(){
    
}

void Cursors::paint(juce::Graphics &g) {
    juce::Rectangle<int>cursor = juce::Rectangle<int>();
    double playPosition = 0.0f;
    if (edit != nullptr) {
        playPosition = edit->getTransport().getCurrentPosition();
    }
    cursor.setBounds(playPosition * timeScale, 0, cursor_width, this->getHeight());
    g.setColour(cursor_color);
    g.fillRect(cursor);
}

void Cursors::resized(){
    repaint();
}

void Cursors::setEdit(te::Edit *edit) {
    this->edit = edit;
}

void Cursors::defineCursorByRect(juce::Rectangle<int> window) {
    this->setBounds(window);
}
