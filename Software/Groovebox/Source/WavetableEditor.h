/*
  ==============================================================================

    WavetableEditor.h
    Created: 17 Apr 2022 9:27:04pm
    Author:  Dima

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Wavetable.h"

//class WavetableEditor : public juce::AudioProcessorEditor {
class WavetableEditor {
public:
    WavetableEditor();
    //~WavetableEditor() override;

    ////======================================================================
    //void paint(juce::Graphics& g) override;
    //void resized() override;

private:
    /*juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;*/

    //Wavetable& wavetable;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    /*std::unique_ptr<SliderAttachment> attackAttachment;
    std::unique_ptr<SliderAttachment> decayAttachment;
    std::unique_ptr<SliderAttachment> sustainAttachment;
    std::unique_ptr<SliderAttachment> releaseAttachment;*/



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavetableEditor)

};