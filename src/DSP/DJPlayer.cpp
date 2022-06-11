#include "DJPlayer.hpp"

namespace ta
{

DJPlayer::DJPlayer(juce::AudioFormatManager& formatManager) : _formatManager(formatManager) {}

DJPlayer::~DJPlayer() = default;

auto DJPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) -> void
{
    _transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    _resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
auto DJPlayer::getNextAudioBlock(juce::AudioSourceChannelInfo const& bufferToFill) -> void
{
    _resampleSource.getNextAudioBlock(bufferToFill);
}
auto DJPlayer::releaseResources() -> void
{
    _transportSource.releaseResources();
    _resampleSource.releaseResources();
}

auto DJPlayer::loadFile(juce::File audioFile) -> LengthAndSamplerate
{
    auto sr      = 0.0;
    auto length  = 0;
    auto* reader = _formatManager.createReaderFor(audioFile);

    if (reader != nullptr)  // good file!
    {
        length = reader->lengthInSamples;
        sr     = reader->sampleRate;
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
        _transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        _readerSource = std::move(newSource);
    }

    gain(1.0);
    positionRelative(0.0);
    _listeners.call(&Listener::djPlayerFileChanged, audioFile);

    return LengthAndSamplerate{length, sr};
}

auto DJPlayer::gain(double gain) -> void
{
    jassert(juce::isPositiveAndBelow(gain, 4.0));
    _transportSource.setGain(gain);
}
auto DJPlayer::speed(double ratio) -> void
{
    jassert(juce::isPositiveAndBelow(ratio, 4.0));
    _resampleSource.setResamplingRatio(ratio);
}

auto DJPlayer::position(double posInSecs) -> void { _transportSource.setPosition(posInSecs); }

auto DJPlayer::positionRelative(double pos) -> void
{
    jassert(juce::isPositiveAndBelow(pos, 1.0));
    position(_transportSource.getLengthInSeconds() * pos);
}

auto DJPlayer::startPlayback() -> void { _transportSource.start(); }

auto DJPlayer::stopPlayback() -> void { _transportSource.stop(); }

auto DJPlayer::positionRelative() const -> double
{
    return _transportSource.getCurrentPosition() / _transportSource.getLengthInSeconds();
}

auto DJPlayer::isPlaying() const -> bool { return _transportSource.isPlaying(); }

auto DJPlayer::addListener(Listener* listener) -> void { _listeners.add(listener); }

auto DJPlayer::removeListener(Listener* listener) -> void { _listeners.remove(listener); }

}  // namespace ta
