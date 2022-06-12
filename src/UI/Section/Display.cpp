#include "Display.hpp"

#include "Core/Array.hpp"

namespace ta
{
Display::Display(juce::AudioFormatManager& formatManager, DJPlayer& djPlayer)
    : _djPlayer{djPlayer}, _thumbnailCache{1}, _waveformDisplay{formatManager, _thumbnailCache}
{
    addAndMakeVisible(_waveformDisplay);
    _djPlayer.addListener(this);
    startTimerHz(30);
}

Display::~Display() { _djPlayer.removeListener(this); }

auto Display::resized() -> void { _waveformDisplay.setBounds(getLocalBounds()); }

auto Display::timerCallback() -> void { _waveformDisplay.setPositionRelative(_djPlayer.positionRelative()); }

auto Display::loadURL(juce::URL const& url) -> void { _waveformDisplay.loadURL(url); }

auto Display::djPlayerFileChanged(juce::File const& file) -> void { _waveformDisplay.loadURL(juce::URL{file}); }

}  // namespace ta