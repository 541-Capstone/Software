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
    setNumberofTracks(-1);
    
    // just call fload for now
    fload();
    
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
        auto tg = tO.drawState ();
        
        /* draw using drawState and pass &g to it */
        tg (&g);
    }
}

void MainComponent::buttonClicked(juce::Button *button){

}

void MainComponent::timerCallback() {
    timeCount += isplaying ? ((double)frameInterval * 0.001) : 0;
    std::cout<<timeCount<<'\n';
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
        /* Add the buttons and make visible */
        /*
        addAndMakeVisible(play);
        addAndMakeVisible(pause);
        addAndMakeVisible(record);
        
        addAndMakeVisible(load);
         */
        viewObjects* tb = tO.getObjects();
        for (auto btn: tb->btns) {
            addAndMakeVisible(*btn);
        }
        
        tb->btns[0]->onClick = [this](void)->void {fplay();};
        tb->btns[1]->onClick = [this](void)->void {fpause();};
        tb->btns[2]->onClick = [this](void)->void {frecord();};
        
        // Pass functions for the buttons
        /*
        play.onClick = [this](void)->void{ fplay(); };
        
        pause.onClick = [this](void)->void{ fpause(); };
        
        record.onClick = [this](void)->void{ frecord(); };
        */
        // load just makes a new empty edit for now
        //load.onClick = [this](void)->void{ fload(); };
    }
}

void MainComponent::setNumberofTracks(int n){
    maxNumTracks = n;
}

void MainComponent::fload(){
    std::string filename = "\0";
    const juce::String editFilePath = editPath + filename;
    const File editFile (editFilePath);
    
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
    play.setEnabled(false);
    pause.setEnabled(false);
    record.setEnabled(false);
    
    // Hide the timeline buttons
    play.setVisible(false);
    pause.setVisible(false);
    record.setVisible(false);
}

void MainComponent::enableButtonAtCurrentState(){
    
    // if the state is at Timeline, enable and set visible all buttons
    if (WState == WindowStates::Timeline) {
        play.setEnabled(true);
        pause.setEnabled(true);
        record.setEnabled(true);
        
        play.setVisible(true);
        pause.setVisible(true);
        record.setVisible(true);
        
        return;
    }
}

void MainComponent::fplay(){
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

void MainComponent::fpause(){
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

void MainComponent::frecord(){
    LOG ("Recording\n");
}

void MainComponent::drawAudioWaveform(){
    int thumbnailSize = 512; // px
    std::unique_ptr<juce::FileChooser> chooser;
        
    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbnailCache {5};
    juce::AudioThumbnail thumbnail {thumbnailSize, formatManager, thumbnailCache};
    
}
