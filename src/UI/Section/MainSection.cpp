#include "MainSection.hpp"

#include "Core/Array.hpp"

namespace ta
{
MainSection::MainSection() { addAndMakeVisible(_jogWheel); }

auto MainSection::resized() -> void
{
    auto area = getLocalBounds();
    auto size = std::min(area.getWidth(), area.getHeight());
    _jogWheel.setBounds(area.withSizeKeepingCentre(size, size));
}

}  // namespace ta