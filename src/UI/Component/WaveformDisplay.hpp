#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace ta
{
struct WaveformDisplay final
    : juce::Component
    , juce::ChangeListener
{
    WaveformDisplay(juce::AudioFormatManager& formatManager, juce::AudioThumbnailCache& cache);
    ~WaveformDisplay() override = default;

    void loadURL(juce::URL const& audioURL);

    /// \brief Set the relative position of the playhead
    void setPositionRelative(double pos);

    void paint(juce::Graphics& g) override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

private:
    juce::AudioThumbnail _audioThumb;
    bool _fileLoaded{false};
    double _position{0};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)  // NOLINT
};
}  // namespace ta