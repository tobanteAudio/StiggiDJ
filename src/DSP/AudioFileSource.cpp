#include "AudioFileSource.hpp"

namespace ta
{

AudioFileSource::AudioFileSource(juce::AudioFormatManager& _formatManager) : formatManager(_formatManager) {}

AudioFileSource::~AudioFileSource() {}

auto AudioFileSource::prepareToPlay(int samplesPerBlockExpected, double sampleRate) -> void
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
auto AudioFileSource::getNextAudioBlock(juce::AudioSourceChannelInfo const& bufferToFill) -> void
{
    resampleSource.getNextAudioBlock(bufferToFill);
}
auto AudioFileSource::releaseResources() -> void
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

auto AudioFileSource::loadFile(juce::File audioFile) -> LengthAndSamplerate
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

    return LengthAndSamplerate{length, sr};
}

auto AudioFileSource::gain(double gain) -> void
{
    jassert(juce::isPositiveAndBelow(gain, 4.0));
    transportSource.setGain(gain);
}
auto AudioFileSource::speed(double ratio) -> void
{
    jassert(juce::isPositiveAndBelow(ratio, 4.0));
    resampleSource.setResamplingRatio(ratio);
}

auto AudioFileSource::position(double posInSecs) -> void { transportSource.setPosition(posInSecs); }

auto AudioFileSource::positionRelative(double pos) -> void
{
    jassert(juce::isPositiveAndBelow(pos, 1.0));
    position(transportSource.getLengthInSeconds() * pos);
}

auto AudioFileSource::startPlayback() -> void { transportSource.start(); }

auto AudioFileSource::stopPlayback() -> void { transportSource.stop(); }

auto AudioFileSource::positionRelative() const -> double
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

auto AudioFileSource::isPlaying() const -> bool { return transportSource.isPlaying(); }

}  // namespace ta
