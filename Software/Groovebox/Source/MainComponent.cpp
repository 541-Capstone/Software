#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent(){
    /* set the window size */
    setSize(window[0], window[1]);
    
    // set edit to nullptr for the time being
    edit = nullptr;
    isplaying = false;
    
    // start the timer
    startTimer(frameInterval);
    
    // set the maximum number of tracks
    setNumberOfTracks(-1);
    
    // just call loadEdit for now
    loadEdit();
    
    // set current track to zero (testing purposes for now)
    currentTrack = 0;
    timeline.setCurrentTrackPtr (&currentTrack);
    
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
    DBG("Button pressed");
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
    timeCount += isplaying ? ((double)frameInterval * 0.001) : 0;
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

        viewObjects* viewObjs = timeline.getObjects();
        for (auto btn: viewObjs->btns) {
            addAndMakeVisible(*btn);
            btn->addListener(this);
        }
        //TODO: Refactor this
        std::function<void()> playFun = [this](void)->void{play();};
        
        std::function<void()> pauseFun = [this](void)->void{pause();};
        
        std::function<void()> recordFun = [this](void)->void{record();};
        
        timeline.assignFunctionToObjects({playFun, pauseFun, recordFun});
    }
}

void MainComponent::setNumberOfTracks(int n){
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
        isplaying = true;
    }
    else {
        LOG("Already playing\n");
    }
}

void MainComponent::pause(){
    if (edit == nullptr) {LOG("Missing edit!\n"); return; }
    isplaying = false;
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

void MainComponent::drawAudioWaveform(){
    int thumbnailSize = 512; // px
    std::unique_ptr<juce::FileChooser> chooser;
    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbnailCache {5};
    juce::AudioThumbnail thumbnail {thumbnailSize, formatManager, thumbnailCache};
    
}
