#include "Utilities.h"

void Helpers::insertClipFromFile(te::AudioTrack* track, te::TransportControl* transport, juce::String filePath) {
    /* get the audio file */
    juce::File file(filePath);
    LOG("File path: " + filePath +
        "\nFile Type: " + typeid(file).name());
    /* create audio file */
    tracktion_engine::AudioFile audioFile{ track->edit.engine, file };

    /* insert audio file as clip */
    const double audioFileSampleRate{ audioFile.getSampleRate() };
    const double sampleRate{ transport->engine.getDeviceManager().getSampleRate() };

    if (audioFileSampleRate != sampleRate)
    {
        LOG("Import Audio Error:\n\tFileSampleRate= " + juce::String(audioFileSampleRate) +
            "\n\tEditSampleRate= " + juce::String(sampleRate));
        return;
    }

    double playheadPosition = transport->getCurrentPosition();

    te::ClipPosition position{ {playheadPosition, audioFile.getLength() + playheadPosition}, 0.0 };

    te::WaveAudioClip::Ptr clip = track->insertWaveClip(file.getFileNameWithoutExtension(), file, position, false);
    if (clip != nullptr) {
        clip->setAutoPitch(false);
        clip->setAutoTempo(false);
        clip->setTimeStretchMode(te::TimeStretcher::defaultMode);
        const double start{ playheadPosition };
        clip->setStart(start, true, true);
    }
}

void Helpers::insertClipToTrack(te::AudioTrack *track, te::TransportControl *transport, juce::String filePath) {
    juce::File file(filePath);
    LOG("File path: " + filePath +
        "\nFile Type: " + typeid(file).name());
    te::AudioFile audioFile{ track->edit.engine, file };
    const double audioFileSampleRate {audioFile.getSampleRate()};
    const double sampleRate { transport->engine.getDeviceManager().getSampleRate() };
    if (audioFileSampleRate != sampleRate) {
        LOG("Import Audio Error:\n\tFileSampleRate= " + juce::String(audioFileSampleRate) +
            "\n\tEditSampleRate= " + juce::String(sampleRate));
        return;
    }
    
    double playheadPosition = transport->getCurrentPosition();
    te::ClipPosition position{ {playheadPosition, audioFile.getLength() + playheadPosition}, 0.0 };
    /* check to make sure that it does not interfere with existing clips */
    auto clips = track->getClips();
    for (auto clip: clips) {
        double invalid_start = clip->getPosition().getStart();
        double clip_length = clip->getPosition().getLength();
        double invalid_end = invalid_start + clip_length;
        if (playheadPosition >= invalid_start && playheadPosition <= invalid_end) {
            LOG ("Cannot add track here!: ");
            return;
        }
    }
    te::WaveAudioClip::Ptr clip = track->insertWaveClip(file.getFileNameWithoutExtension(), file, position, false);
    if (clip != nullptr) {
        clip->setAutoPitch(false);
        clip->setAutoTempo(false);
        clip->setTimeStretchMode(te::TimeStretcher::defaultMode);
        const double start{ playheadPosition };
        clip->setStart(start, true, true);
    }

}

void Helpers::insertClipFromJuceFile(te::AudioTrack *track, te::TransportControl *transport, juce::File file){
    juce::String filePath = file.getFullPathName();
    /* prevent from adding non-existant clips
       and empty clips */
    if(!file.exists()) return;
    if(file.getSize()==0) return;
    insertClipToTrack(track, transport, filePath);
}

void Helpers::renderEditToFile(te::Edit *edit){
    if (edit->getLength() <= 0.0) return;
    juce::String saveFilename = Helpers::getFormattedDate();
    juce::String filenameWithPath = APATH;
    filenameWithPath = filenameWithPath + EXPORTPATH + saveFilename + ".wav";
    juce::File file(filenameWithPath);
    
    /* check to see if file exists */
    if (file.exists()) return;
    auto res = file.create();
    const juce::String td = "Export to "+file.getFullPathName();
    std::cout<<td<<'\n';
    
    juce::BigInteger tracksTodo {0};
    for (int i = 0; i < te::getAllTracks(*edit).size(); ++i)
        tracksTodo.setBit(i, true);
    te::EditTimeRange range = {0.0, edit->getLength()};
    bool s = te::Renderer::renderToFile("Render", file, *edit, range, tracksTodo, true, {}, false);
    if (s) DBG("Export successful.\n");
    else DBG("Export failed!\n");
    
}

Helpers::MessageType Helpers::getMidiMessageType(const juce::MidiMessage& msg) {
    if (msg.isNoteOnOrOff()) {
        return Helpers::MessageType::Note;
    }
    else {
        if (msg.getControllerNumber() == CONTEXTUAL_CC_CHANNEL) {
            return Helpers::MessageType::Contextual;
        }
        else {
            return Helpers::MessageType::Universal;
        }
    }
}

Helpers::Encoders Helpers::getEncoderType(const juce::MidiMessage &message) {
    switch (message.getControllerValue()) {
        case ENC_1_CW:
            return Helpers::Encoders::CW1;
        case ENC_1_CCW:
            return Helpers::Encoders::CCW1;
        case ENC_2_CW:
            return Helpers::Encoders::CW2;
        case ENC_2_CCW:
            return Helpers::Encoders::CCW2;
        case ENC_3_CW:
            return Helpers::Encoders::CW3;
        case ENC_3_CCW:
            return Helpers::Encoders::CCW3;
        case ENC_4_CW:
            return Helpers::Encoders::CW4;
        case ENC_4_CCW:
            return Helpers::Encoders::CCW4;
        default:
            return Helpers::Encoders::NOTASSIGNED;
    }
}

Helpers::ContextualCommands Helpers::getContextualCmdType(const juce::MidiMessage &message){
    switch (message.getControllerValue()) {
        case CUT:
            return Helpers::ContextualCommands::Cut;
        case PASTE:
            return Helpers::ContextualCommands::Paste;
        case SAVE:
            return Helpers::ContextualCommands::Save;
        case LOAD:
            return Helpers::ContextualCommands::Load;
        case ADD:
            return Helpers::ContextualCommands::Add;
        case DELETE:
            return Helpers::ContextualCommands::Delete;
        default:
            return Helpers::ContextualCommands::Encoder;
    }
}

Helpers::UniversalCommands Helpers::getUniversalCmdType(const juce::MidiMessage &message) {
    /* check if play or stop */
    if (message.isMidiStop()) return Helpers::UniversalCommands::Pause;
    if (message.isMidiStart()) return Helpers::UniversalCommands::Play;
    switch (message.getControllerValue()) {
        case 1:
            return Helpers::UniversalCommands::Record;
        case 2:
            return Helpers::UniversalCommands::Mute;
        case 3:
            return Helpers::UniversalCommands::Solo;
        case 4:
            return Helpers::UniversalCommands::Timeline;
        case 5:
            return Helpers::UniversalCommands::Synth;
        case 6:
            return Helpers::UniversalCommands::Settings;
        case 7:
            return Helpers::UniversalCommands::Fx;
        case 8:
            return Helpers::UniversalCommands::Metronome;
        case 9:
            return Helpers::UniversalCommands::OctaveUp;
        case 10:
            return Helpers::UniversalCommands::OctaveDown;
        default:
            return Helpers::UniversalCommands::Pause;
    }
}
juce::String Helpers::getMidiMessageDescription(const juce::MidiMessage& m){
    if (m.isNoteOn())           return "Note on " + juce::MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3);
    if (m.isNoteOff())          return "Note off " + juce::MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3);
    if (m.isProgramChange())    return "Program change " + juce::String(m.getProgramChangeNumber());
    if (m.isPitchWheel())       return "Pitch wheel " + juce::String(m.getPitchWheelValue());
    if (m.isAftertouch())       return "After touch " + juce::MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3) + ": " + juce::String(m.getAfterTouchValue());
    if (m.isChannelPressure())  return "Channel pressure " + juce::String(m.getChannelPressureValue());
    if (m.isAllNotesOff())      return "All notes off";
    if (m.isAllSoundOff())      return "All sound off";
    if (m.isMetaEvent())        return "Meta event";

    if (m.isController())
    {
        juce::String name(juce::MidiMessage::getControllerName(m.getControllerNumber()));

        if (name.isEmpty())
            name = "[" + juce::String(m.getControllerNumber()) + "]";

        return "Controller " + name + ": " + juce::String(m.getControllerValue());
    }

    return juce::String::toHexString(m.getRawData(), m.getRawDataSize());
}

juce::String Helpers::getFormattedDate(){
    juce::Time t(juce::Time::getCurrentTime());
    juce::String mm = t.getMonthName(true);
    int dd   = t.getDayOfMonth();
    int yyyy = t.getYear();
    int ss   = t.getSeconds();
    return mm+"-"+juce::String(dd)+"-"+juce::String(yyyy)+"-"+juce::String(ss);
}
