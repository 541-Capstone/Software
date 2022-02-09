#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent(){
    /* set the window size */
    setSize(window[0], window[1]);
    
    // set edit to nullptr for the time being
    edit = nullptr;
    
    // start the timer
    startTimer(frameInterval);
    
    // set the maximum number of tracks
    setNumberofTracks(-1);
    
    tmp = 0;
    
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
        
        g.setColour(defaultColor);
        g.fillRect(0, 0, window[0], 40 + controlImageHeightpx);
    }
}

void MainComponent::buttonClicked(juce::Button *button){

}

void MainComponent::timerCallback() {
    timeCount = timeCount + ((double)frameInterval * 0.001);
    repaint();
    if (timeCount >= 1) {
        timeCount = 0;
        tmp++;
        std::cout<<"second_eclipsed\n";
    }
    
    // Remove later, just testing for now!
    // Removes all buttons after 10 seconds
    if(tmp == 10){
        disableAllButtons();
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


/* Private functions */

void MainComponent::setupTrackView(bool fst){
    /* add the buttons */
    if (fst) {
        /* Add the buttons and make visible */
        addAndMakeVisible(play);
        addAndMakeVisible(pause);
        addAndMakeVisible(record);
        
        // Pass functions for the buttons
        play.onClick = [this](void)->void{
            fplay();
        };
        
        pause.onClick = [this](void)->void{
            fpause();
        };
        
        record.onClick = [this](void)->void{
            frecord();
        };
    }
        const int y_spacing = 20;
        const int x_spacing = 20;
        const int leftmost = windowCenter - (1.5 * controlImageWidthpx) - x_spacing;
        
        juce::Colour zeroAlpha = juce::Colour::fromRGBA(0x00, 0x00, 0x00, 0x00);
        juce::Colour darken = juce::Colour::fromRGBA(0xFF, 0x00, 0x00, 128);
         
        play.setImages(true,
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
    
        play.setBounds(leftmost, y_spacing, controlImageWidthpx, controlImageHeightpx);
        
        record.setImages(true,
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
        
        record.setBounds(leftmost + x_spacing + controlImageWidthpx, y_spacing, controlImageWidthpx, controlImageHeightpx);
        
        pause.setImages(true,
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
        
        pause.setBounds(leftmost + (2 * x_spacing) + (2 * controlImageWidthpx), y_spacing, controlImageWidthpx, controlImageHeightpx);
}

void MainComponent::setNumberofTracks(int n){
    maxNumTracks = n;
}

void MainComponent::fload(std::string filename){
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
    // Play functions when paused and edit is loaded
    if (PState == PlayStates::Pause && edit != nullptr) {
        PState = PlayStates::Play;
        LOG("Playing");
        edit->getTransport().play(false);
    }
    else{
        LOG("Missing edit!");
    }
}

void MainComponent::fpause(){
    
}

void MainComponent::frecord(){
    
}
