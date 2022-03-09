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

    te::ClipPosition position{ {0.0, audioFile.getLength()}, transport->getCurrentPosition() };

    te::WaveAudioClip::Ptr clip = track->insertWaveClip(file.getFileNameWithoutExtension(), file, position, false);
    if (clip != nullptr) {
        clip->setAutoPitch(false);
        clip->setAutoTempo(false);
        clip->setTimeStretchMode(te::TimeStretcher::defaultMode);
        const double start{ transport->getCurrentPosition() };
        clip->setStart(start, true, true);
    }
}
