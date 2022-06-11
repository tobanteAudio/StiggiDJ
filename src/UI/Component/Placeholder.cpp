#include "Placeholder.hpp"

namespace ta
{

Placeholder::Placeholder(juce::String text, juce::Colour color) : _text{std::move(text)}, _background{color} {}

auto Placeholder::paint(juce::Graphics& g) -> void
{
    g.fillAll(_background);

    g.setFont(juce::Font(16.0f));
    g.setColour(juce::Colours::white);
    g.drawText(_text, getLocalBounds(), juce::Justification::centred, true);
}

}  // namespace ta
