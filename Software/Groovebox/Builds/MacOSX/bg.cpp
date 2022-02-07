//
//  bg.cpp
//  Groovebox - App
//
//  Created by Peter Tso on 2/1/22.
//

#include "bg.hpp"

bg::bg(std::function <void(void)> *funcs){
    arg_funcs.clear();
    // add to list
    for(int i = 0; i < arg_funcs_size; ++i)
        arg_funcs.push_back(funcs[i]);
}

bg::~bg(){
    
}

void bg::paint(){
    arg_funcs[0]();
}

void bg::resized(){
    arg_funcs[1]();
}
