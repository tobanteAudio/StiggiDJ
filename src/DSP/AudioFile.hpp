#pragma once

#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_core/juce_core.h>

namespace ta
{

template<typename ExpectedFormat>
auto fileHasFormat(juce::AudioFormatManager& formatManager, juce::File const& file) -> bool
{
    static_assert(std::is_base_of<juce::AudioFormat, ExpectedFormat>::value,
                  "ExpectedFormat must be a subclass of AudioFormat");

    auto reader = std::unique_ptr<juce::AudioFormatReader>(formatManager.createReaderFor(file));
    if (reader == nullptr) { return false; }
    return dynamic_cast<ExpectedFormat*>(reader.get()) != nullptr;
}

template<typename ExpectedFormat>
auto fileHasFormat(juce::File const& file) -> bool
{
    juce::AudioFormatManager formatManager{};
    formatManager.registerBasicFormats();
    return fileHasFormat<ExpectedFormat>(formatManager, file);
}

}  // namespace ta
