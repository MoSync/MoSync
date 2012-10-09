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
#include "ListScreenListener.h"
#include "OptionsBoxListener.h"

// Forward class declaration
class OptionsBox;

using namespace NativeUI;

class SettingsScreen:
	public Screen,
	public ListScreenListener,
	public CheckBoxListener,
	public ButtonListener,
	public OptionsBoxListener,
	public EditBoxListener
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
	void createListViewSectionHeaderPropertiesLayout(VerticalLayout* parent);
	void createListViewSectionFooterPropertiesLayout(VerticalLayout* parent);

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
	void createListViewItemSelectionStyleLayout(VerticalLayout* parent);

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
     * Platform: iOS, Android and Windows Phone 7.
     * @param button The button object that generated the event.
     */
    virtual void buttonClicked(Widget* button);

	/**
	 * This method is called when a list item is clicked.
	 * @param parentSection The parent section of the list view item clicked.
	 * @param item The list view item clicked.
	 */
	virtual void listScreenItemClicked(ListViewSection* &parentSection, ListViewItem* &listViewItem);

	/**
	 * Will show an options box so user can select the list view item selection
	 * style.
	 * Works on iOS platform only.
	 */
	void showSelectionStyleOptionBox();

	/**
	 * Show currently used selection style for an list view item.
	 * Works on iOS platform.
	 */
	void showSelectionStyleMessageBox();

	/**
	 * Change all the list view's items selection style to a given value.
	 * @param style Style to set.
	 */
	void changeSelectionStyle(ListViewItemSelectionStyle style);

	/**
	 * This method is called when a button from options dialog was
	 * clicked.
	 * This method is not called if the destructive or cancel button were
	 * clicked.
	 * @param buttonIndex The index of the button that was clicked.
	 * @param buttonTitle The title of the button that was clicked.
	 */
	virtual void optionsBoxButtonClicked(const int buttonIndex,
		const MAUtil::WString& buttonTitle);

    /**
     * This method is called when the return button was pressed.
     * On iphone platform the virtual keyboard is not hidden after
     * receiving this event.
     * @param editBox The edit box object that generated the event.
     */
    virtual void editBoxReturn(EditBox* editBox);

private:
	/**
	 * Main layout.
	 */
	VerticalLayout* mMainLayout;

	/**
	 * The main list (located on the ListScreen).
	 */
	ListView* mListView;

	/**
	 * The list contains all the properties of the list view, list view section and list view item.
	 */
	ListView *mPropertiesListView;

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

	/**
	 * Sets the section in focus header background color.
	 */
	Button* mSetSectionHeaderBackgroundColor;

	/**
	 * Sets the section in focus header font size.
	 */
	Button* mSetSectionHeaderFontSize;

	/**
	 * Sets the section in focus header font color.
	 */
	Button* mSetSectionHeaderFontColor;

	/**
	 * Contains the section in focus header background color.
	 */
	EditBox* mHeaderBackgroundColorEditBox;

	/**
	 * Contains the section in focus header font size.
	 */
	EditBox* mHeaderFontSizeEditBox;

	/**
	 * Contains the section in focus header font color.
	 */
	EditBox* mHeaderFontColorEditBox;

	/**
	 * Sets the section in focus footer background color.
	 */
	Button* mSetSectionFooterBackgroundColor;

	/**
	 * Sets the section in focus footer font size.
	 */
	Button* mSetSectionFooterFontSize;

	/**
	 * Sets the section in focus footer font color.
	 */
	Button* mSetSectionFooterFontColor;

	/**
	 * Sets the selection style for an list view item.
	 * Used to test tMAW_LIST_VIEW_ITEM_SELECTION_STYLE setter property.
	 */
	Button* mSetSelectionStyle;

	/**
	 * Gets the selection style for an list view item.
	 * Used to test tMAW_LIST_VIEW_ITEM_SELECTION_STYLE getter property.
	 */
	Button* mGetSelectionStyle;

	/**
	 * Contains the section in focus footer background color.
	 */
	EditBox* mFooterBackgroundColorEditBox;

	/**
	 * Contains the section in focus footer font size.
	 */
	EditBox* mFooterFontSizeEditBox;

	/**
	 * Contains the section in focus footer font color.
	 */
	EditBox* mFooterFontColorEditBox;
};

#endif /* SETTINGSSCREEN_H_ */
