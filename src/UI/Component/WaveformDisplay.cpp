#include "WaveformDisplay.hpp"

namespace ta
{
WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse)
    : audioThumb_(1000, formatManagerToUse, cacheToUse), fileLoaded_(false), position_(0)
{
    audioThumb_.addChangeListener(this);
}

void WaveformDisplay::paint(juce::Graphics& g)
{
    auto area = getLocalBounds().reduced(5);

    g.setColour(juce::Colours::white);
    if (fileLoaded_)
    {
        audioThumb_.drawChannel(g, area, 0, audioThumb_.getTotalLength(), 0, 1.0f);

        auto x      = area.getX() + (area.getWidth() * position_);
        auto top    = area.getY();
        auto bottom = area.getBottom();
        g.fillRect(juce::Rectangle<float>(x, top, 3.0f, bottom - top));
    }
}

void WaveformDisplay::loadURL(juce::URL audioURL)
{
    audioThumb_.clear();
    fileLoaded_ = audioThumb_.setSource(new juce::URLInputSource(audioURL));
    if (fileLoaded_) { repaint(); }
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* /*source*/) { repaint(); }

void WaveformDisplay::setPositionRelative(double pos)
{
    if (pos != position_ && !std::isnan(pos))
    {
        position_ = pos;
        repaint();
    }
}
}  // namespace ta
