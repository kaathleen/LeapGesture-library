#include "MainToolbarItemFactory.h"

void MainToolbarItemFactory::getAllToolbarItemIds (Array <int>& ids)
{
    // This returns the complete list of all item IDs that are allowed to
    // go in our toolbar. Any items you might want to add must be listed here. The
    // order in which they are listed will be used by the toolbar customisation panel.
	ids.add (change_work_mode);
    ids.add (open_gesture_files);
    ids.add (choose_gesture_file);

    // If you're going to use separators, then they must also be added explicitly
    // to the list.
    ids.add (separatorBarId);
    ids.add (spacerId);
    ids.add (flexibleSpacerId);
}

void MainToolbarItemFactory::getDefaultItemSet (Array <int>& ids)
{
    // This returns an ordered list of the set of items that make up a
    // toolbar's default set. Not all items need to be on this list, and
    // items can appear multiple times (e.g. the separators used here).
    ids.add (change_work_mode);
	ids.add (separatorBarId);
	ids.add (open_gesture_files);
    ids.add (choose_gesture_file);
}

ToolbarItemComponent* MainToolbarItemFactory::createItem (int itemId)
{
    switch (itemId)
    {
		case change_work_mode:		return changeWorkModeButton = new ToolbarTextButton(itemId, "Change to recorder mode", "CHANGE_WORK_MODE", buttonListener);
        case open_gesture_files:	return chooseGesturesButton = new ToolbarTextButton(itemId, "Open files", "OPEN_GESTURES_FILES", buttonListener);
		case choose_gesture_file:	return gesturesComboBox = new ToolbarComboBox(itemId, "CHOOSE_GESTURE_FILE", comboBoxListener);
        default:					break;
    }

    return 0;
}

void MainToolbarItemFactory::setGesturesList(StringArray gestures)
{
	gesturesComboBox->setValues(gestures);
}

void MainToolbarItemFactory::changeWorkMode(bool isVisualizerMode)
{
	if (!isVisualizerMode)
	{
		changeWorkModeButton->setButtonText("RECORDER MODE");
		chooseGesturesButton->setVisible(false);
		gesturesComboBox->setVisible(false);
	}
	else
	{
		changeWorkModeButton->setButtonText("VISUALIZER MODE");
		chooseGesturesButton->setVisible(true);
		gesturesComboBox->setVisible(true);
	}
}

void MainToolbarItemFactory::setWorkModeButtonEnable(bool isEnable)
{
	changeWorkModeButton->setEnabled(isEnable);
}
