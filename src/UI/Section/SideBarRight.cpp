#include "SideBarRight.hpp"

namespace ta
{
SideBarRight::SideBarRight()
{
    addAndMakeVisible(_placeholderTop);
    addAndMakeVisible(_placeholderBottom);
    addAndMakeVisible(_waveformZoom);
    addAndMakeVisible(_vinylButton);
    addAndMakeVisible(_warpButton);
    addAndMakeVisible(_tempoRange);
    addAndMakeVisible(_tempo);

    _waveformZoom.setRange({0.01, 1.0}, 0.001);
    _waveformZoom.setSkewFactorFromMidPoint(0.1);
    _waveformZoom.setValue(1.0);
    _waveformZoom.onValueChange = [this]()
    {
        if (onWaveformZoomChanged) { onWaveformZoomChanged(_waveformZoom.getValue()); }
    };

    _warpButton.setClickingTogglesState(true);
    _warpButton.onClick = [this]()
    {
        if (onTimeStretchToggled) { onTimeStretchToggled(_warpButton.getToggleState()); }
    };

    _tempoRange.setEditableText(false);
    _tempoRange.setJustificationType(juce::Justification::centred);
    _tempoRange.addItemList({"6%", "10%", "20%", "100%"}, 1);
    _tempoRange.setSelectedId(1);
    _tempoRange.onChange = [this]() { updateSpeedRange(); };
    updateSpeedRange();

    _tempo.setValue(0.0, juce::dontSendNotification);
    _tempo.onValueChange = [this]()
    {
        if (onTempoDeltaChanged) { onTempoDeltaChanged(_tempo.getValue()); }
    };
}

auto SideBarRight::resized() -> void
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
        GridItem(_placeholderTop).withArea(GridItem::Span(2), {}),
        GridItem(_waveformZoom).withArea(GridItem::Span(1), {}),
        GridItem(_vinylButton).withArea(GridItem::Span(1), {}),
        GridItem(_placeholderBottom).withArea(GridItem::Span(2), {}),
        GridItem(_warpButton).withArea(GridItem::Span(1), {}),
        GridItem(_tempoRange).withArea(GridItem::Span(1), {}),
        GridItem(_tempo).withArea(GridItem::Span(4), {}),
    });

    grid.performLayout(getLocalBounds());
}

auto SideBarRight::updateSpeedRange() -> void
{
    auto rangeID = _tempoRange.getSelectedId();
    switch (rangeID)
    {
        case 1: _tempo.setRange({-6.0, 6.0}, 0.01); break;
        case 2: _tempo.setRange({-10.0, 10.0}, 0.01); break;
        case 3: _tempo.setRange({-20.0, 20.0}, 0.01); break;
        case 4: _tempo.setRange({-100.0, 100.0}, 0.01); break;
        default: jassertfalse;
    }

    auto const val     = _tempo.getValue();
    auto const range   = _tempo.getRange();
    auto const clipped = range.clipValue(val);

    if (onTempoDeltaChanged) { onTempoDeltaChanged(clipped); }
    _tempo.setValue(clipped, juce::sendNotification);
}

}  // namespace ta