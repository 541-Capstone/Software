/*
  ==============================================================================

    Timeline.cpp
    Created: 10 Feb 2022 12:18:14pm
    Author:  Peter Tso

  ==============================================================================
*/

#include "Timeline.h"

Timeline::Timeline() {
    // Set all the nullptrs
    this->edit = nullptr;
    this->transport = nullptr;
    this->mainComponent = nullptr;
    
    funcs.clear();
    
    // Add the filebrowserHandler code. Set the directory
    addAndMakeVisible(fileBrowserHandler);
    const std::string pp = std::string(APATH) + "/wavs/";
    juce::File fbhDir(pp);
    assert (fbhDir.isDirectory());
    fileBrowserHandler.setDirectory(fbhDir);
    
    //waveform_window.setBounds(100, 100, 512, 64);
    waveform_window.setBounds(150, 100, 650, 256);
    
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
    timelineObjects.btns.push_back(&addClip);
    
    /* push labels onto timelineObjects label vector */
    timelineObjects.lbls.push_back(&statusLbl);
    timelineObjects.lbls.push_back(&trackCountLbl);
    
    /* push other objects onto timelineObjects */
    timelineObjects.waveforms = &waveforms;
    timelineObjects.cursors = &cursors;
    
    for (auto btn: timelineObjects.btns){
        addAndMakeVisible(*btn);
        btn->addListener(this);
    }
    for (auto lbl: timelineObjects.lbls){
        addAndMakeVisible(*lbl);
    }
    
    addAndMakeVisible(addSelClip);
    addSelClip.onClick = [&]()->void {
        // make sure that filebrowser is enabled
        if (usingFileBrowser) {
            juce::File ff = fileBrowserHandler.getFileAtIndex();
            auto track = trackManager->getActiveTrack();
            Helpers::insertClipFromJuceFile(track->getTrack(), &edit->getTransport(), ff);
            
            // After adding clip, revert state
            fileBrowserState(false);
            setAllComponents(true);
            redrawWaveform();
        }
    };
    // We default to not showing filebrowser
    fileBrowserState(false);
    
    
}

Timeline::~Timeline(){
    // do nothing for now!
}

viewObjects* Timeline::getObjects() {
    return &timelineObjects;
}

void Timeline::setTrackManager(std::shared_ptr<TrackManager> trackManager) {
    this->trackManager = trackManager;
}

void Timeline::resized() {
    int buttonHeight = 100, buttonWidth = 100, i = 0;
    juce::Rectangle<int> buttonRect = this->getBounds();
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
    
    fileBrowserHandler.setBounds(this->getBounds());
    addSelClip.setBounds(this->getWidth()/2-50, this->getHeight()-100, 100, 100);
}

void Timeline::paint(juce::Graphics &g) {
    cursors.defineCursorByRect(waveforms.getBounds());
    scrollable.defineScrollByRect(waveforms.getBounds());
    g.fillAll(bg_color);
    
    if (edit == nullptr) return;
    transport = &edit->getTransport();
    
    // You can add your drawing code here!
    statusLbl.setText((juce::String) transport->getCurrentPosition(), juce::NotificationType::dontSendNotification);
    trackCountLbl.setText((juce::String)trackManager->getNumTracks(), juce::NotificationType::dontSendNotification);
    juce::Array<std::shared_ptr<TrackManager::TrackWrapper>> trackList = trackManager->getAudioTrackList();
    juce::String trackNames = "";
    
    /* only iter when the there are tracks in audiotracklist */
    trackNames += "Track List: \n";
    if (trackList.size () > 0){
        for (auto track : trackList) {
            
            if (track == trackManager->getActiveTrack()) {
                trackNames += "* ";
            }
             
            trackNames += track->getName();
            trackNames += '\n';
        }
    }
    trackCountLbl.setText(trackNames, juce::NotificationType::dontSendNotification);
}

void Timeline::setEdit(te::Edit *edit) {
    this->edit = edit;
    setupWaveformDisplay();
}

void Timeline::setupWaveformDisplay(){
    waveforms.setColorRandomizer(true);
    
    waveforms.setBounds(waveform_window);
    cursors.setBounds(waveform_window);
    scrollable.setBounds(waveform_window);
    
    waveforms.setEdit(edit);
    cursors.setEdit(edit);
    scrollable.setEdit(edit);
    
    addAndMakeVisible(waveforms);
    addAndMakeVisible(cursors);
    addAndMakeVisible(scrollable);
    
    waveforms.showEdit();
}

void Timeline::timerCallback(){
    repaint();
}

void Timeline::setMainComponentPtr(juce::Component *component) {
    this->mainComponent = component;
}

void Timeline::play(){
    if (funcs.size() < 1) return;
    funcs[0]();
    redrawWaveform();
}

void Timeline::pause(){
    if (funcs.size() < 2) return;
    funcs[1]();
    redrawWaveform();
}

void Timeline::record(){
    if (funcs.size() < 3) return;
    funcs[2]();
    redrawWaveform();
}

void Timeline::nextTrack(){
    if (funcs.size() < 4) return;
    funcs[3]();
    redrawWaveform();
}

void Timeline::prevTrack(){
    if (funcs.size() < 5) return;
    funcs[4]();
    redrawWaveform();
}

void Timeline::addAudioTrack(){
    if (funcs.size() < 6) return;
    funcs[5]();
    redrawWaveform();
}

void Timeline::addClipToTrack(){
    if (funcs.size() < 7) return;
    //funcs[6]();
    usingFileBrowser = true;
    setAllComponents(false);
    fileBrowserState(true);
    
    redrawWaveform();
}

void Timeline::addActionListener(juce::ActionListener *listener) {
    
}

void Timeline::removeActionListener(juce::ActionListener *listener) {
    
}

void Timeline::removeAllActionListener() {
    
}

void Timeline::sendActionMessage(const juce::String &message) const {
    
}

void Timeline::setupFunctions(std::vector<std::function<void ()>> vec) {
    funcs = vec;
    
}

void Timeline::onClick(juce::Button *button) {
    if (button == &playBtn) play();
    if (button == &pauseBtn) pause();
    if (button == &recordBtn) record();
    if (button == &addTrackBtn) addAudioTrack();
    if (button == &leftBtn) prevTrack();
    if (button == &rightBtn) nextTrack();
    if (button == &addClip) addClipToTrack();
    return;
}

void Timeline::buttonClicked(juce::Button *button){
    this->onClick(button);
}

void Timeline::setAllComponents(bool state){
    for (auto btn: timelineObjects.btns) {
        btn->setVisible(state);
        btn->setEnabled(state);
    }
    
    for (auto lbl: timelineObjects.lbls) {
        lbl->setVisible(state);
        lbl->setEnabled(state);
    }
    
    waveforms.setVisible(state);
    waveforms.setEnabled(state);
    
    cursors.setVisible(state);
    cursors.setEnabled(state);
    
    scrollable.setVisible(state);
    scrollable.setEnabled(state);
}

void Timeline::redrawWaveform(){
    waveforms.showEdit();
    cursors.defineCursorByRect(waveforms.getBounds());
    scrollable.defineScrollByRect(waveforms.getBounds());
}

void Timeline::actionListenerCallback (const juce::String &message) {
    
}

void Timeline::contextControl(const juce::MidiMessageMetadata &metadata) {
    
    printf("\nTimeline says Hello!\n");
    
    /* get the MidiMessage from metadata */
    const juce::MidiMessage message = metadata.getMessage();
    
    Helpers::ContextualCommands cmd = Helpers::getContextualCmdType(message);
    
    // We utilize the file browser context control
    // instead
    if (usingFileBrowser) {
        assert(fileBrowserHandler.isVisible());
        juce::File file = fileBrowserHandler.contextControl(metadata);
        if (file.exists()) {
            // We add to active track
            auto track = trackManager->getActiveTrack();
            Helpers::insertClipFromJuceFile(track->getTrack(), &edit->getTransport(), file);
            
            // We re-enable all other components
            setAllComponents(true);
            fileBrowserState(false);
        } else return;
    }
    
    if (cmd == Helpers::ContextualCommands::Encoder) {
        Helpers::Encoders enc = Helpers::getEncoderType(message);
        switch (enc) {
            case Helpers::Encoders::CW1:
                
                break;
            case Helpers::Encoders::CCW1:
                
                break;
            default:
                break;
        }
    }
    else {
        switch (cmd) {
            case Helpers::ContextualCommands::Cut:
                
                break;
            case Helpers::ContextualCommands::Paste:
                
                break;
            case Helpers::ContextualCommands::Save:
                
                break;
            case Helpers::ContextualCommands::Load:
                
                break;
            case Helpers::ContextualCommands::Add:
                // We stop displaying elements,
                // and display the file browser comp.
                this->setAllComponents(false);
                fileBrowserState(true);
                break;
            case Helpers::ContextualCommands::Delete:
                
                break;
            default:
                break;
        }
    }

}

Waveforms *Timeline::getWaveformPtr(){
    return &waveforms;
}

void Timeline::fileBrowserState(bool state){
    usingFileBrowser = state;
    fileBrowserHandler.setAllComponent(state);
    fileBrowserHandler.setVisible(state);
    fileBrowserHandler.setEnabled(state);
    addSelClip.setVisible(state);
    addSelClip.setEnabled(state);
}
