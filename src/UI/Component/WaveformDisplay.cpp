#include "WaveformDisplay.hpp"

namespace ta
{
WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse)
    : _audioThumb(128, formatManagerToUse, cacheToUse)
{
    _audioThumb.addChangeListener(this);
}

void WaveformDisplay::paint(juce::Graphics& g)
{
    auto area = getLocalBounds().reduced(5);

    g.setColour(juce::Colours::white);
    if (!_fileLoaded) { return; }

    auto const totalLength  = _audioThumb.getTotalLength();
    auto const lengthToShow = totalLength * _proportionToShow;
    auto const posInSeconds = totalLength * _playHeadPosition;

    _audioThumb.drawChannel(g, area, posInSeconds, posInSeconds + lengthToShow, 0, 0.5f);

    // auto const x      = area.getX();
    // auto const top    = area.getY();
    // auto const bottom = area.getBottom();
    // auto const width  = area.getWidth();

    // g.setColour(juce::Colours::black);
    // g.fillRect(juce::Rectangle<double>(x + (width * _playHeadPosition), top, 2.0, bottom - top).toFloat());

    // g.setColour(juce::Colours::black.withAlpha(0.5f));
    // for (auto beat : _beatPositions)
    // {
    //     auto normalized = beat / totalLength;
    //     g.fillRect(juce::Rectangle<double>(x + (width * normalized), top, 2.0, bottom - top).toFloat());
    // }
}

void WaveformDisplay::loadURL(juce::URL const& audioURL)
{
    _audioThumb.clear();
    _fileLoaded = _audioThumb.setSource(new juce::URLInputSource(audioURL));
    if (_fileLoaded) { repaint(); }
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* /*source*/) { repaint(); }

void WaveformDisplay::positionRelative(double pos)
{
    if (pos != _playHeadPosition && !std::isnan(pos))
    {
        _playHeadPosition = pos;
        repaint();
    }
}

auto WaveformDisplay::beatPositions(std::vector<double> positionsInSeconds) -> void
{
    _beatPositions = std::move(positionsInSeconds);
    repaint();
}

auto WaveformDisplay::lengthToShow(double proportionToShow) -> void
{
    _proportionToShow = proportionToShow;
    repaint();
}

}  // namespace ta
