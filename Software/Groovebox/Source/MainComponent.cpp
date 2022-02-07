#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent(){
    /* set the window size */
    setSize(window[0], window[1]);
    
    /* initally, program set to TrackView*/
    setupTrackView(true);
}

MainComponent::~MainComponent(){
}

void MainComponent::paint(juce::Graphics &g){
    g.fillAll(background_color);
}

void MainComponent::buttonClicked(juce::Button *button){

}

void MainComponent::timerCallback() {
    
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
        
        
        /* Draw the triange for the playImage */
        for(float i = 0; i < 1; i = i + 0.001){
            for(float j = 0; j < 1; j = j + 0.001){
                if(i >= 0.2 && i <= 0.8 && (-0.5 * i + 0.9) > j && (j) >= (0.5 * i + 0.1)){
                    playImage.setPixelAt(500 * i, 500 * j, juce::Colours::red);
                    playHoverImage.setPixelAt(500 * i, 500 * j, juce::Colours::darkred);
                }
                else{
                    playImage.setPixelAt(500 * i, 500 * j, juce::Colours::white);
                    playHoverImage.setPixelAt(500 * i, 500 * j, juce::Colours::white);
                }
            }
        }
        
        /* Draw a circle for the record image */
        float i2 = 0.0f, j2 = 0.0f, r = 0.35;
        float r2 = r * r;
        for(float i = 0; i < 1; i = i + 0.001){
            for(float j = 0; j < 1; j = j + 0.001){
                i2 = (i - 0.5) * (i - 0.5);
                j2 = (j - 0.5) * (j - 0.5);
                if(i2 + j2 < r2){
                    recordImage.setPixelAt(500 * i, 500 * j, juce::Colours::red);
                    recordHoverImage.setPixelAt(500 * i, 500 * j, juce::Colours::darkred);
                }
                else{
                    recordImage.setPixelAt(500 * i, 500 * j, juce::Colours::white);
                    recordHoverImage.setPixelAt(500 * i, 500 * j, juce::Colours::white);
                }
            }
        }
        
        /* Draw two vertical lines for the pause image */
        bool b1 = false;
        bool b2 = false;
        for(float i = 0; i < 1; i = i + 0.001){
            for(float j = 0; j < 1; j = j + 0.001){
                b1 = i >= 0.15 && i <= 0.35;
                b2 = i >= 0.65 && i <= 0.85;
                if((b1 || b2) && 0.8 > j && j > 0.2){
                    pauseImage.setPixelAt(500 * i, 500 * j, juce::Colours::red);
                    pauseHoverImage.setPixelAt(500 * i, 500 * j, juce::Colours::darkred);
                }
                else{
                    pauseImage.setPixelAt(500 * i, 500 * j, juce::Colours::white);
                    pauseHoverImage.setPixelAt(500 * i, 500 * j, juce::Colours::white);

                }
            }
        }
        
        juce::Colour zeroAlpha = juce::Colour::fromRGBA(0x00, 0x00, 0x00, 0x00);
         
        play.setImages(true,
                       true,
                       true,
                       playImage,
                       100,
                       zeroAlpha,
                       playHoverImage,
                       100,
                       zeroAlpha,
                       playImage,
                       100,
                       zeroAlpha);
    
        play.setBounds(10, 10, 100, 100);
        
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
        
        record.setBounds(10 + 100 + 10, 10, 100, 100);
        
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
        
        pause.setBounds(10 + 100 + 10 + 100 + 10, 10, 100, 100);
    }
}

void MainComponent::setNumberofTracks(){
    
}

void MainComponent::load(){
    
}

