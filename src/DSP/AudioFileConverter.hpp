#pragma once

#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_core/juce_core.h>

namespace ta
{

struct AudioFileConverterOptions
{
    juce::File source{};
    juce::File destination{};
};

struct AudioFileConverterResult
{
    bool success{false};
    juce::String errorMessage{};
};

struct AudioFileConverter
{
    using Options = AudioFileConverterOptions;
    using Result  = AudioFileConverterResult;

    AudioFileConverter();

    template<typename DestinationFormat>
    auto convert(Options const& options) -> Result;

private:
    juce::AudioFormatManager _formatManager;
};

inline AudioFileConverter::AudioFileConverter() { _formatManager.registerBasicFormats(); }

template<typename DestinationFormat>
inline auto AudioFileConverter::convert(Options const& options) -> Result
{
    auto reader = std::unique_ptr<juce::AudioFormatReader>(_formatManager.createReaderFor(options.source));
    if (reader == nullptr) { return Result{false, "Invalid file"}; }

    DestinationFormat format{};
    auto const sr   = reader->sampleRate;
    auto const ch   = reader->numChannels;
    auto const bits = reader->bitsPerSample;

    auto out    = std::make_unique<juce::FileOutputStream>(options.destination);
    auto writer = std::unique_ptr<juce::AudioFormatWriter>(format.createWriterFor(out.get(), sr, ch, bits, {}, 0));

    if (writer == nullptr) { return Result{false, "Failed to convert to wav"}; }
    auto* p = out.release();  // writer took ownership
    (void)p;                  // fixes nodiscard warning on release

    writer->writeFromAudioReader(*reader, 0, reader->lengthInSamples);
    writer->flush();

    return Result{true, {}};
}

}  // namespace ta
