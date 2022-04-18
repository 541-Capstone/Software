#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent(){
    /* set the window size */
    setSize(window[0], window[1]);
    
    // set edit to nullptr for the time being
    edit = nullptr;
    isPlaying = false;
    
    // start the timer. The frequency of the timer is also its ID
    startTimer(frameInterval, frameInterval);
    
    // set the maximum number of Audio tracks
    setMaxTracks(-1);
    
    // just call loadEdit for now
    loadEdit("");

    //Set sample rate 
    sampleRate = (int)edit->getTransport().engine.getDeviceManager().getSampleRate();

    //Initialize previousSampleNumber
    inputPrevSampleNumber = 0;
    
    // call fileManager
    fileManager.setEdit(edit.get());
    Helpers::insertClipFromFile(tracktion_engine::getAudioTracks(*edit)[0], &edit->getTransport(), TESTAUDIOPATH);

    // Initialize TrackManager
    trackManager = std::make_shared<TrackManager>(edit);

    // set current track to zero (testing purposes for now)
    trackManager->createTrack();
    timeline.setTrackManager(trackManager);
    /* initally, program set to TrackView*/
    setupTrackView();
    
    /*  ===========
     *  Initalize all states here!
     *
     *  ===========
     */
     
    // Setup timeline
    timeline.setBounds(this->getBounds());
    addAndMakeVisible(timeline);
    timeline.setEdit(edit.get());
    timeline.setMainComponentPtr(this);
    
    // Setup settings
    setting.setBounds(this->getBounds());
    addAndMakeVisible(setting);
    setting.setEdit(edit.get());
    
    // Setup MIDI
    inputMidiBuffer = std::make_shared<juce::MidiBuffer>();
    midiService = std::make_unique<MidiService>(sampleRate, inputMidiBuffer);
    midiService->addActionListener(this);
    
    // Setup save and load functions for setting
    setupSetting();
    
    // Setup splash screen
    disableAllStates();
    WState = WindowStates::TrackView;
    //setting.displaySplashScreen();
    currentComponent = &timeline;
    //currentComponent->contextControl({});
    //TODO: Move to Synth Context
    engine.getPluginManager().createBuiltInType<Wavetable>();
    //te::Plugin::Ptr wavetablePlugin = edit->getPluginCache().createNewPlugin(Wavetable::xmlTypeName, {});
    //=========================================================================================================
    auto& dm = edit->engine.getDeviceManager();

    for (int i = 0; i < dm.getNumMidiInDevices(); i++)
    {
        auto dev = dm.getMidiInDevice(i);
        dev->setEnabled(true);
        dev->setEndToEndEnabled(true);
    }
    // Add the midi input to track 1
    if (auto t = trackManager->getActiveTrack()->getTrack())
        if (auto dev = dm.getMidiInDevice(0))
            for (auto instance : edit->getAllInputDevices())
                if (&instance->getInputDevice() == dev)
                    instance->setTargetTrack(*t, 0, true);

    if (auto synth = dynamic_cast<Wavetable*> (edit->getPluginCache().createNewPlugin(Wavetable::xmlTypeName, {}).get()))
    {
        if (auto t = trackManager->getActiveTrack()->getTrack())
            t->pluginList.insertPlugin(*synth, 0, nullptr);
    }
    //=========================================================================================================
    //trackManager->setSynth(wavetablePlugin);
    //trackManager->getActiveTrack()->getTrack()->pluginList.insertPlugin(wavetablePlugin, 0, nullptr);
}

MainComponent::~MainComponent(){
    currentComponent = nullptr;
}

void MainComponent::paint(juce::Graphics &g){
    //g.setColour(background_color);
    //g.fillAll(background_color);
    
    /* Draw the time line graphics objects */
    if (WState == WindowStates::TrackView){
        timeline.setVisible(true);
        timeline.setEnabled(true);
    }
    if (WState == WindowStates::Settings){
        setting.setVisible(true);
        setting.setEnabled(true);
    }
    //midiService.paint(g);

}

void MainComponent::resized() {
    //Make this into a for loop to resize each context
    timeline.resized();
    setting.resized();
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
    if (message == "MIDI") {
        auto currentTime = edit->getTransport().getCurrentPosition();
        auto currentSampleNumber = (int)(currentTime * sampleRate);
        for (const juce::MidiBufferIterator::reference metadata : *inputMidiBuffer)
        {
            juce::MidiMessage message = metadata.getMessage();
            auto type = Helpers::getMidiMessageType(message);
            if (type == Helpers::MessageType::Note) {
                //LOG("Sending note " + juce::MidiMessage::getMidiNoteName(message.getNoteNumber(), true, true, 3) 
                //+ " to Track " + (juce::String)trackManager->getActiveTrackIndex());
                //Send the midi note to the track's MIDI buffer
                trackManager->addMidiToBuffer(message, currentSampleNumber, false);
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
                currentComponent->contextControl(metadata);
                
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

void MainComponent::loadEdit(std::string filename){
    const juce::String editFilePath = editPath + filename;
    const juce::File editFile (editFilePath);
    
    // get the edit if it exists
    if (editFile.existsAsFile()) {
        edit = std::move(te::loadEditFromFile(engine, editFile));
    }
    // else create a new, empty edit if it doesn't exist
    else{
        edit = std::move(te::createEmptyEdit(engine, editFile));
    }
    edit->playInStopEnabled = true;
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
    trackManager->createTrack();
    //TODO: Remove this line!
    //Helpers::insertClipFromFile(trackManager->getActiveTrack(), &edit->getTransport(), TESTAUDIOPATH);
    LOG("Added track " + (juce::String)trackManager->getAudioTrackList().size() +
        "\nNumber of clips in track: " + (juce::String)trackManager->getActiveTrack()->track->getClips().size() +
        "\nNumber of tracks in edit: " + (juce::String)trackManager->getNumTracks());
}

void MainComponent::nextTrack() {
    if (trackManager->getActiveTrackIndex() < trackManager->getNumTracks() - 1) {
        trackManager->setActiveTrack(trackManager->getActiveTrackIndex() + 1);
        LOG("Paged up. Current track:" + (juce::String)trackManager->getActiveTrackIndex() + "\n");
    }
}

void MainComponent::prevTrack() {
    if (trackManager->getActiveTrackIndex() > 0) {
        trackManager->setActiveTrack(trackManager->getActiveTrackIndex() - 1);
        LOG("Paged down. Current track:" + (juce::String)trackManager->getActiveTrackIndex() + "\n");
    }
}

void MainComponent::addClipToTrack() {
    auto track = trackManager->getActiveTrack();
    Helpers::insertClipToTrack(track->getTrack(), &edit->getTransport(), TESTAUDIOPATH);
    //waveforms.showEdit();
}

void MainComponent::disableAllStates(){
    /* add states here to disable! */
    timeline.setAllComponents(false);
    timeline.setVisible(false);
    timeline.setEnabled(false);
    
    setting.setAllComponents(false);
    setting.setVisible(false);
    setting.setEnabled(false);
}

/* This assumes that the type of messaegs are of type UNIVERSAL */
void MainComponent::universalControls(const juce::MidiMessageMetadata &metadata) {
    /* get the controller Value */
    const int controllerValue = metadata.getMessage().getControllerValue();
    
    //Helpers::ContextualCommands cmd = Helpers::getContextualCmdType(metadata.getMessage());
    
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
    
    /* utilize helper function */
    Helpers::UniversalCommands cmd = Helpers::getUniversalCmdType(message);
    
    switch (cmd) {
        case Helpers::UniversalCommands::Play:
            play();
            break;
        case Helpers::UniversalCommands::Pause:
            pause();
            break;
        case Helpers::UniversalCommands::Record:
            record();
            break;
        case Helpers::UniversalCommands::Solo:
            
            break;
        case Helpers::UniversalCommands::Timeline:
            disableAllStates();
            timeline.setEnabled(true);
            timeline.setVisible(true);
            timeline.setAllComponents(true);
            currentComponent = &timeline;
            break;
        case Helpers::UniversalCommands::Synth:
            disableAllStates();
            break;
        case Helpers::UniversalCommands::Settings:
            disableAllStates();
            setting.setEnabled(true);
            setting.setEnabled(true);
            setting.setAllComponents(true);
            currentComponent = &setting;
            break;
        case Helpers::UniversalCommands::Fx:
            disableAllStates();
            break;
        case Helpers::UniversalCommands::Metronome:
            disableAllStates();
            break;
        case Helpers::UniversalCommands::OctaveUp:
            
            break;
        case Helpers::UniversalCommands::OctaveDown:
            
            break;
        default:
            LOG("\nInvalid Universal Command!\n");
            break;
    }
}

void MainComponent::setupSetting(){
    /* change how this function works to change how
       splash screen functions */
    std::function<void()> onStartup = [&](void)->void{
        setting.toggleFirstStartToFalse();
        WState = WindowStates::Settings;
        disableAllStates();
        setting.setAllComponents(true);
        setting.setVisible(true);
        setting.setEnabled(true);
    };
    setting.setStartFunction(onStartup);
    
    std::function<void(std::string)> giveLoad = [&](std::string filename)->void{
        loadEdit(filename);
    };
    setting.setLoadEditFunction(giveLoad);
    
    std::function<void(std::string)> giveSave = [&](std::string filename)->void{
      // no save for now!
    };
    setting.setSaveEditFunction(giveSave);
    
    
}
