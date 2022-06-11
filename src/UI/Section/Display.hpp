#pragma once

#include "UI/Component/WaveformDisplay.hpp"

namespace ta
{
struct Display final : juce::Component
{
    explicit Display(juce::AudioFormatManager& formatManager);
    ~Display() override = default;

    auto loadURL(juce::URL url) -> void;

    auto resized() -> void override;

private:
    juce::AudioThumbnailCache _thumbnailCache;
    WaveformDisplay _waveformDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Display)
};

}  // namespace ta