#pragma once

#include "DSP/DJPlayer.hpp"
#include "UI/Component/Placeholder.hpp"
#include "UI/Section/Display.hpp"
#include "UI/Section/MainSection.hpp"
#include "UI/Section/SideBarLeft.hpp"
#include "UI/Section/SideBarRight.hpp"

#include <juce_audio_devices/juce_audio_devices.h>

struct MainComponent final : juce::Component
{
    MainComponent();
    ~MainComponent() override;

    auto paint(juce::Graphics& g) -> void override;
    auto resized() -> void override;

private:
    auto setAudioDevices() -> void;

    juce::AudioDeviceManager _deviceManager;
    juce::AudioFormatManager _formatManager;
    juce::AudioSourcePlayer _audioPlayer;
    ta::DJPlayer _djPlayer;

    ta::SideBarLeft _sideBarLeft{};
    ta::SideBarRight _sideBarRight{};
    ta::MainSection _jogWheel;
    ta::Display _display{_formatManager, _djPlayer};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)  // NOLINT
};
