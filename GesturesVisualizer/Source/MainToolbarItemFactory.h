#ifndef MAINITEMTOOLBARFACTORY_H_INCLUDED
#define MAINITEMTOOLBARFACTORY_H_INCLUDED

using namespace std;

#include "../JuceLibraryCode/JuceHeader.h"

//========================================================================== ====
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainToolbarItemFactory : public ToolbarItemFactory
{
public:
	MainToolbarItemFactory(ButtonListener* buttonListener, ComboBoxListener* comboBoxListener)
	{
		this->buttonListener = buttonListener;
		this->comboBoxListener = comboBoxListener;
	}

    //==============================================================================
    // Each type of item a toolbar can contain must be given a unique ID. These
    // are the ones we'll use in this demo.
    enum MainToolbarItemIds
    {
        open_gesture_files	= 1,
        choose_gesture_file		= 2
    };

    void getAllToolbarItemIds (Array <int>& ids);

    void getDefaultItemSet (Array <int>& ids);

    ToolbarItemComponent* createItem (int itemId);

	void setGesturesList(StringArray gestures);

private:
	ButtonListener* buttonListener;
	ComboBoxListener* comboBoxListener;

    class ToolbarComboBox : public ToolbarItemComponent
    {
    public:
		ToolbarComboBox (const int toolbarItemId, const String componentId, ComboBoxListener* listenerComponent)
            : ToolbarItemComponent (toolbarItemId, componentId, false),
                comboBox (componentId)
        {
            addAndMakeVisible (&comboBox);
			comboBox.setEditableText(false);
			
			comboBox.setComponentID(componentId);
			
			if (listenerComponent != NULL)
			{
				comboBox.addListener(listenerComponent);
			}
        }

		void setValues(StringArray values)
		{
			comboBox.clear();
			for (int i = 0; i < values.size(); i++)
			{
                comboBox.addItem (values[i], i+1);
			}
			if (values.size() > 0)
			{
				comboBox.setSelectedId(1);
			}
		}

        bool getToolbarItemSizes (int /*toolbarDepth*/, bool isVertical,
                                    int& preferredSize, int& minSize, int& maxSize)
        {
            if (isVertical)
                return false;

            preferredSize = 250;
            minSize = 80;
            maxSize = 300;
            return true;
        }

        void paintButtonArea (Graphics&, int, int, bool, bool)
        {
        }

        void contentAreaChanged (const Rectangle<int>& newArea)
        {
            comboBox.setSize (newArea.getWidth() - 2,
                                jmin (newArea.getHeight() - 2, 22));

            comboBox.setCentrePosition (newArea.getCentreX(), newArea.getCentreY());
        }

    private:
        ComboBox comboBox;
    };

	class ToolbarTextButton : public ToolbarItemComponent
    {
    public:
        ToolbarTextButton (const int toolbarItemId, const String text, const String componentId, ButtonListener* listenerComponent)
            : ToolbarItemComponent (toolbarItemId, text, false),
                textButton(text)
        {
			addAndMakeVisible (&textButton);
			textButton.setComponentID(componentId);

			if (listenerComponent != NULL)
			{
				textButton.addListener(listenerComponent);
			}
        }

        bool getToolbarItemSizes (int /*toolbarDepth*/, bool isVertical,
                                    int& preferredSize, int& minSize, int& maxSize)
        {
            if (isVertical)
                return false;

            preferredSize = 250;
            minSize = 80;
            maxSize = 300;
            return true;
        }

        void paintButtonArea (Graphics&, int, int, bool, bool)
        {
        }

        void contentAreaChanged (const Rectangle<int>& newArea)
        {
            textButton.setSize (newArea.getWidth() - 2,
                                jmin (newArea.getHeight() - 2, 22));

            textButton.setCentrePosition (newArea.getCentreX(), newArea.getCentreY());
        }

    private:
		TextButton textButton;
    };

	ToolbarTextButton* chooseGesturesButton;
	ToolbarComboBox* gesturesComboBox;
};


#endif  // MAINITEMTOOLBARFACTORY_H_INCLUDED
