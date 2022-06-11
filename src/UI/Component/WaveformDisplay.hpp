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

    void loadURL(juce::URL audioURL);

    /// \brief Set the relative position of the playhead
    void setPositionRelative(double pos);

    void paint(juce::Graphics&) override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

private:
    juce::AudioThumbnail audioThumb_;
    bool fileLoaded_;
    double position_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};
}  // namespace ta