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
            break;
    }
    return Helpers::UniversalCommands::Pause;
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
