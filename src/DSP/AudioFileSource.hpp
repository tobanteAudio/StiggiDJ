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

struct AudioFileSource final : juce::AudioSource
{
    explicit AudioFileSource(juce::AudioFormatManager& _formatManager);
    ~AudioFileSource() override;

    auto loadFile(juce::File audioFile) -> LengthAndSamplerate;

    auto gain(double gain) -> void;
    auto speed(double ratio) -> void;

    auto position(double posInSecs) -> void;
    auto positionRelative(double pos) -> void;
    auto positionRelative() const -> double;

    auto startPlayback() -> void;
    auto stopPlayback() -> void;
    auto isPlaying() const -> bool;

    /// \brief Implements juce::AudioSource::prepareToPlay()
    auto prepareToPlay(int samplesPerBlockExpected, double sampleRate) -> void override;

    /// \brief Implements juce::AudioSource::getNextAudioBlock()
    auto getNextAudioBlock(juce::AudioSourceChannelInfo const& bufferToFill) -> void override;

    /// \brief Implements juce::AudioSource::releaseResources()
    auto releaseResources() -> void override;

private:
    juce::AudioFormatManager& formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{&transportSource, false, 2};
};
}  // namespace ta