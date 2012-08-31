/*
Copyright (C) 2012 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/**
 * SettingsScreen.cpp
 *
 *  Created on: Aug 7, 2012
 *      Author: Spiridon Alexandru
 */

#ifndef SETTINGSSCREEN_H_
#define SETTINGSSCREEN_H_

#include <maapi.h>
#include <MAUtil/util.h>

// Include all the wrappers.
#include <NativeUI/Widgets.h>

using namespace NativeUI;

class SettingsScreen:
	public Screen,
	public ListViewListener,
	public CheckBoxListener,
	public ButtonListener
{
public:
	/**
	 * Constructor.
	 */
	SettingsScreen();

	/**
	 * Destructor.
	 */
	~SettingsScreen();

	/**
	 * Sets the list view used on the ListView screen.
	 */
	void setListView(ListView* listView);
private:

	/**
	 * Creates and adds main layout to the screen.
	 */
	void createMainLayout();

	/**
	 * Create ListView properties layout
	 */
	void createListViewPropertiesLayout();

	/**
	 * Create ListView section properties layout
	 */
	void createListViewSectionPropertiesLayout();

	/**
	 * Create ListView item properties layout
	 */
	void createListViewItemPropertiesLayout();
	void createListViewItemTextLayout(VerticalLayout* parent);
	void createListViewItemFontColorLayout(VerticalLayout* parent);
	void createListViewItemFontSizeLayout(VerticalLayout* parent);
	void createListViewItemEditModeLayout(VerticalLayout* parent);
	void createListViewItemDeleteButtonTextLayout(VerticalLayout* parent);
	void createListViewItemSelectedAnimationLayout(VerticalLayout* parent);
	void createListViewItemUnselectedAnimationLayout(VerticalLayout* parent);
	void createListViewItemHighlightedAnimationLayout(VerticalLayout* parent);
	void createListViewItemUnhighlightedAnimationLayout(VerticalLayout* parent);
	void createListViewItemAccessoryTypeIntLayout(VerticalLayout* parent);
	void createListViewItemAccessoryTypeEditModeLayout(VerticalLayout* parent);
	void createListViewItemEditStyleLayout(VerticalLayout* parent);

	/**
	 * Updates the widgets related to the selected list view item.
	 */
	void updateSelectedItemUI();

	/**
	 * Sets the accessory type int list view item property for the current selected item.
	 */
	void setAccessoryTypeIntProperty(ListViewItemAccessory accessoryType);

	/**
	 * Sets the accessory type edit mode list view item property for the current selected item.
	 */
	void setAccessoryTypeEditModeProperty(ListViewItemAccessory accessoryType);

	/**
	 * Sets the edit mode of the current selected list view item.
	 */
	void setEditModeProperty(ListViewItemEditStyle editMode);

    /** from ListViewListener
     * This method is called when a list view item is clicked.
     * @param listView The list view object that generated the event.
     * @param listViewItem The ListViewItem object that was clicked.
     */
	virtual void listViewItemClicked(
        ListView* listView,
        ListViewItem* listViewItem);

    /**
     * This method is called when the state of the check box was changed
     * by the user.
     * @param checkBox The check box object that generated the event.
     * @param state True if the check box is checked, false otherwise.
     */
    virtual void SettingsScreen::checkBoxStateChanged(
        CheckBox* checkBox,
        bool state);

    /**
     * This method is called if the touch-up event was inside the
     * bounds of the button.
     * Platform: iOS and Android.
     * @param button The button object that generated the event.
     */
    virtual void buttonClicked(Widget* button);
private:
	/**
	 * Main layout.
	 */
	VerticalLayout* mMainLayout;

	/**
	 * The list contains all the properties of the list view, list view section and list view item.
	 */
	RelativeLayout *mPropertiesRelativeLayout;

	/**
	 * The application alphabetical list view.
	 */
	ListView* mListView;

	/**
	 * The current list view section in focus.
	 */
	ListViewSection* mCurrentListViewSection;

	/**
	 * The current application selected list view item.
	 */
	ListViewItem* mCurrentListViewItem;

	/**
	 * If checked, the list view allows selection.
	 */
	CheckBox* mListViewAllowSelectionCheckbox;

	/**
	 * The label that contains the current list view section.
	 */
	Label* mListViewSectionLabel;

	/**
	 * The label that contains the selected list view item text.
	 */
	Label* mCurrentListViewItemLabel;

	/**
	 * A radio group for the list mode selection.
	 */
	CheckBox* mDisplayCheckBox;
	CheckBox* mEditCheckBox;

	/**
	 * Contains the current list view section title.
	 */
	EditBox* mCurrentListViewSectionTitleEditBox;

	/**
	 * Sets the current list view section title.
	 */
	Button* mSetListViewSectionTitleButton;

	/**
	 * Contains the current list view section header text.
	 */
	EditBox* mCurrentListViewSectionHeaderEditBox;

	/**
	 * Sets the current list view section header text.
	 */
	Button* mSetListViewSectionHeaderButton;

	/**
	 * Contains the current list view section footer text.
	 */
	EditBox* mCurrentListViewSectionFooterEditBox;

	/**
	 * Sets the current list view section footer text.
	 */
	Button* mSetListViewSectionFooterButton;

	/**
	 * Contains the list view item text to be set.
	 */
	EditBox* mSetListViewItemTextEditBox;

	/**
	 * Sets the current list view item text.
	 */
	Button* mSetListViewItemTextButton;

	/**
	 * Contains the list view item text to be set.
	 */
	EditBox* mSetListViewItemFontColorEditBox;

	/**
	 * Sets the current list view item text.
	 */
	Button* mSetListViewItemFontColorButton;

	/**
	 * Contains the list view item text to be set.
	 */
	EditBox* mSetListViewItemFontSizeEditBox;

	/**
	 * Sets the current list view item text.
	 */
	Button* mSetListViewItemFontSizeButton;

	/**
	 * If checked, the list view item is in edit mode.
	 */
	CheckBox* mListViewItemEditModeCheckbox;

	/**
	 * Sets the current list view item delete button title.
	 */
	Button* mSetListViewItemDeleteTitleButton;

	/**
	 * Contains the list view item delete button title.
	 */
	EditBox* mSetListViewItemDeleteTitleEditBox;

	/**
	 * If checked, the list view item will animate the selected state.
	 */
	CheckBox* mListViewItemSelectedCheckbox;

	/**
	 * If checked, the list view item will animate the unselected state.
	 */
	CheckBox* mListViewItemUnselectedCheckbox;

	/**
	 * If checked, the list view item will animate the highlighted state.
	 */
	CheckBox* mListViewItemHighlightedCheckbox;

	/**
	 * If checked, the list view item will animate the unhighlighted state.
	 */
	CheckBox* mListViewItemUnhighlightedCheckbox;

	/**
	 * If checked, set the list view item accessory type int to none.
	 */
	CheckBox* mListViewItemAccessoryTypeIntNoneCheckbox;

	/**
	 * If checked, set the list view item accessory type int to disclosure.
	 */
	CheckBox* mListViewItemAccessoryTypeIntDisclosureCheckbox;

	/**
	 * If checked, set the list view item accessory type int to detail.
	 */
	CheckBox* mListViewItemAccessoryTypeIntDetailCheckbox;

	/**
	 * If checked, set the list view item accessory type int to checkmark.
	 */
	CheckBox* mListViewItemAccessoryTypeIntCheckmarkCheckbox;

	/**
	 * If checked, set the list view item accessory type edit mode to none.
	 */
	CheckBox* mListViewItemAccessoryTypeEditModeNoneCheckbox;

	/**
	 * If checked, set the list view item accessory type edit mode to disclosure.
	 */
	CheckBox* mListViewItemAccessoryTypeEditModeDisclosureCheckbox;

	/**
	 * If checked, set the list view item accessory type edit mode to detail.
	 */
	CheckBox* mListViewItemAccessoryTypeEditModeDetailCheckbox;

	/**
	 * If checked, set the list view item accessory type edit mode to checkmark.
	 */
	CheckBox* mListViewItemAccessoryTypeEditModeCheckmarkCheckbox;

	/**
	 * If checked, sets the list view item edit style to none.
	 */
	CheckBox* mListViewItemEditStyleNoneCheckbox;

	/**
	 * If checked, sets the list view item edit style to delete.
	 */
	CheckBox* mListViewItemEditStyleDeleteCheckbox;

	/**
	 * If checked, sets the list view item edit style to insert.
	 */
	CheckBox* mListViewItemEditStyleInsertCheckbox;
};

#endif /* SETTINGSSCREEN_H_ */
