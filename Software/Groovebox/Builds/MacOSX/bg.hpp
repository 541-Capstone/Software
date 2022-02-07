//
//  bg.hpp
//  Groovebox - App
//
//  Created by Peter Tso on 2/1/22.
//

#ifndef bg_hpp
#define bg_hpp


#include <stdio.h>
#include <JuceHeader.h>
#include <iostream>
#include <vector>
#include <cstdarg>

class bg {
public:
    bg(std::function <void(void)> *funcs);
    
    ~bg();
    
    void paint();
    
    void resized();
    
private:
    const int arg_funcs_size = 2;
    std::vector<std::function<void(void)>> arg_funcs;
};

#endif /* bg_hpp */
