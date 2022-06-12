#include "BeatTrack.hpp"

namespace ta
{
auto beatTrack(juce::File const& audioFile) -> BeatTrackResult
{
    auto const* pythonPath = "/home/tobante/Developer/tobanteAudio/StiggiDJ/venv/bin/python3";
    auto const scriptDir   = juce::File{"/home/tobante/Developer/tobanteAudio/StiggiDJ/src/Scripts"};
    auto const scriptPath  = scriptDir.getChildFile("bpm_track.py");

    juce::ChildProcess process{};
    auto processArgs = juce::StringArray{pythonPath, scriptPath.getFullPathName(), audioFile.getFullPathName()};
    if (!process.start(processArgs)) { return BeatTrackResult{"Failed to start"}; }
    if (!process.waitForProcessToFinish(10'000)) { return BeatTrackResult{"Timeout"}; }

    auto exitCode = process.getExitCode();
    auto output   = process.readAllProcessOutput();
    if (exitCode != 0) { return BeatTrackResult{output}; }

    auto outputLines = juce::StringArray::fromLines(output);
    if (outputLines.size() < 2) { return BeatTrackResult{"Invalid output"}; }

    auto bpm           = outputLines[0].getDoubleValue();
    auto beatPositions = std::vector<double>{};
    beatPositions.reserve(outputLines.size() - 1);

    auto toDouble = [](auto const& line) { return line.getDoubleValue(); };
    std::transform(outputLines.begin() + 1, outputLines.end(), std::back_inserter(beatPositions), toDouble);

    return BeatTrackResult{bpm, std::move(beatPositions)};
}
}  // namespace ta
