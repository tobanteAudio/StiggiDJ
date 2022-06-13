#pragma once

#include <juce_core/juce_core.h>

namespace ta
{

struct BeatTrackOptions
{
    juce::File audioFile{};
    bool sumToMono{false};
};

struct BeatTrackResult
{
    explicit BeatTrackResult(std::string error) noexcept;
    BeatTrackResult(double bpm, std::vector<double>&& beatPositions) noexcept;

    auto ok() const noexcept -> bool;
    auto error() const noexcept -> bool;
    auto errorMessage() const -> std::string const&;

    auto bpm() const -> double;
    auto beatPositions() const -> std::vector<double> const&;

private:
    double _bpm{};
    std::vector<double> _beatPositions{};
    std::string _errorMessage{};
};

auto beatTrack(BeatTrackOptions const& options) -> BeatTrackResult;

}  // namespace ta
