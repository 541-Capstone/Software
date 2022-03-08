#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent(){
    /* set the window size */
    setSize(window[0], window[1]);
    
    // set edit to nullptr for the time being
    edit = nullptr;
    isPlaying = false;
    numAudioTracks = 0;
    
    // start the timer
    startTimer(frameInterval);
    
    // set the maximum number of Audio tracks
    setMaxTracks(-1);
    
    // just call loadEdit for now
    loadEdit();

    //Set sample rate 
    
    // call fileManager
    fileManager.setEdit(edit.get());
    fileManager.loadAudioAsClip(TESTAUDIOPATH, tracktion_engine::getAudioTracks (*edit)[0]);

    // Add new track to audioTracks

    
    // set current track to zero (testing purposes for now)
    currentTrack = edit->getTrackList().objects[edit->getTrackList().objects.size() - 1];
    addAudioTrack((te::AudioTrack*)currentTrack);
    currentTrackIndex = 0;
    timeline.setCurrentTrackPtr (&currentTrackIndex);
    timeline.setAudioTrackList  (&audioTracks);
    /* initally, program set to TrackView*/
    setupTrackView(true);
    
    
}

MainComponent::~MainComponent(){
}

void MainComponent::paint(juce::Graphics &g){
    g.setColour(background_color);
    g.fillAll(background_color);
    
    /* Draw the time line graphics objects */
    if (WState == WindowStates::Timeline){
        /* get the drawState function from timeline object */
        auto tg = timeline.drawState ();
        
        /* draw using drawState and pass &g to it */
        /* call edit.get() to get raw pointer from
           std::unique_ptr */
        tg (&g, edit.get());
    }
}

void MainComponent::resized() {
    //Make this into a for loop to resize each context
    timeline.resize();
}

/* button listener */
void MainComponent::buttonClicked(juce::Button *button){
    /* check to see for timeline button */
    if (WState == WindowStates::Timeline) {
        for (auto btn : timeline.getObjects()->btns) {
            if (button == btn) {
                timeline.onClick (btn);
            }
        }
    }
}

void MainComponent::timerCallback() {
    timeCount += isPlaying ? ((double)frameInterval * 0.001) : 0;
    //std::cout<<timeCount<<'\n';
    repaint();
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


/* Private functions */

void MainComponent::setupTrackView(bool fst){
    /* add the buttons */
    if (fst) {

        /* get the viewable objects from timeline */
        viewObjects* viewObjs = timeline.getObjects();
        
        /* make the buttons visible and add listeners */
        for (auto btn: viewObjs->btns) {
            addAndMakeVisible(*btn);
            btn->addListener(this);
        }
        
        /* make the labels visible */
        for (auto lbl: viewObjs->lbls) {
            addAndMakeVisible(*lbl);
        }
        
        //TODO: Refactor this
        std::function<void()> playFun = [this](void)->void{play();};
        
        std::function<void()> pauseFun = [this](void)->void{pause();};
        
        std::function<void()> recordFun = [this](void)->void{record();};

        std::function<void()> addTrackFun = [this](void)->void {createAudioTrack(); };

        std::function<void()> prevTrackFun = [this](void)->void {prevTrack(); };

        std::function<void()> nextTrackFun = [this](void)->void {nextTrack(); };
        
        timeline.assignFunctionToObjects({playFun, pauseFun, recordFun, addTrackFun, prevTrackFun, nextTrackFun});
    }
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
    if (WState == WindowStates::Timeline) {
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
    addAudioTrack(newTrack);
}

void MainComponent::addAudioTrack(te::AudioTrack *audioTrack) {
    numTracks++;
    numAudioTracks++;
    audioTracks.push_back(audioTrack);
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

void MainComponent::drawAudioWaveform(){
    int thumbnailSize = 512; // px
    std::unique_ptr<juce::FileChooser> chooser;
    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbnailCache {5};
    juce::AudioThumbnail thumbnail {thumbnailSize, formatManager, thumbnailCache};
    
}
