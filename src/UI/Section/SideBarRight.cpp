#include "SideBarRight.hpp"

namespace ta
{
SideBarRight::SideBarRight()
{
    addAndMakeVisible(_placeholder);
    addAndMakeVisible(_vinylButton);
    addAndMakeVisible(_warpButton);
    addAndMakeVisible(_tempo);
}

auto SideBarRight::resized() -> void
{
    using namespace juce;
    using Track = Grid::TrackInfo;

    auto grid            = Grid{};
    grid.rowGap          = 4_px;
    grid.columnGap       = 4_px;
    grid.autoColumns     = Track(1_fr);
    grid.autoRows        = Track(1_fr);
    grid.autoFlow        = Grid::AutoFlow::column;
    grid.templateRows    = fillArray(Track(1_fr), 12);
    grid.templateColumns = fillArray(Track(1_fr), 1);
    grid.items.addArray({
        GridItem(_placeholder).withArea(GridItem::Span(6), {}),
        GridItem(_vinylButton).withArea(GridItem::Span(1), {}),
        GridItem(_warpButton).withArea(GridItem::Span(1), {}),
        GridItem(_tempo).withArea(GridItem::Span(4), {}),
    });

    grid.performLayout(getLocalBounds());
}

}  // namespace ta