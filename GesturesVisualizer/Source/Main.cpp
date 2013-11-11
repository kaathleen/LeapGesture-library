#include "../JuceLibraryCode/JuceHeader.h"

#include "MainComponent.h"
#include "DefaultWindow.h"

#include "Header.h"


//==============================================================================
class GesturesVisualizerApplication  : public JUCEApplication
{
public:
    //==============================================================================
    GesturesVisualizerApplication() {}

    const String getApplicationName()       { return ProjectInfo::projectName; }
    const String getApplicationVersion()    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed()       { return true; }

    //==============================================================================
    void initialise (const String& commandLine)
    {
		vector<string> commands = StringHelper::split(commandLine.toStdString(), ' ');
        mainWindow = new DefaultWindow("Gestures Visualizer");
		mainWindow.get()->setContentOwned (new MainComponent(commands), true);
		mainWindow.get()->centreWithSize (mainWindow.get()->getWidth(), mainWindow.get()->getHeight());
    }

    void shutdown()
    {
        // Add your application's shutdown code here..

        mainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit()
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted (const String& commandLine)
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }

private:
    ScopedPointer<DefaultWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (GesturesVisualizerApplication)
