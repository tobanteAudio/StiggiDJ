#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_formats/juce_audio_formats.h>

namespace ta
{
struct LengthAndSamplerate
{
    juce::int64 length{0};
    double samplerate{0.0};
};

class AudioFileSource : public juce::AudioSource
{
public:
    AudioFileSource(juce::AudioFormatManager& _formatManager);
    ~AudioFileSource() override;

    auto loadFile(juce::File audioFile) -> LengthAndSamplerate;
    auto setGain(double gain) -> void;
    auto setSpeed(double ratio) -> void;
    auto setPosition(double posInSecs) -> void;
    auto setPositionRelative(double pos) -> void;

    auto start() -> void;
    auto stop() -> void;

    auto getPositionRelative() -> double;

    auto prepareToPlay(int samplesPerBlockExpected, double sampleRate) -> void override;
    auto getNextAudioBlock(juce::AudioSourceChannelInfo const& bufferToFill) -> void override;
    auto releaseResources() -> void override;

private:
    juce::AudioFormatManager& formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{&transportSource, false, 2};
};
}  // namespace ta