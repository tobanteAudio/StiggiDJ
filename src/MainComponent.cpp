#include "MainComponent.hpp"

#include "Core/Array.hpp"

MainComponent::MainComponent() : _audioFileSource{_formatManager}
{
    setAudioDevices();

    addAndMakeVisible(_sideBarLeft);
    addAndMakeVisible(_sideBarRight);
    addAndMakeVisible(_display);
    addAndMakeVisible(_jogWheel);

    _sideBarLeft.onPlayClicked = [this]() { _audioFileSource.start(); };
    _sideBarLeft.onCueClicked  = [this]() { _audioFileSource.stop(); };
    setSize(640, 800);
}
MainComponent::~MainComponent()
{
    _audioFileSource.stop();
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
    _audioPlayer.setSource(&_audioFileSource);

    _audioFileSource.loadFile(juce::File{"/home/tobante/Music/Loops/Bass.wav"});
    _audioFileSource.setGain(1.0);
}