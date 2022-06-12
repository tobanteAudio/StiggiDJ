#pragma once

#include "Core/Array.hpp"
#include "UI/Component/Placeholder.hpp"

namespace ta
{
struct SideBarLeft final : juce::Component
{
    SideBarLeft();
    ~SideBarLeft() override = default;

    auto resized() -> void override;

    std::function<void()> onLoadClicked{};
    std::function<void()> onCueClicked{};
    std::function<void()> onPlayClicked{};

private:
    Placeholder _placeholder{"", juce::Colours::transparentBlack};
    juce::TextButton _loadButton{"Load"};
    juce::TextButton _cueButton{"Cue"};
    juce::TextButton _playButton{"Play"};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SideBarLeft)  // NOLINT
};

}  // namespace ta