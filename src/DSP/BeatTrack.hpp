#pragma once

#include <juce_core/juce_core.h>

namespace ta
{

struct BeatTrackResult
{
    explicit BeatTrackResult(juce::String error) : _errorMessage{std::move(error)} {}

    BeatTrackResult(double estimatedBPM, std::vector<double>&& onsetPositions)
        : _bpm{estimatedBPM}, onsets{std::move(onsetPositions)}
    {
    }

    auto ok() const noexcept -> bool { return _errorMessage.isEmpty(); }
    auto error() const noexcept -> bool { return !ok(); }
    auto errorMessage() const -> juce::String const& { return _errorMessage; }

    auto estimatedBPM() const -> double { return _bpm; }
    auto beatPositions() const -> std::vector<double> const& { return onsets; }

private:
    double _bpm{};
    std::vector<double> onsets{};
    juce::String _errorMessage{};
};

auto beatTrack(juce::File const& audioFile) -> BeatTrackResult;

}  // namespace ta
