#include "WaveformDisplay.hpp"

namespace ta
{
WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse)
    : _audioThumb(1000, formatManagerToUse, cacheToUse)
{
    _audioThumb.addChangeListener(this);
}

void WaveformDisplay::paint(juce::Graphics& g)
{
    auto area = getLocalBounds().reduced(5);

    g.setColour(juce::Colours::white);
    if (!_fileLoaded) { return; }

    auto const length = _audioThumb.getTotalLength();
    _audioThumb.drawChannel(g, area, 0, length, 0, 1.0f);

    auto const x      = area.getX();
    auto const top    = area.getY();
    auto const bottom = area.getBottom();
    auto const width  = area.getWidth();

    g.fillRect(juce::Rectangle<double>(x + (width * _playHeadPosition), top, 2.0, bottom - top).toFloat());

    g.setColour(juce::Colours::white.withAlpha(0.5f));
    for (auto beat : _beatPositions)
    {
        auto normalized = beat / length;
        g.fillRect(juce::Rectangle<double>(x + (width * normalized), top, 2.0, bottom - top).toFloat());
    }
}

void WaveformDisplay::loadURL(juce::URL const& audioURL)
{
    _audioThumb.clear();
    _fileLoaded = _audioThumb.setSource(new juce::URLInputSource(audioURL));
    if (_fileLoaded) { repaint(); }
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* /*source*/) { repaint(); }

void WaveformDisplay::setPositionRelative(double pos)
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

}  // namespace ta
