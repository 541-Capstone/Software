//
//  test.hpp
//  Groovebox - App
//
//  Created by Peter Tso on 2/3/22.
//

#ifndef test_hpp
#define test_hpp

#include <stdio.h>
#include <JuceHeader.h>

class test : public juce::Component {
public:
    test(juce::Color c);
    
    ~test() override;
    
    void paint(juce::Graphics &g) override;
    
private:
    juce::Colour bg = juce::Colours::white;
};

#endif /* test_hpp */
