//
//  test.cpp
//  Groovebox - App
//
//  Created by Peter Tso on 2/3/22.
//

#include "test.hpp"

test::test(juce::Colour p_bg){
    bg = p_bg;
}

test::~test(){
    
}

void test::paint(juce::Graphics &g){
    g.setColour(bg);
    g.fillAll();
}
