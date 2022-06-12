#pragma once

#include <juce_core/juce_core.h>

namespace ta
{

struct BeatTrackResult
{
    explicit BeatTrackResult(juce::String error) noexcept : _errorMessage{std::move(error)} {}

    BeatTrackResult(double estimatedBPM, std::vector<double>&& onsetPositions) noexcept
        : _bpm{estimatedBPM}, _onsets{std::move(onsetPositions)}
    {
    }

    auto ok() const noexcept -> bool { return _errorMessage.isEmpty(); }
    auto error() const noexcept -> bool { return !ok(); }
    auto errorMessage() const -> juce::String const& { return _errorMessage; }

    auto estimatedBPM() const -> double { return _bpm; }
    auto beatPositions() const -> std::vector<double> const& { return _onsets; }

private:
    double _bpm{};
    std::vector<double> _onsets{};
    juce::String _errorMessage{};
};

auto beatTrack(juce::File const& audioFile) -> BeatTrackResult;

}  // namespace ta
