#pragma once

#include <juce_gui_extra/juce_gui_extra.h>

namespace ta
{

struct Placeholder final : juce::Component
{
    Placeholder(juce::String text, juce::Colour color);
    ~Placeholder() override = default;

    auto paint(juce::Graphics& g) -> void override;

private:
    juce::String _text{};
    juce::Colour _background{};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Placeholder)  // NOLINT
};

}  // namespace ta
