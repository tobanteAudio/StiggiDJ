#pragma once

#include <juce_gui_extra/juce_gui_extra.h>

namespace ta
{

struct JogWheel final : juce::Component
{
    JogWheel(juce::String text, juce::Colour color);
    ~JogWheel() override = default;

    auto paint(juce::Graphics& g) -> void override;

private:
    juce::String _text{};
    juce::Colour _background{};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JogWheel)  // NOLINT
};

}  // namespace ta
