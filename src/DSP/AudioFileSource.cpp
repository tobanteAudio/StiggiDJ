#include "AudioFileSource.hpp"

namespace ta
{

AudioFileSource::AudioFileSource(juce::AudioFormatManager& _formatManager) : formatManager(_formatManager) {}
AudioFileSource::~AudioFileSource() {}

void AudioFileSource::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
void AudioFileSource::getNextAudioBlock(juce::AudioSourceChannelInfo const& bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);
}
void AudioFileSource::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

LengthAndSamplerate AudioFileSource::loadFile(juce::File audioFile)
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

void AudioFileSource::setGain(double gain)
{
    if (gain < 0 || gain > 1.0) { std::cout << "AudioFileSource::setGain gain should be between 0 and 1" << std::endl; }
    else { transportSource.setGain(gain); }
}
void AudioFileSource::setSpeed(double ratio)
{
    if (ratio <= 0 || ratio > 100.0)
    {
        std::cout << "AudioFileSource::setSpeed ratio should be between 0 and 100" << std::endl;
    }
    else { resampleSource.setResamplingRatio(ratio); }
}
void AudioFileSource::setPosition(double posInSecs) { transportSource.setPosition(posInSecs); }

void AudioFileSource::setPositionRelative(double pos)
{
    if (pos < 0 || pos > 1.0)
    {
        std::cout << "AudioFileSource::setPositionRelative pos should be between 0 and 1" << std::endl;
    }
    else
    {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

void AudioFileSource::start() { transportSource.start(); }
void AudioFileSource::stop() { transportSource.stop(); }

double AudioFileSource::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

}  // namespace ta
