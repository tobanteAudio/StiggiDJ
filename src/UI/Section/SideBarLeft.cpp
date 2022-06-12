#include "SideBarLeft.hpp"

namespace ta
{
SideBarLeft::SideBarLeft()
{
    addAndMakeVisible(_placeholder);
    addAndMakeVisible(_cueButton);
    addAndMakeVisible(_playButton);

    _cueButton.onClick = [this]()
    {
        if (onCueClicked) { onCueClicked(); }
    };
    _playButton.onClick = [this]()
    {
        if (onPlayClicked) { onPlayClicked(); }
    };
}

auto SideBarLeft::resized() -> void
{
    using namespace juce;
    using Track = Grid::TrackInfo;

    Grid grid{};
    grid.rowGap          = 4_px;
    grid.columnGap       = 4_px;
    grid.autoColumns     = Track(1_fr);
    grid.autoRows        = Track(1_fr);
    grid.autoFlow        = Grid::AutoFlow::column;
    grid.templateRows    = fillArray(Track(1_fr), 12);
    grid.templateColumns = fillArray(Track(1_fr), 1);
    grid.items.addArray({
        GridItem(_placeholder).withArea(GridItem::Span(10), {}),
        GridItem(_cueButton).withArea(GridItem::Span(1), {}),
        GridItem(_playButton).withArea(GridItem::Span(1), {}),
    });

    grid.performLayout(getLocalBounds());
}

}  // namespace ta