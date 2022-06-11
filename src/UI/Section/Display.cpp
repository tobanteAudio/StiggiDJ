#include "Display.hpp"

#include "Core/Array.hpp"

namespace ta
{
Display::Display(juce::AudioFormatManager& formatManager)
    : _thumbnailCache{1}, _waveformDisplay{formatManager, _thumbnailCache}
{
    addAndMakeVisible(_waveformDisplay);
}

auto Display::resized() -> void { _waveformDisplay.setBounds(getLocalBounds()); }

auto Display::loadURL(juce::URL url) -> void { _waveformDisplay.loadURL(url); }

}  // namespace ta