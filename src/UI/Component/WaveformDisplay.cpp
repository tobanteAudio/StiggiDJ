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
    if (_fileLoaded)
    {
        _audioThumb.drawChannel(g, area, 0, _audioThumb.getTotalLength(), 0, 1.0f);

        auto x      = area.getX() + (area.getWidth() * _position);
        auto top    = area.getY();
        auto bottom = area.getBottom();
        g.fillRect(juce::Rectangle<float>(x, top, 3.0f, bottom - top));
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
    if (pos != _position && !std::isnan(pos))
    {
        _position = pos;
        repaint();
    }
}
}  // namespace ta
