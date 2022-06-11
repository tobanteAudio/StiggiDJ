#include "DJPlayer.hpp"

namespace ta
{

DJPlayer::DJPlayer(juce::AudioFormatManager& _formatManager) : formatManager(_formatManager) {}

DJPlayer::~DJPlayer() = default;

auto DJPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) -> void
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
auto DJPlayer::getNextAudioBlock(juce::AudioSourceChannelInfo const& bufferToFill) -> void
{
    resampleSource.getNextAudioBlock(bufferToFill);
}
auto DJPlayer::releaseResources() -> void
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

auto DJPlayer::loadFile(juce::File audioFile) -> LengthAndSamplerate
{
    auto sr      = 0.0;
    auto length  = 0;
    auto* reader = formatManager.createReaderFor(audioFile);

    if (reader != nullptr)  // good file!
    {
        length = reader->lengthInSamples;
        sr     = reader->sampleRate;
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }

    gain(1.0);
    positionRelative(0.0);
    _listeners.call(&Listener::djPlayerFileChanged, audioFile);

    return LengthAndSamplerate{length, sr};
}

auto DJPlayer::gain(double gain) -> void
{
    jassert(juce::isPositiveAndBelow(gain, 4.0));
    transportSource.setGain(gain);
}
auto DJPlayer::speed(double ratio) -> void
{
    jassert(juce::isPositiveAndBelow(ratio, 4.0));
    resampleSource.setResamplingRatio(ratio);
}

auto DJPlayer::position(double posInSecs) -> void { transportSource.setPosition(posInSecs); }

auto DJPlayer::positionRelative(double pos) -> void
{
    jassert(juce::isPositiveAndBelow(pos, 1.0));
    position(transportSource.getLengthInSeconds() * pos);
}

auto DJPlayer::startPlayback() -> void { transportSource.start(); }

auto DJPlayer::stopPlayback() -> void { transportSource.stop(); }

auto DJPlayer::positionRelative() const -> double
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

auto DJPlayer::isPlaying() const -> bool { return transportSource.isPlaying(); }

auto DJPlayer::addListener(Listener* listener) -> void { _listeners.add(listener); }

auto DJPlayer::removeListener(Listener* listener) -> void { _listeners.remove(listener); }

}  // namespace ta
