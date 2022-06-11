#pragma once

#include "Core/Array.hpp"
#include "UI/Component/Placeholder.hpp"

namespace ta
{
struct SideBarRight final : juce::Component
{
    SideBarRight();
    ~SideBarRight() override = default;

    auto resized() -> void override;

private:
    Placeholder _placeholder{"", juce::Colours::transparentBlack};
    juce::TextButton _vinylButton{"Vinyl"};
    juce::TextButton _warpButton{"Warp"};
    juce::Slider _tempo{juce::Slider::LinearVertical, juce::Slider::TextBoxBelow};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SideBarRight)
};

}  // namespace ta