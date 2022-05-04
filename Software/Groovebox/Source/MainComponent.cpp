#include "MainComponent.h"
#define TIMELINE_IDX 0
#define SETTINGS_IDX 1
#define SYNTH_IDX    2
//==============================================================================
MainComponent::MainComponent(){
    
    /* set the window size */
    setSize(window[0], window[1]);
    
    // set edit to nullptr for the time being
    edit = nullptr;
    
    // start the timer. The frequency of the timer is also its ID
    startTimer(frameInterval, frameInterval);
    
    // set the maximum number of Audio tracks
    setMaxTracks(-1);
    
    // just call loadEdit for now
    loadEdit("");

    //Set sample rate 
    sampleRate = (int)edit->engine.getDeviceManager().getSampleRate();

    //Initialize previousSampleNumber
    inputPrevSampleNumber = 0;
    
    // call fileManager
    fileManager.setEdit(edit.get());
    //Helpers::insertClipFromFile(tracktion_engine::getAudioTracks(*edit)[0], &edit->getTransport(), TESTAUDIOPATH);

    // Initialize TrackManager
    trackManager = std::make_unique<TrackManager>(edit);

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
    setupTimeline();
    
    // Setup settings
    setting.setBounds(this->getBounds());
    addAndMakeVisible(setting);
    setting.setEdit(edit.get());

    //Setup synth
    engine.getPluginManager().createBuiltInType<Wavetable>();
    engine.getPluginManager().createBuiltInType<Delay>();
    /*if (auto synth = dynamic_cast<Wavetable*> (edit->getPluginCache().createNewPlugin(Wavetable::xmlTypeName, {}).get()))
    {
        auto fx = dynamic_cast<Wavetable*> (edit->getPluginCache().createNewPlugin(Wavetable::xmlTypeName, {}).get());
        if (auto t = trackManager->getActiveTrack()) {
            t->getTrack()->pluginList.insertPlugin(*synth, 0, nullptr);
            t->getTrack()->pluginList.insertPlugin(*fx, 1, nullptr);
            t->hasSynth = true;
        }
    }*/
    synthWindow.setTrackManager(trackManager);
    synthWindow.setEdit(edit);

    LOG("Audio device: " + engine.getDeviceManager().deviceManager.getCurrentAudioDevice()->getName());
    synthWindow.loadTrack(*(trackManager->getTrackWrapper()));
    synthWindow.setBounds(this->getBounds());
    addAndMakeVisible(synthWindow);

    //Set up effects view
    effects.setTrackManager(trackManager);
    effects.setEdit(edit);
    effects.loadTrack(*(trackManager->getTrackWrapper()));
    effects.setBounds(this->getBounds());
    addAndMakeVisible(effects);
    
    // Setup MIDI
    inputMidiBuffer = std::make_shared<juce::MidiBuffer>();
    midiService = std::make_unique<MidiService>(edit->engine, inputMidiBuffer);
    midiService->addActionListener(this);
    
    // Setup save and load functions for setting
    setupSetting();
    
    // Setup splash screen
    disableAllStates();
    WState = WindowStates::Settings;
    //setting.displaySplashScreen();
    
    // Setup timeline view
    //timeline.setEnabled(true);
    //timeline.setVisible(true);
    //timeline.setAllComponents(true);
    
    // Setup Setting view
    setting.setEnabled(true);
    setting.setVisible(true);
    setting.setAllComponents(true);
    
    currentComponent = &setting;
    
    trackManager->setActiveTrack(0);
    
    setupExample();
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
    if (WState == WindowStates::Synthesizer) {
        synthWindow.setVisible(true);
        synthWindow.setEnabled(true);
    }
    if (WState == WindowStates::Effects) {
        effects.setVisible(true);
        effects.setEnabled(true);
    }

}

void MainComponent::resized() {
    //Make this into a for loop to resize each context
    timeline.resized();
    setting.resized();
    juce::Rectangle<int> midiRect = getLocalBounds().removeFromBottom(300);
}

/* button listener */
void MainComponent::buttonClicked(juce::Button *button){
    /* check to see for timeline button */
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
        timeCount += edit->getTransport().isPlaying() ? ((double)frameInterval * 0.001) : 0;
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
    const juce::String editFilePath = filename;
    const juce::File editFile (editFilePath);

    auto& dm = engine.getDeviceManager().deviceManager;

    auto deviceManagerSetup = dm.getAudioDeviceSetup();
    deviceManagerSetup.sampleRate = 44100;
    dm.setAudioDeviceSetup(deviceManagerSetup, true);
    //dm.initialise(2, 2, 0, true, juce::String(), &deviceManagerSetup);
    LOG((juce::String)engine.getDeviceManager().getSampleRate());
    
    // get the edit if it exists
    if (editFile.existsAsFile()) {
        DBG("load edit called");
        edit = std::move(te::loadEditFromFile(engine, editFile));
    }
    // else create a new, empty edit if it doesn't exist
    else{
        edit = std::move(te::createEmptyEdit(engine, editFile));
    }
    edit->playInStopEnabled = true;

    //Set the sample rate to 44.1kHz
    /*auto deviceManagerSetup = edit->engine.getDeviceManager().deviceManager.getAudioDeviceSetup();
    deviceManagerSetup.sampleRate = 44100;
    edit->engine.getDeviceManager().deviceManager.setAudioDeviceSetup(deviceManagerSetup, true);
    LOG((juce::String)edit->engine.getDeviceManager().getSampleRate());*/
    
    /* we want the timeline and
       setting edits to point to correct location */
    timeline.setEdit(edit.get());
    trackManager = std::make_unique<TrackManager>(edit);
    timeline.setTrackManager(trackManager);
    setting.setEdit(edit.get());
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
        if (PStyle == PlayStyle::Mult) {
            auto tracks = te::getAudioTracks(*edit.get());
            for (auto track: tracks)
                track->setSolo(false);
        }
        else if (PStyle == PlayStyle::Solo){
            LOG("Playing solo");
            auto trackWrapper = trackManager->getActiveTrack();
            auto track = trackWrapper->getTrack();
            track->setSolo(true);
        }
        edit->getTransport().play(false);
    }
    else {
        LOG("Already playing\n");
    }
}

void MainComponent::pause(){
    if (edit == nullptr) {LOG("Missing edit!\n"); return; }
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
    auto& transport = edit->getTransport();
    bool wasRecording = transport.isRecording();
    Helpers::toggleRecording(transport);
    if (wasRecording) {
        PState = PlayStates::Pause;
        te::EditFileOperations(*edit).save(true, true, false);
    }
    else {
        PState = PlayStates::Record;
    }
    timeline.redrawWaveform();
}

void MainComponent::createAudioTrack() {
    trackManager->createTrack();
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
    timeline.fileBrowserState(false);
    timeline.setVisible(false);
    timeline.setEnabled(false);
    
    setting.setAllComponents(false);
    setting.setVisible(false);
    setting.setEnabled(false);

    //synthWindow.setAllComponents(false);
    synthWindow.setVisible(false);
    synthWindow.setEnabled(false);

    effects.setVisible(false);
    effects.setEnabled(false);
}

/* This assumes that the type of messaegs are of type UNIVERSAL */
void MainComponent::universalControls(const juce::MidiMessageMetadata &metadata) {
    
    /* get the message */
    const juce::MidiMessage message = metadata.getMessage();
    
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
            solo();
            break;
        case Helpers::UniversalCommands::Mute:
            mute();
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
            synthWindow.setVisible(true);
            synthWindow.setEnabled(true);
            currentComponent = &synthWindow;
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
            effects.setVisible(true);
            effects.setEnabled(true);
            currentComponent = &effects;
            break;
        case Helpers::UniversalCommands::Metronome:
            disableAllStates();
            break;
        default:
            LOG("\nInvalid Universal Command!\n");
            break;
    }
}

void MainComponent::setupSetting(){
    std::function<void(std::string)> giveLoad = [&](std::string filename)->void{
        loadEdit(filename);
    };
    setting.setLoadEditFunction(giveLoad);
    
    std::function<void(std::string)> giveSave = [&](std::string filename)->void{
      // no save for now!
        saveEdit(filename);
    };
    setting.setSaveEditFunction(giveSave);
    
    std::function<void()> giveExit = [&]()->void{
        WState = WindowStates::TrackView;
        disableAllStates();
        timeline.setAllComponents(true);
        timeline.setVisible(true);
        timeline.setEnabled(true);
    };
    setting.setExitFunction(giveExit);
    
}

void MainComponent::setupTimeline(){
    std::function<void()> changeState = [&]()->void {
        disableAllStates();
        setting.setAllComponents(true);
        setting.setVisible(true);
        setting.setEnabled(true);
    };
    timeline.setupTimelineSave(changeState);
}

void MainComponent::saveEdit(std::string filename){
    /* make sure that there is a filename present */
    if (filename.length() == 0) {
        std::cout<<"no filename!\n";
        return;
    }
    
    juce::String filenameWithPath = APATH;
    filenameWithPath = filenameWithPath + EDITPATH + filename + ".edit";
    
    juce::File savefile(filenameWithPath);
    auto res = savefile.create();
    
    // Write to file
    std::cout<<"Save to "<<savefile.getFullPathName()<<'\n';
    tracktion_engine::Edit *e = edit.get();
    tracktion_engine::EditFileOperations edo(*e);
    edo.writeToFile(savefile, false);
}

void MainComponent::solo(){
    if (PState == PlayStates::Pause) {
        switch (PStyle) {
            case PlayStyle::Solo:
                PStyle = PlayStyle::Mult;
                LOG("Un-solo'd\n");
                break;
            case PlayStyle::Mult:
                PStyle = PlayStyle::Solo;
                LOG("Changed to solo\n");
                break;
            default:
                break;
        }
    }
    else {
        LOG("Must be in pause to change\n");
    }
}

void MainComponent::mute() {
    if (PState == PlayStates::Pause) {
        auto track = trackManager->getActiveTrack()->getTrack();
        track->setMute(!track->isMuted(false));
    }
    else {
        LOG("Must be in pause to change\n");
    }
}


void MainComponent::setupExample() {
    te::TransportControl& transport = edit->getTransport();
    auto at = trackManager->getActiveTrack().get()->getTrack();
    transport.setCurrentPosition(10.5);
    Helpers::insertClipToTrack(at, &transport, juce::String(AUDIO_FILES_APATH) + "test-track-2.wav");

    trackManager->setActiveTrack(1);
    at = trackManager->getActiveTrack().get()->getTrack();
    transport.setCurrentPosition(5.25);
    Helpers::insertClipToTrack(at, &transport, juce::String(AUDIO_FILES_APATH) + "lemons-for-scale.wav");
    transport.setCurrentPosition(30.25);
    Helpers::insertClipToTrack(at, &transport, juce::String(AUDIO_FILES_APATH) + "lemons-for-scale.wav");

    trackManager->createTrack();
    trackManager->setActiveTrack(2);
    at = trackManager->getActiveTrack().get()->getTrack();
    transport.setCurrentPosition(6.3);
    Helpers::insertClipToTrack(at, &transport, juce::String(AUDIO_FILES_APATH) + "MF2_120_Drums_17.wav");
    transport.setCurrentPosition(11.3);
    Helpers::insertClipToTrack(at, &transport, juce::String(AUDIO_FILES_APATH) + "MF2_120_Drums_17.wav");
    transport.setCurrentPosition(16.3);
    Helpers::insertClipToTrack(at, &transport, juce::String(AUDIO_FILES_APATH) + "MF2_120_Drums_17.wav");
    transport.setCurrentPosition(75.3);
    Helpers::insertClipToTrack(at, &transport, juce::String(AUDIO_FILES_APATH) + "MF2_120_Drums_4.wav");
    transport.setCurrentPosition(79.5);
    Helpers::insertClipToTrack(at, &transport, juce::String(AUDIO_FILES_APATH) + "MF2_120_Drums_17.wav");
    transport.setCurrentPosition(83.7);
    Helpers::insertClipToTrack(at, &transport, juce::String(AUDIO_FILES_APATH) + "MF2_120_Drums_4.wav");

    trackManager->createTrack();
    trackManager->setActiveTrack(3);
    at = trackManager->getActiveTrack().get()->getTrack();
    transport.setCurrentPosition(50.0);
    Helpers::insertClipToTrack(at, &transport, juce::String(AUDIO_FILES_APATH) + "test-track-2.wav");
    transport.setCurrentPosition(43.3);
    Helpers::insertClipToTrack(at, &transport, juce::String(AUDIO_FILES_APATH) + "MF2_120_Drums_13.wav");
    transport.setCurrentPosition(38.3);
    Helpers::insertClipToTrack(at, &transport, juce::String(AUDIO_FILES_APATH) + "MF2_120_Drums_4.wav");

    transport.setCurrentPosition(33.65);
 
    trackManager->setActiveTrack(0);
    timeline.redrawWaveform();
    //transport.play(false);
}
