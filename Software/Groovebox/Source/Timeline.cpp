/*
  ==============================================================================

    Timeline.cpp
    Created: 10 Feb 2022 12:18:14pm
    Author:  Peter Tso

  ==============================================================================
*/

#include "Timeline.h"

Timeline_t::Timeline_t(int x, int y, int sc, int *numtracks) {
    paintWindow.setHeight(y);
    paintWindow.setWidth(x);
    window[0] = x;
    window[1] = y;
    windowCenter = window[0] / 2;
    
    scale = sc;
    controlImageHeightpx = controlImageHeightpx / scale;
    controlImageWidthpx = controlImageWidthpx / scale;
    
    this->numtracks = numtracks;
    
    this->currentTrack = nullptr;
    
    /* setup image buttons , not used anymore */
    setupButtonImages ();
    
    audioTrackList = new std::vector<tracktion_engine::Track*>();
    
    /* set that buttons are included in myObjects */
    myObjects.inclBtns = true;
    myObjects.inclLbls = false;
    
    /* push buttons onto myObjects button vector */
    myObjects.btns.push_back(&playBtn);
    myObjects.btns.push_back(&pauseBtn);
    myObjects.btns.push_back(&recordBtn);
    myObjects.btns.push_back(&addTrackBtn);
    myObjects.btns.push_back(&leftBtn);
    myObjects.btns.push_back(&rightBtn);
}

Timeline_t::~Timeline_t(){
    // do nothing for now!
    delete audioTrackList;
}

viewObjects* Timeline_t::getObjects() {
    return &myObjects;
}

bool Timeline_t::assignFunctionToObjects(std::initializer_list<std::function<void()>> list) {
    for (auto elem : list)
        funcs.push_back(elem);
    return true;
}

bool Timeline_t::assignFuncToBtn(juce::Button *btn, std::function<void ()> func) {
    int i = 0;
    for (auto b : myObjects.btns) {
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
    for (auto b : myObjects.btns) {
        if (i >= funcs.size () ) {
            std::cout<<"Function not implmented!\n";
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

void Timeline_t::resize() {
    int buttonHeight = 100, buttonWidth = 100, i = 0;
    juce::Rectangle<int> buttonRect = juce::Rectangle<int>(window[0], window[1]);
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
    
    
    for (juce::Button *btn : myObjects.btns) {
        buttonBox.items.add (juce::FlexItem (buttonWidth, buttonHeight, *btn));
        i++;
    }
    /*
    buttonBox.items.add(juce::FlexItem(100, 100, playBtn));
    buttonBox.items.add(juce::FlexItem(100, 100, pauseBtn));
    buttonBox.items.add(juce::FlexItem(100, 100, addTrackBtn));
    buttonBox.items.add(juce::FlexItem(100, 100, leftBtn));
    buttonBox.items.add(juce::FlexItem(100, 100, rightBtn));
    */

    textBox.items.add(juce::FlexItem(150, 10, statusLbl));
    textBox.items.add(juce::FlexItem(textRect.getWidth(), textRect.getHeight(), trackCountLbl));

    buttonBox.performLayout(buttonRect);
    textBox.performLayout(textRect);
}

std::function<void(juce::Graphics*, tracktion_engine::Edit*)> Timeline_t::drawState () {
    /*std::function<void(juce::Graphics*)> ffsetupState = [&](juce::Graphics *g)->void {
        g->setColour(bg_color);
        g->fillRect (0, 0, window[0], 40 + controlImageHeightpx);
    };
    
    return ffsetupState;*/
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    std::function<void(juce::Graphics*, tracktion_engine::Edit*)> paintFunc = [&](juce::Graphics* g, tracktion_engine::Edit* edit) -> void {
        g->fillAll(bg_color);
        
        transport = &edit->getTransport();
        

        // You can add your drawing code here!
        statusLbl.setText((juce::String) transport->getCurrentPosition(), juce::NotificationType::dontSendNotification);
        trackCountLbl.setText((juce::String)edit->getTrackList().size() + ";" + (juce::String)(*numtracks), juce::NotificationType::dontSendNotification);
        juce::Array<tracktion_engine::Track*> trackList = edit->getTrackList().objects;
        juce::String trackNames = "";//(int)trackList.size() + "\n";
        
        /* only iter when the there are tracks in audiotracklist */
        if (audioTrackList->size () > 0){
            for (auto track : trackList) {
                if (track == (*audioTrackList)[(*currentTrack)]) trackNames += "* ";
                trackNames += track->getName();
                trackNames += '\n';
            }
        }
        trackCountLbl.setText(trackNames, juce::NotificationType::dontSendNotification);
    };
    
    return paintFunc;
}

void Timeline_t::setupButtonImages() {
    return;
    /*
    juce::Colour zeroAlpha = juce::Colour::fromRGBA(0x00,
                                                    0x00,
                                                    0x00,
                                                    0x00);
    juce::Colour darken = juce::Colour::fromRGBA(0xFF,
                                                 0x00,
                                                 0x00,
                                                 128);
    
    const int y_spacing = 20;
    const int x_spacing = 20;
    const int leftmost = windowCenter - (1.5 * controlImageWidthpx) - x_spacing;
    
    playBtn.setImages(true,
                   true,
                   true,
                   playImage,
                   100,
                   zeroAlpha,
                   playHoverImage,
                   100,
                   darken,
                   playImage,
                   100,
                   zeroAlpha);

    
    playBtn.setBounds(leftmost,
                   y_spacing,
                   controlImageWidthpx,
                   controlImageHeightpx);
    
    recordBtn.setImages(true,
                    true,
                    true,
                    recordImage,
                    100,
                    zeroAlpha,
                    recordHoverImage,
                    100,
                    zeroAlpha,
                    recordImage,
                    100,
                    zeroAlpha);
    
    
    recordBtn.setBounds(leftmost + x_spacing + controlImageWidthpx,
                     y_spacing,
                     controlImageWidthpx,
                     controlImageHeightpx);
    
    pauseBtn.setImages(true,
                    true,
                    true,
                    pauseImage,
                    100,
                    zeroAlpha,
                    pauseHoverImage,
                    100,
                    zeroAlpha,
                    pauseImage,
                    100,
                    zeroAlpha);
    
    
    pauseBtn.setBounds(leftmost + (2 * x_spacing) + (2 * controlImageWidthpx),
                    y_spacing,
                    controlImageWidthpx,
                    controlImageHeightpx);
    
    */
    
}
