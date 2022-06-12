#include "MainComponent.hpp"

#include "Core/Array.hpp"
#include "DSP/BeatTrack.hpp"

MainComponent::MainComponent() : _djPlayer{_threadPool, _formatManager}
{
    setupAudioDevices();

    addAndMakeVisible(_sideBarLeft);
    addAndMakeVisible(_sideBarRight);
    addAndMakeVisible(_tabs);
    addAndMakeVisible(_jogWheel);

    _tabs.addTab("Tack", juce::Colours::transparentBlack, &_display, false);
    _tabs.addTab("Browser", juce::Colours::transparentBlack, &_browser, false);
    _tabs.addTab("Settings", juce::Colours::transparentBlack, &_settings, false);

    _sideBarLeft.onPlayClicked = [this]()
    {
        if (_djPlayer.isPlaying()) { _djPlayer.stopPlayback(); }
        else { _djPlayer.startPlayback(); }
    };

    _sideBarLeft.onLoadClicked          = [this]() { loadFile(); };
    _sideBarLeft.onCueClicked           = [this]() { _djPlayer.positionRelative(0.0); };
    _sideBarRight.onTempoDeltaChanged   = [this](double delta) { _djPlayer.speed((100.0 + delta) / 100.0); };
    _sideBarRight.onWaveformZoomChanged = [this](double zoom) { _display.waveformZoom(zoom); };
    _sideBarRight.onTimeStretchToggled  = [this](bool isEnabled) { _djPlayer.timeStretch(isEnabled); };

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

    Grid grid{};
    grid.rowGap          = 4_px;
    grid.columnGap       = 4_px;
    grid.autoColumns     = Track(1_fr);
    grid.autoRows        = Track(1_fr);
    grid.autoFlow        = Grid::AutoFlow::column;
    grid.templateRows    = ta::fillArray(Track(1_fr), 3);
    grid.templateColumns = ta::fillArray(Track(1_fr), 7);
    grid.items.addArray({
        GridItem(_sideBarLeft).withArea(GridItem::Span(3), GridItem::Span(1)),
        GridItem(_tabs).withArea(GridItem::Span(1), GridItem::Span(5)),
        GridItem(_jogWheel).withArea(GridItem::Span(2), GridItem::Span(5)),
        GridItem(_sideBarRight).withArea(GridItem::Span(3), GridItem::Span(1)),
    });

    grid.performLayout(getLocalBounds().reduced(2));
}

auto MainComponent::loadFile() -> void
{
    auto const* msg      = "Please select the audio file you want to load...";
    auto const dir       = juce::File::getSpecialLocation(juce::File::userMusicDirectory);
    auto const fileFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

    auto load = [this](auto const& chooser)
    {
        auto file = chooser.getResult();
        if (!file.existsAsFile()) { return; }
        _djPlayer.loadFile(file);
    };

    _fileChooser = std::make_unique<juce::FileChooser>(msg, dir, _formatManager.getWildcardForAllFormats());
    _fileChooser->launchAsync(fileFlags, load);
}

auto MainComponent::setupAudioDevices() -> void
{
    _formatManager.registerBasicFormats();
    _deviceManager.initialiseWithDefaultDevices(0, 2);
    _deviceManager.addAudioCallback(&_audioPlayer);
    _audioPlayer.setSource(&_djPlayer);
}