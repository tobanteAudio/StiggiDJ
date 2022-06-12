#include "Display.hpp"

#include "Core/Array.hpp"

namespace ta
{
Display::Display(juce::AudioFormatManager& formatManager, DJPlayer& djPlayer)
    : _djPlayer{djPlayer}, _thumbnailCache{1}, _waveformDisplay{formatManager, _thumbnailCache}
{
    addAndMakeVisible(_filename);
    addAndMakeVisible(_bpm);
    addAndMakeVisible(_waveformDisplay);

    _bpm.setJustificationType(juce::Justification::centred);

    _djPlayer.addListener(this);

    startTimerHz(30);
}

Display::~Display() { _djPlayer.removeListener(this); }

auto Display::resized() -> void
{
    using namespace juce;

    Grid grid{};
    grid.rowGap          = 4_px;
    grid.columnGap       = 4_px;
    grid.autoColumns     = Grid::TrackInfo(1_fr);
    grid.autoRows        = Grid::TrackInfo(1_fr);
    grid.autoFlow        = Grid::AutoFlow::row;
    grid.templateRows    = fillArray(Grid::TrackInfo(1_fr), 4);
    grid.templateColumns = fillArray(Grid::TrackInfo(1_fr), 4);
    grid.items.addArray({
        GridItem(_filename).withArea({}, GridItem::Span(3)),
        GridItem(_bpm).withArea({}, {}),
        GridItem(_waveformDisplay).withArea(GridItem::Span(3), GridItem::Span(4)),
    });

    grid.performLayout(getLocalBounds());
}

auto Display::timerCallback() -> void { _waveformDisplay.setPositionRelative(_djPlayer.positionRelative()); }

auto Display::loadURL(juce::URL const& url) -> void { _waveformDisplay.loadURL(url); }

auto Display::waveformZoom(double zoom) -> void { _waveformDisplay.lengthToShow(zoom); }

auto Display::djPlayerFileChanged(juce::File const& file) -> void
{
    _waveformDisplay.loadURL(juce::URL{file});
    _filename.setText(file.getFileName(), juce::sendNotification);
}

auto Display::djPlayerFileAnalysisFinished(BeatTrackResult const& result) -> void
{
    if (result.ok())
    {
        _bpm.setText(juce::String{result.estimatedBPM()} + " bpm", juce::sendNotification);
        _waveformDisplay.beatPositions(result.beatPositions());
        return;
    }

    _bpm.setText("Error:" + result.errorMessage(), juce::sendNotification);
}
}  // namespace ta