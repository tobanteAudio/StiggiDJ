#include "JogWheel.hpp"

namespace ta
{

JogWheel::JogWheel(juce::String text, juce::Colour color) : _text{std::move(text)}, _background{color} {}

auto JogWheel::paint(juce::Graphics& g) -> void
{
    g.setColour(_background);

    auto const area       = getLocalBounds().toFloat();
    auto const size       = std::min(area.getWidth(), area.getHeight());
    auto const circleArea = area.withSizeKeepingCentre(size, size);
    g.fillEllipse(circleArea);

    g.setFont(juce::Font(16.0f));
    g.setColour(juce::Colours::white);
    g.drawText(_text, circleArea, juce::Justification::centred, true);
}

}  // namespace ta
