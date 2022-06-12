#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_formats/juce_audio_formats.h>

#include "rubberband/RubberBandStretcher.h"

namespace ta
{
struct LengthAndSamplerate
{
    juce::int64 length{0};
    double samplerate{0.0};
};

struct DJPlayerListener
{
    virtual ~DJPlayerListener() = default;

    virtual auto djPlayerFileChanged(juce::File const& file) -> void = 0;
};

struct DJPlayer final : juce::AudioSource
{
    using Listener = DJPlayerListener;

    explicit DJPlayer(juce::AudioFormatManager& formatManager);
    ~DJPlayer() override;

    auto loadFile(juce::File audioFile) -> LengthAndSamplerate;

    auto gain(double gain) -> void;
    auto speed(double ratio) -> void;

    auto position(double posInSecs) -> void;
    auto positionRelative(double pos) -> void;
    auto positionRelative() const -> double;

    auto startPlayback() -> void;
    auto stopPlayback() -> void;
    auto isPlaying() const -> bool;

    auto addListener(Listener* listener) -> void;
    auto removeListener(Listener* listener) -> void;

    /// \brief Implements juce::AudioSource::prepareToPlay()
    /// \internal
    auto prepareToPlay(int samplesPerBlockExpected, double sampleRate) -> void override;

    /// \brief Implements juce::AudioSource::getNextAudioBlock()
    /// \internal
    auto getNextAudioBlock(juce::AudioSourceChannelInfo const& bufferToFill) -> void override;

    /// \brief Implements juce::AudioSource::releaseResources()
    /// \internal
    auto releaseResources() -> void override;

private:
    juce::AudioFormatManager& _formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> _readerSource;
    juce::AudioTransportSource _transportSource;
    juce::ResamplingAudioSource _resampleSource{&_transportSource, false, 2};

    std::unique_ptr<RubberBand::RubberBandStretcher> _stretcher;
    juce::AudioBuffer<float> _stretcherBuffer;
    std::atomic<double> _stretchRatio{1.0};

    juce::ListenerList<Listener> _listeners;
};
}  // namespace ta