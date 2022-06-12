#include <memory>

#include "MainComponent.hpp"

struct GuiAppApplication final : public juce::JUCEApplication
{
    GuiAppApplication() = default;

    auto getApplicationName() -> juce::String const override { return JUCE_APPLICATION_NAME_STRING; }
    auto getApplicationVersion() -> juce::String const override { return JUCE_APPLICATION_VERSION_STRING; }
    auto moreThanOneInstanceAllowed() -> bool override { return true; }

    void initialise(juce::String const& /*commandLine*/) override
    {
        _mainWindow = std::make_unique<MainWindow>(getApplicationName());
    }

    void shutdown() override { _mainWindow = nullptr; }

    void systemRequestedQuit() override { quit(); }

    void anotherInstanceStarted(juce::String const& commandLine) override { juce::ignoreUnused(commandLine); }

    struct MainWindow final : juce::DocumentWindow
    {
        explicit MainWindow(juce::String const& name)
            : DocumentWindow(
                name,
                juce::Desktop::getInstance().getDefaultLookAndFeel().findColour(ResizableWindow::backgroundColourId),
                DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(new MainComponent(), true);

#if JUCE_IOS || JUCE_ANDROID
            setFullScreen(true);
#else
            setResizable(true, true);
            centreWithSize(getWidth(), getHeight());
#endif

            setVisible(true);
        }

        void closeButtonPressed() override { JUCEApplication::getInstance()->systemRequestedQuit(); }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its
           functionality. It's best to do all your work in your content component
           instead, but if you really have to override any DocumentWindow methods,
           make sure your subclass also calls the superclass's method.
        */

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)  // NOLINT
    };

private:
    std::unique_ptr<MainWindow> _mainWindow;
};

// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION(GuiAppApplication)  // NOLINT
