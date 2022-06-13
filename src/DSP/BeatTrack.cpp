#include "BeatTrack.hpp"

#include "DSP/AudioFile.hpp"
#include "DSP/AudioFileConverter.hpp"

namespace ta
{
auto beatTrack(juce::File const& audioFile) -> BeatTrackResult
{
    juce::TemporaryFile tmpFile{};
    auto activeFile = audioFile;
    if (!fileHasFormat<juce::WavAudioFormat>(activeFile))
    {
        AudioFileConverter converter{};
        activeFile                = tmpFile.getFile();
        auto const convertOptions = AudioFileConverterOptions{audioFile, activeFile};
        auto const convertResult  = converter.convert<juce::WavAudioFormat>(convertOptions);
        if (!convertResult.success) { return BeatTrackResult{convertResult.errorMessage}; }
    }

    auto const* pythonPath = "/home/tobante/Developer/tobanteAudio/StiggiDJ/venv/bin/python3";
    auto const scriptDir   = juce::File{"/home/tobante/Developer/tobanteAudio/StiggiDJ/src/Scripts"};
    auto const scriptPath  = scriptDir.getChildFile("bpm_track.py");

    juce::ChildProcess process{};
    auto processArgs = juce::StringArray{pythonPath, scriptPath.getFullPathName(), activeFile.getFullPathName()};
    if (!process.start(processArgs)) { return BeatTrackResult{"Failed to start"}; }
    if (!process.waitForProcessToFinish(10'000)) { return BeatTrackResult{"Timeout"}; }

    auto exitCode = process.getExitCode();
    auto output   = process.readAllProcessOutput();
    if (exitCode != 0) { return BeatTrackResult{output}; }

    auto lines = juce::StringArray::fromLines(output);
    if (lines.size() < 2) { return BeatTrackResult{"Invalid output"}; }

    auto bpm           = lines[0].getDoubleValue();
    auto beatPositions = std::vector<double>{};
    beatPositions.reserve(lines.size() - 1);

    auto toDouble = [](auto const& line) { return line.getDoubleValue(); };
    std::transform(std::next(std::begin(lines), 1), std::end(lines), std::back_inserter(beatPositions), toDouble);

    return BeatTrackResult{bpm, std::move(beatPositions)};
}
}  // namespace ta
