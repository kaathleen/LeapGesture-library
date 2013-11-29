#ifndef DEFAULTWINDOW_H_INCLUDED
#define DEFAULTWINDOW_H_INCLUDED

#include "Header.h"

//==============================================================================
/*
    This class implements the desktop window that contains an instance of
    our MainComponent class.
*/
class DefaultWindow : public DocumentWindow
{
public:
	DefaultWindow(const juce::String windowName)  : DocumentWindow (windowName, Colours::lightgrey, DocumentWindow::allButtons)
	{
		setUsingNativeTitleBar(true);
		setVisible (true);
		setResizable(true, true);
	}

    void closeButtonPressed();

    /* Note: Be careful if you override any DocumentWindow methods - the base
        class uses a lot of them, so by overriding you might break its functionality.
        It's best to do all your work in your content component instead, but if
        you really have to override any DocumentWindow methods, make sure your
        subclass also calls the superclass's method.
    */

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DefaultWindow)
};

#endif // DEFAULTWINDOW_H_INCLUDED
