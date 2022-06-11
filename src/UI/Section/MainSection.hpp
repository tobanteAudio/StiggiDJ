#pragma once

#include "UI/Component/JogWheel.hpp"

namespace ta
{
struct MainSection final : juce::Component
{
    MainSection();
    ~MainSection() override = default;

    auto resized() -> void override;

private:
    JogWheel _jogWheel{"Jogwheel", juce::Colours::green};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainSection)
};

}  // namespace ta