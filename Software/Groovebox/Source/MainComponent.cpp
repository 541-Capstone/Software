#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent(){
    /* set the window size */
    setSize(window[0], window[1]);
    
    // set edit to nullptr for the time being
    edit = nullptr;
    
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
    repaint();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate){
    
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill){
    
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
        
        /* fill the topbar */
        for(int x = 0; x < window[0]; ++x){
            for(int y = 0; y < 40 + controlImageHeightpx; ++y){
                timelineTopBar.setPixelAt(x, y, juce::Colour::fromRGBA(0xFF, 0xFF, 0xFF, 0x1F));
            }
        }
    }
        int y_spacing = 20;
        int x_spacing = 20;
        int leftmost = windowCenter - (1.5 * controlImageWidthpx) - x_spacing;
        
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

void MainComponent::setNumberofTracks(){
    
}

void MainComponent::fload(){
    
}

void MainComponent::fplay(){
    // Play functions when paused and edit is loaded
    if (PState == PlayStates::Pause && edit != nullptr) {
        PState = PlayStates::Play;
        LOG("Playing");
        edit->getTransport().play(false);
    }
}

void MainComponent::fpause(){
    
}

void MainComponent::frecord(){
    
}

