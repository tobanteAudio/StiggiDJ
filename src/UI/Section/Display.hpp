#pragma once

#include "DSP/DJPlayer.hpp"
#include "UI/Component/WaveformDisplay.hpp"

namespace ta
{
struct Display final
    : juce::Component
    , juce::Timer
    , DJPlayer::Listener
{
    Display(juce::AudioFormatManager& formatManager, DJPlayer& djPlayer);
    ~Display() override;

    auto loadURL(juce::URL const& url) -> void;

    auto resized() -> void override;
    auto timerCallback() -> void override;
    auto djPlayerFileChanged(juce::File const& file) -> void override;

private:
    DJPlayer& _djPlayer;
    juce::AudioThumbnailCache _thumbnailCache;
    WaveformDisplay _waveformDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Display)  // NOLINT
};

}  // namespace ta