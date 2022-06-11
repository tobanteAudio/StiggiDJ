#include "MainComponent.hpp"

#include "Core/Array.hpp"

MainComponent::MainComponent() : _djPlayer{_formatManager}
{
    setAudioDevices();

    addAndMakeVisible(_sideBarLeft);
    addAndMakeVisible(_sideBarRight);
    addAndMakeVisible(_display);
    addAndMakeVisible(_jogWheel);

    _sideBarLeft.onPlayClicked = [this]()
    {
        if (_djPlayer.isPlaying()) { _djPlayer.stopPlayback(); }
        else { _djPlayer.startPlayback(); }
    };

    _sideBarLeft.onCueClicked         = [this]() { _djPlayer.positionRelative(0.0); };
    _sideBarRight.onTempoDeltaChanged = [this](double delta) { _djPlayer.speed((100.0 + delta) / 100.0); };
    setSize(640, 800);
}
MainComponent::~MainComponent()
{
    _djPlayer.stopPlayback();
    _deviceManager.removeAudioCallback(&_audioPlayer);
    _audioPlayer.setSource(nullptr);
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    using namespace juce;
    using Track = Grid::TrackInfo;

    auto grid            = Grid{};
    grid.rowGap          = 4_px;
    grid.columnGap       = 4_px;
    grid.autoColumns     = Track(1_fr);
    grid.autoRows        = Track(1_fr);
    grid.autoFlow        = Grid::AutoFlow::column;
    grid.templateRows    = ta::fillArray(Track(1_fr), 3);
    grid.templateColumns = ta::fillArray(Track(1_fr), 7);
    grid.items.addArray({
        GridItem(_sideBarLeft).withArea(GridItem::Span(3), GridItem::Span(1)),
        GridItem(_display).withArea(GridItem::Span(1), GridItem::Span(5)),
        GridItem(_jogWheel).withArea(GridItem::Span(2), GridItem::Span(5)),
        GridItem(_sideBarRight).withArea(GridItem::Span(3), GridItem::Span(1)),
    });

    grid.performLayout(getLocalBounds().reduced(2));
}

auto MainComponent::setAudioDevices() -> void
{
    _formatManager.registerBasicFormats();
    _deviceManager.initialiseWithDefaultDevices(0, 2);
    _deviceManager.addAudioCallback(&_audioPlayer);
    _audioPlayer.setSource(&_djPlayer);

    auto file = juce::File{"/home/tobante/Music/Loops/Bass.wav"};
    _djPlayer.loadFile(file);
    _display.loadURL(juce::URL{file});
    _djPlayer.gain(1.0);
    _djPlayer.positionRelative(0.0);
}