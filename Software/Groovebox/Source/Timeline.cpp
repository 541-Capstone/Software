/*
  ==============================================================================

    Timeline.cpp
    Created: 10 Feb 2022 12:18:14pm
    Author:  Peter Tso

  ==============================================================================
*/

#include "Timeline.h"

Timeline_t::Timeline_t(int x, int y, int sc, int *numTracks) {
    paintWindow.setHeight(y);
    paintWindow.setWidth(x);
    edit = nullptr;
    
    scale = sc;
    controlImageHeightpx = controlImageHeightpx / scale;
    controlImageWidthpx = controlImageWidthpx / scale;
    
    this->numTracks = numTracks;
    
    this->currentTrack = nullptr;
    
    audioTrackList = nullptr;/*new std::vector<te::Track*>();*/
    
    /* set that buttons are included in timelineObjects */
    timelineObjects.inclBtns = true;
    timelineObjects.inclLbls = true;
    
    /* push buttons onto timelineObjects button vector */
    timelineObjects.btns.push_back(&playBtn);
    timelineObjects.btns.push_back(&pauseBtn);
    timelineObjects.btns.push_back(&recordBtn);
    timelineObjects.btns.push_back(&addTrackBtn);
    timelineObjects.btns.push_back(&leftBtn);
    timelineObjects.btns.push_back(&rightBtn);
    
    /* push labels onto timelineObjects label vector */
    timelineObjects.lbls.push_back(&statusLbl);
    timelineObjects.lbls.push_back(&trackCountLbl);
    
    /* testing flags */
    draw_once_for_testing = true;
}

Timeline_t::~Timeline_t(){
    // do nothing for now!
    //delete audioTrackList;
}

viewObjects* Timeline_t::getObjects() {
    return &timelineObjects;
}

bool Timeline_t::assignFunctionToObjects(std::initializer_list<std::function<void()>> list) {
    for (auto elem : list)
        funcs.push_back(elem);
    return true;
}

bool Timeline_t::assignFuncToBtn(juce::Button *btn, std::function<void ()> func) {
    int i = 0;
    for (auto b : timelineObjects.btns) {
        if (b == btn) {
            funcs[i] = func;
            return true;
        }
        i++;
    }
    return false;
 }

void Timeline_t::onClick (juce::Button *btn) {
    int i = 0;
    for (auto b : timelineObjects.btns) {
        if (i >= funcs.size () ) {
           LOG("Function not implmented!");
            return;
        }
        if (b == btn) {
            funcs[i]();
            return;
        }
        i++;
    }
}

void Timeline_t::setCurrentTrackPtr
 (int *currentTrack) {
    this->currentTrack = currentTrack;
}

void Timeline_t::setAudioTrackList(std::vector<te::Track*> *newTracks) {
    audioTrackList = newTracks;
}

void Timeline_t::resized() {
    int buttonHeight = 100, buttonWidth = 100, i = 0;
    juce::Rectangle<int> buttonRect = juce::Rectangle<int>(paintWindow);
    juce::Rectangle<int> textRect = buttonRect.removeFromBottom(buttonRect.getHeight() - 150);

    juce::FlexBox buttonBox{ juce::FlexBox::Direction::row,
                             juce::FlexBox::Wrap::noWrap,
                             juce::FlexBox::AlignContent::flexStart,
                             juce::FlexBox::AlignItems::flexStart,
                             juce::FlexBox::JustifyContent::center };

    juce::FlexBox textBox{ juce::FlexBox::Direction::column,
                           juce::FlexBox::Wrap::noWrap,
                           juce::FlexBox::AlignContent::flexStart,
                           juce::FlexBox::AlignItems::flexStart,
                           juce::FlexBox::JustifyContent::flexEnd
    };
    
    
    for (juce::Button *btn : timelineObjects.btns) {
        buttonBox.items.add (juce::FlexItem (buttonWidth, buttonHeight, *btn));
        i++;
    }

    textBox.items.add(juce::FlexItem(150, 10, statusLbl));
    textBox.items.add(juce::FlexItem(textRect.getWidth(), textRect.getHeight(), trackCountLbl));

    buttonBox.performLayout(buttonRect);
    textRect.setY(100);
    textBox.performLayout(textRect);
}

void Timeline_t::paint(juce::Graphics &g) {
    g.fillAll(bg_color);
    
    transport = &edit->getTransport();
    
    // You can add your drawing code here!
    statusLbl.setText((juce::String) transport->getCurrentPosition(), juce::NotificationType::dontSendNotification);
    trackCountLbl.setText((juce::String)edit->getTrackList().size() + ";" + (juce::String)(*numTracks), juce::NotificationType::dontSendNotification);
    juce::Array<te::Track*> trackList = edit->getTrackList().objects;
    juce::String trackNames = "";
    
    /* only iter when the there are tracks in audiotracklist */
    trackNames += "TrackList: \n";
    if (trackList.size () > 0){
        for (auto track : trackList) {
            
            if (audioTrackList != nullptr && audioTrackList->size() > *currentTrack) {
                if (track == (*audioTrackList)[(*currentTrack)]) {
                    trackNames += "* ";
                }
            }
             
            trackNames += track->getName();
            trackNames += '\n';
        }
    }
    trackCountLbl.setText(trackNames, juce::NotificationType::dontSendNotification);
}

/* do nothing for now */
void Timeline_t::setupButtonImages() {
    return;
}

void Timeline_t::setEdit(te::Edit *edit) {
    this->edit = edit;
}
