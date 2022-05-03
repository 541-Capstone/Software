/*
  ==============================================================================

    Setting.cpp
    Created: 11 Apr 2022 1:31:29pm
    Author:  Peter Tso

  ==============================================================================
*/

#include "Setting.h"

Setting::Setting(){
    edit = nullptr;
    /* set buttons */
    
    /* add and make visible all buttons
       will be removed later, but
       usefull for testing */
    addAndMakeVisible(loadEdit);
    addAndMakeVisible(saveEdit);
    addAndMakeVisible(exit);
    addAndMakeVisible(Export);
    
    std::function<void()> testFunctionLoad = [&]()->void{
        loadEditFromFile();
    };
    
    std::function<void()> testFunctionSave = [&]()->void{
        saveEditToFile();
        
    };
    
    std::function<void()> testFunctionExport = [&]()->void{
        if (edit == nullptr) return;
        Helpers::renderEditToFile(edit);
    };
    
    std::function<juce::File*()> testLoadFunction = [&]()->juce::File*{
        juce::File file = fileBrowserHandler.getFileAtIndex();
        std::cout<<"testLoadFunction: filename: ";
        std::cout<<file.getFileName()<<'\n';
        return nullptr;
    };
    
    fileBrowserHandler.setAction(testLoadFunction);
    
    loadEdit.onClick = testFunctionLoad;
    saveEdit.onClick = testFunctionSave;
    Export.onClick   = testFunctionExport;
    
    /* hide all buttons */
    setAllComponents(false);
    
    //addAndMakeVisible(browser);
    addAndMakeVisible(fileBrowserHandler);
    
    std::cout<<"edit path: "<<editPath<<'\n';
    juce::File dir(editPath);
    if (dir.isDirectory()) std::cout<<"\nHello, World!\n";
    fileBrowserHandler.setDirectory(dir);
    
    //browser.addActionListener(this);
    //updateCursorLocation();
}

Setting::~Setting(){
    
}

void Setting::paint(juce::Graphics &g){
    g.setColour(cursorColor);
    g.fillRect(cursor);
}

void Setting::resized(){
    const int browserHeight = numItemsOnScreen * browser.getItemHeight();
    //browser.setBounds(cursorWidth, 0, this->getWidth()-cursorWidth, browserHeight);
    fileBrowserHandler.setBounds(0, 0, this->getWidth(), browserHeight);
    
    loadEdit.setBounds(this->getWidth()/2-bsize, this->getHeight()-bsize, bsize, bsize);
    
    saveEdit.setBounds(this->getWidth()/2, this->getHeight()-bsize, bsize, bsize);
    
    exit.setBounds(this->getWidth()/2-bsize*2, this->getHeight()-bsize, bsize, bsize);
    
    Export.setBounds(this->getWidth()/2+bsize, this->getHeight()-bsize, bsize, bsize);
}

void Setting::setEdit(te::Edit *edit) {
    this->edit = edit;
    browser.setEdit(edit);
}

/* This is where the menu is called from */
void Setting::contextControl(const juce::MidiMessageMetadata &metadata) {
    const juce::MidiMessage message = metadata.getMessage();
    Helpers::ContextualCommands cmd = Helpers::getContextualCmdType(message);
    
    /* as a check, DO NOT DO ANYTHING if browser
       component is set to not enabled */
    if (!browser.isEnabled()) {
        std::cout<<"This is an error. This should not happen.";
        std::cout<<" From Setting context!\n";
        return;
    }
    
    /* do stuff with encoders */
    if (cmd == Helpers::ContextualCommands::Encoder) {
        Helpers::Encoders enc = Helpers::getEncoderType(message);
        if (enc == Helpers::Encoders::NOTASSIGNED) return;
        
        /*
         * This will be used to increment
         * through file system.
         */
        switch (enc) {
            case Helpers::Encoders::CW1:
                /* rotate clockwise */
                browser.scrollUp(scrollAmount);
                break;
            case Helpers::Encoders::CCW1:
                /* rotate counter clockwise */
                browser.scrollDown(scrollAmount);
                break;
            default:
                /* do nothing */
                break;
        }
        
    }
    /* else it is other than encoder */
    else {
        switch (cmd) {
            case Helpers::ContextualCommands::Load:
                /* load */
                loadEditFromFile();
                break;
            case Helpers::ContextualCommands::Save:
                /* save */
                saveEditToFile();
                break;
            case Helpers::ContextualCommands::Add:
                /* export */
                Helpers::renderEditToFile(edit);
            default:
                /* do nothing */
                break;
        }
    }
}

void Setting::setAllComponents(bool state){
    loadEdit.setEnabled(state);
    loadEdit.setVisible(state);
    saveEdit.setEnabled(state);
    saveEdit.setVisible(state);
    exit.setVisible(state);
    exit.setEnabled(state);
    browser.setAllComponents(state);
    browser.setEnabled(state);
    browser.setVisible(state);
    fileBrowserHandler.setEnabled(state);
    fileBrowserHandler.setVisible(state);
    fileBrowserHandler.setAllComponent(state);
    Export.setVisible(state);
    Export.setEnabled(state);
}

void Setting::loadEditFromFile(){
    /* get the filename */
    juce::File fileToLoadFrom = fileBrowserHandler.getFileAtIndex();
    const std::string filenameFromFile = fileToLoadFrom.getFullPathName().toStdString();
    
    DBG("Save to "+filenameFromFile+"\n");
    
    /* check if valid file name (not "") */
    if (filenameFromFile == "" || filenameFromFile == " ") {
        std::cout<<"Filename not set!\n";
        return;
    }
    
    /* load into edit with function defined in MainComponent */
    loadFromFileLambda(filenameFromFile);
    
    /* now, exit the settings */
    exitAfterLoadingEditLambda();
}

void Setting::saveEditToFile(){
    /* get the time */
    juce::Time saveFileTimename(juce::Time::getCurrentTime());
    
    /* save edit with function defined in MainComponent */
    /* update the filename */
    juce::String saveFilename = saveFileTimename.toISO8601(true);
    saveFilename += "-edit";
    const std::string ss = saveFilename.toStdString();
    saveToFileLambda(ss);
    
    /* now, since we modified the folder
       we should update the file browser */
    fileBrowserHandler.updateFileBrowser();
}

void Setting::setLoadEditFunction(std::function<void (std::string)> func){
    loadFromFileLambda = func;
}

void Setting::setSaveEditFunction(std::function<void (std::string)> func){
    saveToFileLambda = func;
}

void Setting::updateCursorLocation(){
    int maxHeight  = browser.getHeight();
    cursorHeight   = browser.getItemHeight();
    
    int newYCoord = cursorHeight * cursorLocation;
    if (newYCoord < maxHeight) {
        cursor.setBounds(0, cursorLocation * cursorHeight, cursorWidth*2, cursorHeight);
    }
}

void Setting::actionListenerCallback(const juce::String &message){
    int movedAmount = message.getIntValue();
    const int newCursorLocation = movedAmount + cursorLocation;
    if (newCursorLocation < 0 || newCursorLocation >= 10) {
        return;
    }
    else {
        cursorLocation = newCursorLocation;
        updateCursorLocation();
    }
    
}

void Setting::drawCarret(){
    const double inc = 0.01;
    for (float y = 0; y < 1; y += inc) {
        for (float x = 0; x < 1; x += inc) {
            
        }
    }
}

void Setting::setExitFunction(std::function<void ()> func){
    exitAfterLoadingEditLambda = func;
    exit.onClick = func;
}
