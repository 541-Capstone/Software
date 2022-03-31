#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent(){
    /* set the window size */
    setSize(window[0], window[1]);
    
    // set edit to nullptr for the time being
    edit = nullptr;
    isPlaying = false;
    numAudioTracks = 0;
    
    // start the timer. The frequency of the timer is also its ID
    startTimer(frameInterval, frameInterval);
    
    // set the maximum number of Audio tracks
    setMaxTracks(-1);
    
    // just call loadEdit for now
    loadEdit();

    //Set sample rate 
    sampleRate = (int)edit->getTransport().engine.getDeviceManager().getSampleRate();

    //Initialize previousSampleNumber
    inputPrevSampleNumber = 0;
    
    // call fileManager
    fileManager.setEdit(edit.get());
    Helpers::insertClipFromFile(tracktion_engine::getAudioTracks(*edit)[0], &edit->getTransport(), TESTAUDIOPATH);


    // set current track to zero (testing purposes for now)
    currentTrack = edit->getTrackList().objects[edit->getTrackList().objects.size() - 1];
    addAudioTrack((te::AudioTrack*)currentTrack);
    currentTrackIndex = 0;
    timeline.setCurrentTrackPtr (&currentTrackIndex);
    timeline.setAudioTrackList  (&audioTracks);
    /* initally, program set to TrackView*/
    setupTrackView();
     
    // Setup timeline
    timeline.setBounds(this->getBounds());
    addAndMakeVisible(timeline);
    timeline.setEdit(edit.get());
    timeline.setMainComponentPtr(this);
    
    // Setup MIDI
    inputMidiBuffer = std::make_shared<juce::MidiBuffer>();
    midiService = std::make_unique<MidiService>(sampleRate, inputMidiBuffer);
    midiService->addActionListener(this);
}

MainComponent::~MainComponent(){
}

void MainComponent::paint(juce::Graphics &g){
    g.setColour(background_color);
    g.fillAll(background_color);
    
    /* Draw the time line graphics objects */
    if (WState == WindowStates::TrackView){
        timeline.setVisible(true);
        timeline.setEnabled(true);
    }
    //midiService.paint(g);

}

void MainComponent::resized() {
    //Make this into a for loop to resize each context
    timeline.resized();
    juce::Rectangle<int> midiRect = getLocalBounds().removeFromBottom(300);
    //midiService.resize(midiRect);
}

/* button listener */
void MainComponent::buttonClicked(juce::Button *button){
    /* check to see for timeline button */
    LOG("Button Pressed\n");
    if (WState == WindowStates::TrackView) {
        timeline.onClick(button);
    }
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate){
    
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill){

    // For debugging
    if (edit->getTransport().isPlaying()) {
        LOG("Time: " << edit->getTransport().getCurrentPosition());
    }
}

void MainComponent::releaseResources(){
    
}

void MainComponent::timerCallback(int timerId) {
    if (timerId == 0)
        timeCount += isPlaying ? ((double)frameInterval * 0.001) : 0;
    //std::cout<<timeCount<<'\n';
    repaint();
}

void MainComponent::actionListenerCallback(const juce::String& message) {
    LOG("Action Callback received: " + message);
    if (message == "MIDI") {
        auto currentTime = edit->getTransport().getCurrentPosition();
        auto currentSampleNumber = (int)(currentTime * sampleRate);
        for (const auto metadata : *inputMidiBuffer)
        {
            juce::MidiMessage message = metadata.getMessage();
            auto type = Helpers::getMidiMessageType(message);
            if (type == Helpers::MessageType::Note) {
                LOG("Sending note " + juce::MidiMessage::getMidiNoteName(message.getNoteNumber(), true, true, 3) 
                + " to Track " + (juce::String)currentTrackIndex);
                //Send the midi note to the track's MIDI buffer
            }
            else if (type == Helpers::MessageType::Universal) {
                LOG("Universal Control Change: Controller: " + (juce::String)metadata.getMessage().getControllerNumber() +
                    " Value: " + (juce::String)metadata.getMessage().getControllerValue());
                //Create function to handle this
                universalControls(metadata);
                
            }
            else if (type == Helpers::MessageType::Contextual) {
                LOG("Contextual Control Change: Controller: " + (juce::String)metadata.getMessage().getControllerNumber() +
                    " Value: " + (juce::String)metadata.getMessage().getControllerValue());
                //Pass off signal to current context
                
            }
            
        }
        // Clear the input buffer
        //Possible that MIDI messages added between when callback was sent and when loop concluded causes messages to be cleared before being processed
        //inputMidiBuffer->clear(inputPrevSampleNumber, currentSampleNumber - inputPrevSampleNumber);
        inputMidiBuffer->clear();
        inputPrevSampleNumber = currentSampleNumber;
    }
    else if (message == "Contextual" || message == "Note") {

    }
    //LOG(midiManager->getMidiMessage()->getDescription());
}


/* Private functions */

void MainComponent::setupTrackView(){
    std::function<void()> playFunc = [this]()->void{
        play();
    };
    std::function<void()> pauseFunc = [this]()->void{
        pause();
    };
    std::function<void()> recordFunc = [this]()->void{
        record();
    };
    std::function<void()> nextTrackFunc = [this]()->void{
        nextTrack();
    };
    std::function<void()> prevTrackFunc = [this]()->void{
        prevTrack();
    };
    std::function<void()> addTrackFunc = [this]()->void{
        createAudioTrack();
    };
    std::function<void()> addClipFunc = [this]()->void{
        addClipToTrack();
    };
    std::vector<std::function<void()> > funcs;
    funcs.clear();
    funcs.push_back(playFunc);
    funcs.push_back(pauseFunc);
    funcs.push_back(recordFunc);
    funcs.push_back(nextTrackFunc);
    funcs.push_back(prevTrackFunc);
    funcs.push_back(addTrackFunc);
    funcs.push_back(addClipFunc);
    timeline.setupFunctions(funcs);
    return;
}

void MainComponent::setMaxTracks(int n){
    maxNumTracks = n;
}

void MainComponent::loadEdit(){
    std::string filename = "\0";
    const juce::String editFilePath = editPath + filename;
    const juce::File editFile (editFilePath);
    
    // get the edit if it exists
    if (editFile.existsAsFile()) {
        edit = (tracktion_engine::loadEditFromFile(engine, editFile));
    }
    // else create a new, empty edit if it doesn't exist
    else{
        edit = tracktion_engine::createEmptyEdit(engine, editFile);
    }
}

/**
 * These functions enable and disable functions based on state. When changing state,
 * call disableAllButtons() then use enableButtonAtCurrentState()!
 * Physical buttons should be handled separately
 */

void MainComponent::disableAllButtons(){
    // Disable timeline buttons (play, pause, record at the moment)
    
    for (auto btn : timeline.getObjects()->btns) {
        (*btn).setEnabled(false);
        (*btn).setVisible(false);
    }
}

void MainComponent::enableButtonAtCurrentState(){
    
    // if the state is at Timeline, enable and set visible all buttons
    if (WState == WindowStates::TrackView) {
        for (auto btn : timeline.getObjects()->btns) {
            (*btn).setEnabled(true);
            (*btn).setVisible(true);
        }
        
        return;
    }
}

void MainComponent::play(){
    if (edit == nullptr) {LOG("Missing edit!\n"); return; }
    // Play functions when paused and edit is loaded
    if (PState == PlayStates::Pause) {
        PState = PlayStates::Play;
        LOG("Playing\n");
        edit->getTransport().play(false);
        isPlaying = true;
    }
    else {
        LOG("Already playing\n");
    }
}

void MainComponent::pause(){
    if (edit == nullptr) {LOG("Missing edit!\n"); return; }
    isPlaying = false;
    if (PState == PlayStates::Play) {
        LOG ("Pause 1\n");
        PState = PlayStates::Pause;
        edit->getTransport().stop(false, false);
        return;
    }
    if(PState == PlayStates::Pause) {
        LOG ("Pause 2\n");
        if (edit->getTransport().isPlaying()) {
            edit->getTransport().stop(false, false);
        }
        edit->getTransport().setCurrentPosition(0.0f);
        timeCount = 0.0f;
        return;
    }
}

void MainComponent::record(){
    LOG ("Recording\n");
}

void MainComponent::createAudioTrack() {
    edit->ensureNumberOfAudioTracks(numAudioTracks + 1);
    te::AudioTrack* newTrack = te::getAudioTracks(*edit)[numAudioTracks];
    //TODO: Remove this line!
    Helpers::insertClipFromFile(newTrack, &edit->getTransport(), TESTAUDIOPATH);
    addAudioTrack(newTrack);
}

void MainComponent::addAudioTrack(te::AudioTrack *audioTrack) {
    numTracks++;
    numAudioTracks++;
    audioTracks.push_back(audioTrack);
    //waveformManager.showAudioResource(edit.get());
    //waveforms.showEdit();
    //cursor.defineCursorByRect(waveforms.getBounds());
    LOG("Added track " + (juce::String)numAudioTracks +
        "\nNumber of clips in track: " + (juce::String)audioTrack->getClips().size() + 
        "\nNumber of tracks in edit: " + (juce::String)edit->getTrackList().size());
}

void MainComponent::nextTrack() {
    if (currentTrackIndex < audioTracks.size() - 1) {
        currentTrackIndex++;
        LOG("Paged up. Current track:" + (juce::String)currentTrackIndex + "\n");
    }
}

void MainComponent::prevTrack() {
    if (currentTrackIndex > 0) {
        currentTrackIndex--;
        LOG("Paged down. Current track:" + (juce::String)currentTrackIndex + "\n");
    }
}

void MainComponent::addClipToTrack() {
    auto audioTracks = te::getAudioTracks(*edit);
    auto track = audioTracks[currentTrackIndex];
    Helpers::insertClipToTrack(track, &edit->getTransport(), TESTAUDIOPATH);
    //waveforms.showEdit();
}

void MainComponent::disableAllStates(){
    /* add states here to disable! */
    timeline.setAllComponents(false);
    timeline.setVisible(false);
    timeline.setEnabled(false);
}

/* This assumes that the type of messaegs are of type UNIVERSAL */
void MainComponent::universalControls(const juce::MidiMessageMetadata &metadata) {
    /* get the controller Value */
    const int controllerValue = metadata.getMessage().getControllerValue();
    
    /* get the message */
    const juce::MidiMessage message = metadata.getMessage();
    
    const int controllerNumber = metadata.getMessage().getControllerNumber();
    
    /* TODO: */
    /* remove later*/
    /*======================================*/
    if (controllerNumber == 1) pause();
    else if (controllerNumber == 2) play();
    else if (controllerNumber == 3) disableAllStates();
    else if (controllerNumber == 4) {
        timeline.setAllComponents(true);
        timeline.setEnabled(true);
        timeline.setVisible(true);
    }
    return;
    /*======================================*/
    
    /* Check to see if Status is Song Start or Song Stop */
    /* Pause or play */
    if (message.isMidiStop()) pause();
    else if (message.isMidiStart()) play();
    
    /*---------------------*/
    /* Functions     Value */
    /* Record            1 */
    /* Mute              2 */
    /* Solo              3 */
    /* Timeline          4 */
    /* Synth             5 */
    /* Settings          6 */
    /* Plugins           7 */
    /*---------------------*/
    
    switch (controllerValue) {
        case 1:
            LOG("Record\n");
            record();
            break;
        case 2:
            LOG("Mute\n");
            break;
        case 3:
            LOG("Solo\n");
            break;
        case 4:
            LOG("Timeline\n");
            disableAllStates();
            timeline.setEnabled(true);
            timeline.setVisible(true);
            timeline.setAllComponents(true);
            break;
        case 5:
            LOG("Synth\n");
            disableAllStates();
            break;
        case 6:
            LOG("Settings\n");
            disableAllStates();
            break;
        case 7:
            LOG("Plugins\n");
            disableAllStates();
            break;
        default:
            LOG("None called or Play/Pause called\n");
            break;
    }
    
}
