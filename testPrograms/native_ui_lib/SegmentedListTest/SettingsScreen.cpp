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
 *  Created on: Sept 28, 2012
 *      Author: Spiridon Alexandru
 */

#include <NativeUI/ListViewSection.h>

#include "SettingsScreen.h"
#include "OptionsBox.h"
#include "Util.h"

/**
 * Constructor.
 */
SettingsScreen::SettingsScreen():
	Screen(),
	mMainLayout(NULL),
	mPropertiesListView(NULL),
	mListViewAllowSelectionCheckbox(NULL),
	mListViewSectionLabel(NULL),
	mCurrentListViewItemLabel(NULL),
	mDisplayCheckBox(NULL),
	mEditCheckBox(NULL),
	mSetListViewSectionTitleButton(NULL),
	mSetListViewSectionHeaderButton(NULL),
	mSetListViewSectionFooterButton(NULL),
	mCurrentListViewSectionTitleEditBox(NULL),
	mCurrentListViewSectionHeaderEditBox(NULL),
	mCurrentListViewSectionFooterEditBox(NULL),
	mSetListViewItemTextEditBox(NULL),
	mSetListViewItemTextButton(NULL),
	mSetListViewItemFontColorEditBox(NULL),
	mSetListViewItemFontColorButton(NULL),
	mSetListViewItemFontSizeEditBox(NULL),
	mSetListViewItemFontSizeButton(NULL),
	mListViewItemEditModeCheckbox(NULL),
	mSetListViewItemDeleteTitleButton(NULL),
	mSetListViewItemDeleteTitleEditBox(NULL),
	mListViewItemSelectedCheckbox(NULL),
	mListViewItemUnselectedCheckbox(NULL),
	mListViewItemHighlightedCheckbox(NULL),
	mListViewItemUnhighlightedCheckbox(NULL),
	mListViewItemAccessoryTypeIntNoneCheckbox(NULL),
	mListViewItemAccessoryTypeIntDisclosureCheckbox(NULL),
	mListViewItemAccessoryTypeIntDetailCheckbox(NULL),
	mListViewItemAccessoryTypeIntCheckmarkCheckbox(NULL),
	mListViewItemAccessoryTypeEditModeNoneCheckbox(NULL),
	mListViewItemAccessoryTypeEditModeDisclosureCheckbox(NULL),
	mListViewItemAccessoryTypeEditModeDetailCheckbox(NULL),
	mListViewItemAccessoryTypeEditModeCheckmarkCheckbox(NULL),
	mListViewItemEditStyleNoneCheckbox(NULL),
	mListViewItemEditStyleDeleteCheckbox(NULL),
	mListViewItemEditStyleInsertCheckbox(NULL),
	mSetSectionHeaderBackgroundColor(NULL),
	mSetSectionHeaderFontSize(NULL),
	mSetSectionHeaderFontColor(NULL),
	mHeaderBackgroundColorEditBox(NULL),
	mHeaderFontSizeEditBox(NULL),
	mHeaderFontColorEditBox(NULL),
	mSetSectionFooterBackgroundColor(NULL),
	mSetSectionFooterFontSize(NULL),
	mSetSectionFooterFontColor(NULL),
	mSetSelectionStyle(NULL),
	mGetSelectionStyle(NULL),
	mFooterBackgroundColorEditBox(NULL),
	mFooterFontSizeEditBox(NULL),
	mFooterFontColorEditBox(NULL)
{
	createMainLayout();

	mListViewAllowSelectionCheckbox->addCheckBoxListener(this);
	mDisplayCheckBox->addCheckBoxListener(this);
	mEditCheckBox->addCheckBoxListener(this);
	if (isWindowsPhone())
	{
		mSetListViewSectionTitleButton->addButtonListener(this);
	}
	mSetListViewSectionHeaderButton->addButtonListener(this);
	mSetListViewSectionFooterButton->addButtonListener(this);
	mSetListViewItemTextButton->addButtonListener(this);
	mSetListViewItemFontColorButton->addButtonListener(this);
	mSetListViewItemFontSizeButton->addButtonListener(this);
	mCurrentListViewSectionTitleEditBox->addEditBoxListener(this);
	mCurrentListViewSectionHeaderEditBox->addEditBoxListener(this);
	mCurrentListViewSectionFooterEditBox->addEditBoxListener(this);
	mSetListViewItemTextEditBox->addEditBoxListener(this);
	mSetListViewItemFontColorEditBox->addEditBoxListener(this);
	mSetListViewItemFontSizeEditBox->addEditBoxListener(this);
	if (isIOS())
	{
		mListViewItemEditModeCheckbox->addCheckBoxListener(this);
		mSetListViewItemDeleteTitleButton->addButtonListener(this);
		mListViewItemSelectedCheckbox->addCheckBoxListener(this);
		mListViewItemUnselectedCheckbox->addCheckBoxListener(this);
		mListViewItemHighlightedCheckbox->addCheckBoxListener(this);
		mListViewItemUnhighlightedCheckbox->addCheckBoxListener(this);
		mListViewItemAccessoryTypeIntNoneCheckbox->addCheckBoxListener(this);
		mListViewItemAccessoryTypeIntDisclosureCheckbox->addCheckBoxListener(this);
		mListViewItemAccessoryTypeIntDetailCheckbox->addCheckBoxListener(this);
		mListViewItemAccessoryTypeIntCheckmarkCheckbox->addCheckBoxListener(this);
		mListViewItemAccessoryTypeEditModeNoneCheckbox->addCheckBoxListener(this);
		mListViewItemAccessoryTypeEditModeDisclosureCheckbox->addCheckBoxListener(this);
		mListViewItemAccessoryTypeEditModeDetailCheckbox->addCheckBoxListener(this);
		mListViewItemAccessoryTypeEditModeCheckmarkCheckbox->addCheckBoxListener(this);
		mListViewItemEditStyleNoneCheckbox->addCheckBoxListener(this);
		mListViewItemEditStyleDeleteCheckbox->addCheckBoxListener(this);
		mListViewItemEditStyleInsertCheckbox->addCheckBoxListener(this);
		mSetSelectionStyle->addButtonListener(this);
		mGetSelectionStyle->addButtonListener(this);
		mSetListViewItemDeleteTitleEditBox->addEditBoxListener(this);
		OptionsBox* optionBox = OptionsBox::getInstance();
		optionBox->addOptionsBoxListener(this);
	}
	if (isWindowsPhone() || isAndroid())
	{
		mSetSectionHeaderBackgroundColor->addButtonListener(this);
		mSetSectionHeaderFontSize->addButtonListener(this);
		mSetSectionHeaderFontColor->addButtonListener(this);
		mSetSectionFooterBackgroundColor->addButtonListener(this);
		mSetSectionFooterFontSize->addButtonListener(this);
		mSetSectionFooterFontColor->addButtonListener(this);
		mHeaderBackgroundColorEditBox->addEditBoxListener(this);
		mHeaderFontSizeEditBox->addEditBoxListener(this);
		mHeaderFontColorEditBox->addEditBoxListener(this);
		mFooterBackgroundColorEditBox->addEditBoxListener(this);
		mFooterFontSizeEditBox->addEditBoxListener(this);
		mFooterFontColorEditBox->addEditBoxListener(this);
	}
}

/**
 * Destructor.
 */
SettingsScreen::~SettingsScreen()
{
	mListViewAllowSelectionCheckbox->removeCheckBoxListener(this);
	mDisplayCheckBox->removeCheckBoxListener(this);
	mEditCheckBox->removeCheckBoxListener(this);
	if (isWindowsPhone())
	{
		mSetListViewSectionTitleButton->removeButtonListener(this);
	}
	mSetListViewSectionHeaderButton->removeButtonListener(this);
	mSetListViewSectionFooterButton->removeButtonListener(this);
	mSetListViewItemTextButton->removeButtonListener(this);
	mSetListViewItemFontColorButton->removeButtonListener(this);
	mSetListViewItemFontSizeButton->removeButtonListener(this);
	mCurrentListViewSectionTitleEditBox->removeEditBoxListener(this);
	mCurrentListViewSectionFooterEditBox->removeEditBoxListener(this);
	mSetListViewItemTextEditBox->removeEditBoxListener(this);
	mSetListViewItemFontColorEditBox->removeEditBoxListener(this);
	mSetListViewItemFontSizeEditBox->removeEditBoxListener(this);

	if (isIOS())
	{
		mListViewItemEditModeCheckbox->removeCheckBoxListener(this);
		mSetListViewItemDeleteTitleButton->removeButtonListener(this);
		mListViewItemSelectedCheckbox->removeCheckBoxListener(this);
		mListViewItemUnselectedCheckbox->removeCheckBoxListener(this);
		mListViewItemHighlightedCheckbox->removeCheckBoxListener(this);
		mListViewItemUnhighlightedCheckbox->removeCheckBoxListener(this);
		mListViewItemAccessoryTypeIntNoneCheckbox->removeCheckBoxListener(this);
		mListViewItemAccessoryTypeIntDisclosureCheckbox->removeCheckBoxListener(this);
		mListViewItemAccessoryTypeIntDetailCheckbox->removeCheckBoxListener(this);
		mListViewItemAccessoryTypeIntCheckmarkCheckbox->removeCheckBoxListener(this);
		mListViewItemAccessoryTypeEditModeNoneCheckbox->removeCheckBoxListener(this);
		mListViewItemAccessoryTypeEditModeDisclosureCheckbox->removeCheckBoxListener(this);
		mListViewItemAccessoryTypeEditModeDetailCheckbox->removeCheckBoxListener(this);
		mListViewItemAccessoryTypeEditModeCheckmarkCheckbox->removeCheckBoxListener(this);
		mListViewItemEditStyleNoneCheckbox->removeCheckBoxListener(this);
		mListViewItemEditStyleDeleteCheckbox->removeCheckBoxListener(this);
		mListViewItemEditStyleInsertCheckbox->removeCheckBoxListener(this);
		mSetSelectionStyle->removeButtonListener(this);
		mGetSelectionStyle->removeButtonListener(this);
		mSetListViewItemDeleteTitleEditBox->removeEditBoxListener(this);
		OptionsBox* optionBox = OptionsBox::getInstance();
		optionBox->addOptionsBoxListener(this);
		OptionsBox::destroyInstance();
	}
	if (isWindowsPhone() || isAndroid())
	{
		mSetSectionHeaderBackgroundColor->removeButtonListener(this);
		mSetSectionHeaderFontSize->removeButtonListener(this);
		mSetSectionHeaderFontColor->removeButtonListener(this);
		mSetSectionFooterBackgroundColor->removeButtonListener(this);
		mSetSectionFooterFontSize->removeButtonListener(this);
		mSetSectionFooterFontColor->removeButtonListener(this);
		mHeaderBackgroundColorEditBox->removeEditBoxListener(this);
		mHeaderFontSizeEditBox->removeEditBoxListener(this);
		mHeaderFontColorEditBox->removeEditBoxListener(this);
		mFooterBackgroundColorEditBox->removeEditBoxListener(this);
		mFooterFontSizeEditBox->removeEditBoxListener(this);
		mFooterFontColorEditBox->removeEditBoxListener(this);
	}
}

/**
 * Creates and adds main layout to the screen.
 */
void SettingsScreen::createMainLayout()
{
	mMainLayout = new VerticalLayout();
	setMainWidget(mMainLayout);

	mPropertiesListView = new ListView();

	createListViewPropertiesLayout();
	createListViewSectionPropertiesLayout();
	createListViewItemPropertiesLayout();

	mMainLayout->addChild(mPropertiesListView);
}

/**
 * Create ListView properties layout
 */
void SettingsScreen::createListViewPropertiesLayout()
{
	VerticalLayout* listViewPropertiesVerticalLayout = new VerticalLayout();
	listViewPropertiesVerticalLayout->wrapContentVertically();
	Label* mListViewPropertiesLabel = new Label();
	mListViewPropertiesLabel->setText("ListView properties");
	mListViewPropertiesLabel->setFontSize(TITLE_FONT_SIZE);
	mListViewPropertiesLabel->setFontColor(0xFF0000);
	listViewPropertiesVerticalLayout->addChild(mListViewPropertiesLabel);

	// create the allow selection layout
	HorizontalLayout* allowSelectionLayout = new HorizontalLayout();
	allowSelectionLayout->wrapContentVertically();
	mListViewAllowSelectionCheckbox = new CheckBox();
	mListViewAllowSelectionCheckbox->setState(true);
	Label* allowSelectionLabel = new Label();
	allowSelectionLabel->setText("Allow selection");
	allowSelectionLayout->addChild(mListViewAllowSelectionCheckbox);
	allowSelectionLayout->addChild(allowSelectionLabel);
	listViewPropertiesVerticalLayout->addChild(allowSelectionLayout);

	if (isIOS())
	{
		listViewPropertiesVerticalLayout->wrapContentVertically();

		// create the mode selection layout
		HorizontalLayout* modeSelectionLayout = new HorizontalLayout();
		modeSelectionLayout->wrapContentVertically();
		Label* setModeLabel = new Label();
		setModeLabel->setText("Set mode: ");
		modeSelectionLayout->addChild(setModeLabel);
		mDisplayCheckBox = new CheckBox();
		mDisplayCheckBox->setState(true);
		Label* setDisplayLabel = new Label();
		setDisplayLabel->setText("Display");
		modeSelectionLayout->addChild(mDisplayCheckBox);
		modeSelectionLayout->addChild(setDisplayLabel);
		mEditCheckBox = new CheckBox();
		mEditCheckBox->setState(false);
		Label* setEditLabel = new Label();
		setEditLabel->setText("Edit");
		modeSelectionLayout->addChild(mEditCheckBox);
		modeSelectionLayout->addChild(setEditLabel);
		listViewPropertiesVerticalLayout->addChild(modeSelectionLayout);

		createListViewItemSelectionStyleLayout(listViewPropertiesVerticalLayout);
	}

	ListViewItem* mListPropertiesItem = new ListViewItem();
	mListPropertiesItem->setSelectionStyle(LIST_VIEW_ITEM_SELECTION_STYLE_NONE);
	mListPropertiesItem->addChild(listViewPropertiesVerticalLayout);

	mPropertiesListView->addChild(mListPropertiesItem);
}

/**
 * Create ListView section properties layout
 */
void SettingsScreen::createListViewSectionPropertiesLayout()
{
	VerticalLayout* listViewSectionPropertiesVerticalLayout = new VerticalLayout();
	listViewSectionPropertiesVerticalLayout->wrapContentVertically();
	mListViewSectionLabel = new Label();
	mListViewSectionLabel->setText("No section in focus");
	mListViewSectionLabel->setFontSize(TITLE_FONT_SIZE);
	mListViewSectionLabel->setFontColor(0xFF0000);
	listViewSectionPropertiesVerticalLayout->addChild(mListViewSectionLabel);

	if (isWindowsPhone())
	{
		listViewSectionPropertiesVerticalLayout->wrapContentVertically();
		// create the current section title layout
		HorizontalLayout* currentSectionTitleLayout = new HorizontalLayout();
		currentSectionTitleLayout->wrapContentVertically();
		mSetListViewSectionTitleButton = new Button();
		mSetListViewSectionTitleButton->setText("Set title");
		mSetListViewSectionTitleButton->fillSpaceHorizontally();
		currentSectionTitleLayout->addChild(mSetListViewSectionTitleButton);
		mCurrentListViewSectionTitleEditBox = new EditBox();
		mCurrentListViewSectionTitleEditBox->fillSpaceHorizontally();
		currentSectionTitleLayout->addChild(mCurrentListViewSectionTitleEditBox);
		listViewSectionPropertiesVerticalLayout->addChild(currentSectionTitleLayout);
	}

	// create the current section header text layout
	HorizontalLayout* currentSectionHeaderTextLayout = new HorizontalLayout();
	currentSectionHeaderTextLayout->wrapContentVertically();
	mSetListViewSectionHeaderButton = new Button();
	mSetListViewSectionHeaderButton->setText("Set header");
	mSetListViewSectionHeaderButton->fillSpaceHorizontally();
	currentSectionHeaderTextLayout->addChild(mSetListViewSectionHeaderButton);
	mCurrentListViewSectionHeaderEditBox = new EditBox();
	mCurrentListViewSectionHeaderEditBox->fillSpaceHorizontally();
	currentSectionHeaderTextLayout->addChild(mCurrentListViewSectionHeaderEditBox);
	listViewSectionPropertiesVerticalLayout->addChild(currentSectionHeaderTextLayout);

	// create the current section footer text layout
	HorizontalLayout* currentSectionFooterTextLayout = new HorizontalLayout();
	currentSectionFooterTextLayout->wrapContentVertically();
	mSetListViewSectionFooterButton = new Button();
	mSetListViewSectionFooterButton->setText("Set footer");
	mSetListViewSectionFooterButton->fillSpaceHorizontally();
	currentSectionFooterTextLayout->addChild(mSetListViewSectionFooterButton);
	mCurrentListViewSectionFooterEditBox = new EditBox();
	mCurrentListViewSectionFooterEditBox->fillSpaceHorizontally();
	currentSectionFooterTextLayout->addChild(mCurrentListViewSectionFooterEditBox);
	listViewSectionPropertiesVerticalLayout->addChild(currentSectionFooterTextLayout);

	if (isWindowsPhone() || isAndroid())
	{
		listViewSectionPropertiesVerticalLayout->wrapContentVertically();
		createListViewSectionHeaderPropertiesLayout(listViewSectionPropertiesVerticalLayout);
		createListViewSectionFooterPropertiesLayout(listViewSectionPropertiesVerticalLayout);
	}

	ListViewItem* mListSectionPropertiesItem = new ListViewItem();
	mListSectionPropertiesItem->setSelectionStyle(LIST_VIEW_ITEM_SELECTION_STYLE_NONE);
	mListSectionPropertiesItem->addChild(listViewSectionPropertiesVerticalLayout);

	mPropertiesListView->addChild(mListSectionPropertiesItem);
}

void SettingsScreen::createListViewSectionHeaderPropertiesLayout(VerticalLayout* listViewSectionPropertiesVerticalLayout)
{
	// create the current section header background color layout
	HorizontalLayout* currentSectionHeaderBackgroundColorLayout = new HorizontalLayout();
	currentSectionHeaderBackgroundColorLayout->wrapContentVertically();
	mSetSectionHeaderBackgroundColor = new Button();
	mSetSectionHeaderBackgroundColor->setText("Header bg");
	mSetSectionHeaderBackgroundColor->fillSpaceHorizontally();
	currentSectionHeaderBackgroundColorLayout->addChild(mSetSectionHeaderBackgroundColor);
	mHeaderBackgroundColorEditBox = new EditBox();
	mHeaderBackgroundColorEditBox->setPlaceholder("ex: 0xFF0000");
	mHeaderBackgroundColorEditBox->fillSpaceHorizontally();
	currentSectionHeaderBackgroundColorLayout->addChild(mHeaderBackgroundColorEditBox);
	listViewSectionPropertiesVerticalLayout->addChild(currentSectionHeaderBackgroundColorLayout);

	// create the current section header font size layout
	HorizontalLayout* currentSectionHeaderFontSizeLayout = new HorizontalLayout();
	currentSectionHeaderFontSizeLayout->wrapContentVertically();
	mSetSectionHeaderFontSize = new Button();
	mSetSectionHeaderFontSize->setText("Header font sz");
	mSetSectionHeaderFontSize->fillSpaceHorizontally();
	currentSectionHeaderFontSizeLayout->addChild(mSetSectionHeaderFontSize);
	mHeaderFontSizeEditBox = new EditBox();
	mHeaderFontSizeEditBox->setInputMode(EDIT_BOX_INPUT_MODE_NUMERIC);
	mHeaderFontSizeEditBox->fillSpaceHorizontally();
	currentSectionHeaderFontSizeLayout->addChild(mHeaderFontSizeEditBox);
	listViewSectionPropertiesVerticalLayout->addChild(currentSectionHeaderFontSizeLayout);

	// create the current section header font color layout
	HorizontalLayout* currentSectionHeaderFontColorLayout = new HorizontalLayout();
	currentSectionHeaderFontColorLayout->wrapContentVertically();
	mSetSectionHeaderFontColor = new Button();
	mSetSectionHeaderFontColor->setText("Header font color");
	mSetSectionHeaderFontColor->fillSpaceHorizontally();
	currentSectionHeaderFontColorLayout->addChild(mSetSectionHeaderFontColor);
	mHeaderFontColorEditBox = new EditBox();
	mHeaderFontColorEditBox->setPlaceholder("ex: 0xFF0000");
	mHeaderFontColorEditBox->fillSpaceHorizontally();
	currentSectionHeaderFontColorLayout->addChild(mHeaderFontColorEditBox);
	listViewSectionPropertiesVerticalLayout->addChild(currentSectionHeaderFontColorLayout);
}

void SettingsScreen::createListViewSectionFooterPropertiesLayout(VerticalLayout* listViewSectionPropertiesVerticalLayout)
{
	// create the current section footer background color layout
	HorizontalLayout* currentSectionFooterBackgroundColorLayout = new HorizontalLayout();
	currentSectionFooterBackgroundColorLayout->wrapContentVertically();
	mSetSectionFooterBackgroundColor = new Button();
	mSetSectionFooterBackgroundColor->setText("Footer bg");
	mSetSectionFooterBackgroundColor->fillSpaceHorizontally();
	currentSectionFooterBackgroundColorLayout->addChild(mSetSectionFooterBackgroundColor);
	mFooterBackgroundColorEditBox = new EditBox();
	mFooterBackgroundColorEditBox->setPlaceholder("ex: 0xFF0000");
	mFooterBackgroundColorEditBox->fillSpaceHorizontally();
	currentSectionFooterBackgroundColorLayout->addChild(mFooterBackgroundColorEditBox);
	listViewSectionPropertiesVerticalLayout->addChild(currentSectionFooterBackgroundColorLayout);

	// create the current section footer font size layout
	HorizontalLayout* currentSectionFooterFontSizeLayout = new HorizontalLayout();
	currentSectionFooterFontSizeLayout->wrapContentVertically();
	mSetSectionFooterFontSize = new Button();
	mSetSectionFooterFontSize->setText("Footer font sz");
	mSetSectionFooterFontSize->fillSpaceHorizontally();
	currentSectionFooterFontSizeLayout->addChild(mSetSectionFooterFontSize);
	mFooterFontSizeEditBox = new EditBox();
	mFooterFontSizeEditBox->setInputMode(EDIT_BOX_INPUT_MODE_NUMERIC);
	mFooterFontSizeEditBox->fillSpaceHorizontally();
	currentSectionFooterFontSizeLayout->addChild(mFooterFontSizeEditBox);
	listViewSectionPropertiesVerticalLayout->addChild(currentSectionFooterFontSizeLayout);

	// create the current section footer font color layout
	HorizontalLayout* currentSectionFooterFontColorLayout = new HorizontalLayout();
	currentSectionFooterFontColorLayout->wrapContentVertically();
	mSetSectionFooterFontColor = new Button();
	mSetSectionFooterFontColor->setText("Footer font color");
	mSetSectionFooterFontColor->fillSpaceHorizontally();
	currentSectionFooterFontColorLayout->addChild(mSetSectionFooterFontColor);
	mFooterFontColorEditBox = new EditBox();
	mFooterFontColorEditBox->setPlaceholder("ex: 0xFF0000");
	mFooterFontColorEditBox->fillSpaceHorizontally();
	currentSectionFooterFontColorLayout->addChild(mFooterFontColorEditBox);
	listViewSectionPropertiesVerticalLayout->addChild(currentSectionFooterFontColorLayout);
}

/**
 * Create ListView item properties layout
 */
void SettingsScreen::createListViewItemPropertiesLayout()
{
	VerticalLayout* listViewItemPropertiesVerticalLayout = new VerticalLayout();
	listViewItemPropertiesVerticalLayout->wrapContentVertically();
	mCurrentListViewItemLabel = new Label();
	mCurrentListViewItemLabel->setText("No item selected");
	mCurrentListViewItemLabel->setFontSize(TITLE_FONT_SIZE);
	mCurrentListViewItemLabel->setFontColor(0xFF0000);
	listViewItemPropertiesVerticalLayout->addChild(mCurrentListViewItemLabel);

	createListViewItemTextLayout(listViewItemPropertiesVerticalLayout);
	createListViewItemFontColorLayout(listViewItemPropertiesVerticalLayout);
	createListViewItemFontSizeLayout(listViewItemPropertiesVerticalLayout);

	if (isIOS())
	{
		listViewItemPropertiesVerticalLayout->wrapContentVertically();

		createListViewItemEditModeLayout(listViewItemPropertiesVerticalLayout);
		createListViewItemDeleteButtonTextLayout(listViewItemPropertiesVerticalLayout);
		createListViewItemSelectedAnimationLayout(listViewItemPropertiesVerticalLayout);
		createListViewItemUnselectedAnimationLayout(listViewItemPropertiesVerticalLayout);
		createListViewItemHighlightedAnimationLayout(listViewItemPropertiesVerticalLayout);
		createListViewItemUnhighlightedAnimationLayout(listViewItemPropertiesVerticalLayout);
		createListViewItemAccessoryTypeIntLayout(listViewItemPropertiesVerticalLayout);
		createListViewItemAccessoryTypeEditModeLayout(listViewItemPropertiesVerticalLayout);
		createListViewItemEditStyleLayout(listViewItemPropertiesVerticalLayout);
	}

	ListViewItem* mListItemProperties = new ListViewItem();
	mListItemProperties->setSelectionStyle(LIST_VIEW_ITEM_SELECTION_STYLE_NONE);
	mListItemProperties->addChild(listViewItemPropertiesVerticalLayout);

	mPropertiesListView->addChild(mListItemProperties);
}

void SettingsScreen::createListViewItemTextLayout(VerticalLayout* listViewItemPropertiesVerticalLayout)
{
	HorizontalLayout* currentListViewItemTextLayout = new HorizontalLayout();
	currentListViewItemTextLayout->wrapContentVertically();
	mSetListViewItemTextButton = new Button();
	mSetListViewItemTextButton->setText("Set text");
	mSetListViewItemTextButton->fillSpaceHorizontally();
	currentListViewItemTextLayout->addChild(mSetListViewItemTextButton);
	mSetListViewItemTextEditBox = new EditBox();
	mSetListViewItemTextEditBox->fillSpaceHorizontally();
	currentListViewItemTextLayout->addChild(mSetListViewItemTextEditBox);
	listViewItemPropertiesVerticalLayout->addChild(currentListViewItemTextLayout);
}

void SettingsScreen::createListViewItemFontColorLayout(VerticalLayout* listViewItemPropertiesVerticalLayout)
{
	HorizontalLayout* currentListViewItemFontColorLayout = new HorizontalLayout();
	currentListViewItemFontColorLayout->wrapContentVertically();
	mSetListViewItemFontColorButton = new Button();
	mSetListViewItemFontColorButton->setText("Set font color");
	mSetListViewItemFontColorButton->fillSpaceHorizontally();
	currentListViewItemFontColorLayout->addChild(mSetListViewItemFontColorButton);
	mSetListViewItemFontColorEditBox = new EditBox();
	mSetListViewItemFontColorEditBox->fillSpaceHorizontally();
	mSetListViewItemFontColorEditBox->setPlaceholder("ex: 0xFF0000");
	currentListViewItemFontColorLayout->addChild(mSetListViewItemFontColorEditBox);
	listViewItemPropertiesVerticalLayout->addChild(currentListViewItemFontColorLayout);
}

void SettingsScreen::createListViewItemFontSizeLayout(VerticalLayout* listViewItemPropertiesVerticalLayout)
{
	HorizontalLayout* currentListViewItemFontSizeLayout = new HorizontalLayout();
	currentListViewItemFontSizeLayout->wrapContentVertically();
	mSetListViewItemFontSizeButton = new Button();
	mSetListViewItemFontSizeButton->setText("Set font size");
	mSetListViewItemFontSizeButton->fillSpaceHorizontally();
	currentListViewItemFontSizeLayout->addChild(mSetListViewItemFontSizeButton);
	mSetListViewItemFontSizeEditBox = new EditBox();
	mSetListViewItemFontSizeEditBox->fillSpaceHorizontally();
	mSetListViewItemFontSizeEditBox->setInputMode(EDIT_BOX_INPUT_MODE_NUMERIC);
	currentListViewItemFontSizeLayout->addChild(mSetListViewItemFontSizeEditBox);
	listViewItemPropertiesVerticalLayout->addChild(currentListViewItemFontSizeLayout);
}

void SettingsScreen::createListViewItemEditModeLayout(VerticalLayout* listViewItemPropertiesVerticalLayout)
{
	HorizontalLayout* editModeLayout = new HorizontalLayout();
	editModeLayout->wrapContentVertically();
	mListViewItemEditModeCheckbox = new CheckBox();
	mListViewItemEditModeCheckbox->setState(false);
	Label* editModeLabel = new Label();
	editModeLabel->setText("Edit mode");
	editModeLayout->addChild(mListViewItemEditModeCheckbox);
	editModeLayout->addChild(editModeLabel);
	listViewItemPropertiesVerticalLayout->addChild(editModeLayout);
}

void SettingsScreen::createListViewItemDeleteButtonTextLayout(VerticalLayout* listViewItemPropertiesVerticalLayout)
{
	HorizontalLayout* currentListViewItemDeleteButtonLayout = new HorizontalLayout();
	currentListViewItemDeleteButtonLayout->wrapContentVertically();
	mSetListViewItemDeleteTitleButton = new Button();
	mSetListViewItemDeleteTitleButton->setText("Set delete");
	mSetListViewItemDeleteTitleButton->fillSpaceHorizontally();
	currentListViewItemDeleteButtonLayout->addChild(mSetListViewItemDeleteTitleButton);
	mSetListViewItemDeleteTitleEditBox = new EditBox();
	mSetListViewItemDeleteTitleEditBox->fillSpaceHorizontally();
	currentListViewItemDeleteButtonLayout->addChild(mSetListViewItemDeleteTitleEditBox);
	listViewItemPropertiesVerticalLayout->addChild(currentListViewItemDeleteButtonLayout);
}

void SettingsScreen::createListViewItemSelectedAnimationLayout(VerticalLayout* listViewItemPropertiesVerticalLayout)
{
	HorizontalLayout* selectedAnimationLayout = new HorizontalLayout();
	selectedAnimationLayout->wrapContentVertically();
	mListViewItemSelectedCheckbox = new CheckBox();
	mListViewItemSelectedCheckbox->setState(false);
	Label* selectedAnimationLabel = new Label();
	selectedAnimationLabel->setText("Selected animation");
	selectedAnimationLayout->addChild(mListViewItemSelectedCheckbox);
	selectedAnimationLayout->addChild(selectedAnimationLabel);
	listViewItemPropertiesVerticalLayout->addChild(selectedAnimationLayout);
}

void SettingsScreen::createListViewItemUnselectedAnimationLayout(VerticalLayout* listViewItemPropertiesVerticalLayout)
{
	HorizontalLayout* unselectedAnimationLayout = new HorizontalLayout();
	unselectedAnimationLayout->wrapContentVertically();
	mListViewItemUnselectedCheckbox = new CheckBox();
	mListViewItemUnselectedCheckbox->setState(false);
	Label* unselectedAnimationLabel = new Label();
	unselectedAnimationLabel->setText("Unselected animation");
	unselectedAnimationLayout->addChild(mListViewItemUnselectedCheckbox);
	unselectedAnimationLayout->addChild(unselectedAnimationLabel);
	listViewItemPropertiesVerticalLayout->addChild(unselectedAnimationLayout);
}

void SettingsScreen::createListViewItemHighlightedAnimationLayout(VerticalLayout* listViewItemPropertiesVerticalLayout)
{
	HorizontalLayout* highlightedAnimationLayout = new HorizontalLayout();
	highlightedAnimationLayout->wrapContentVertically();
	mListViewItemHighlightedCheckbox = new CheckBox();
	mListViewItemHighlightedCheckbox->setState(false);
	Label* highlightedAnimationLabel = new Label();
	highlightedAnimationLabel->setText("Highlighted animation");
	highlightedAnimationLayout->addChild(mListViewItemHighlightedCheckbox);
	highlightedAnimationLayout->addChild(highlightedAnimationLabel);
	listViewItemPropertiesVerticalLayout->addChild(highlightedAnimationLayout);
}

void SettingsScreen::createListViewItemUnhighlightedAnimationLayout(VerticalLayout* listViewItemPropertiesVerticalLayout)
{
	HorizontalLayout* unhighlightedAnimationLayout = new HorizontalLayout();
	unhighlightedAnimationLayout->wrapContentVertically();
	mListViewItemUnhighlightedCheckbox = new CheckBox();
	mListViewItemUnhighlightedCheckbox->setState(false);
	Label* unhighlightedAnimationLabel = new Label();
	unhighlightedAnimationLabel->setText("Unhighlighted animation");
	unhighlightedAnimationLayout->addChild(mListViewItemUnhighlightedCheckbox);
	unhighlightedAnimationLayout->addChild(unhighlightedAnimationLabel);
	listViewItemPropertiesVerticalLayout->addChild(unhighlightedAnimationLayout);
}

void SettingsScreen::createListViewItemAccessoryTypeIntLayout(VerticalLayout* listViewItemPropertiesVerticalLayout)
{
	Label* accessoryTypeIntLabel = new Label();
	accessoryTypeIntLabel->setText("Accessory type int");
	listViewItemPropertiesVerticalLayout->addChild(accessoryTypeIntLabel);

	HorizontalLayout* accessoryTypeIntLine1Layout = new HorizontalLayout();
	accessoryTypeIntLine1Layout->wrapContentVertically();
	mListViewItemAccessoryTypeIntNoneCheckbox = new CheckBox();
	mListViewItemAccessoryTypeIntNoneCheckbox->setState(true);
	Label* accessoryTypeIntNoneLabel = new Label();
	accessoryTypeIntNoneLabel->setText("None");
	accessoryTypeIntLine1Layout->addChild(mListViewItemAccessoryTypeIntNoneCheckbox);
	accessoryTypeIntLine1Layout->addChild(accessoryTypeIntNoneLabel);
	mListViewItemAccessoryTypeIntDisclosureCheckbox = new CheckBox();
	mListViewItemAccessoryTypeIntDisclosureCheckbox->setState(false);
	Label* accessoryTypeIntDisclosureLabel = new Label();
	accessoryTypeIntDisclosureLabel->setText("Disclosure");
	accessoryTypeIntLine1Layout->addChild(mListViewItemAccessoryTypeIntDisclosureCheckbox);
	accessoryTypeIntLine1Layout->addChild(accessoryTypeIntDisclosureLabel);
	listViewItemPropertiesVerticalLayout->addChild(accessoryTypeIntLine1Layout);

	HorizontalLayout* accessoryTypeIntLine2Layout = new HorizontalLayout();
	accessoryTypeIntLine2Layout->wrapContentVertically();
	mListViewItemAccessoryTypeIntDetailCheckbox = new CheckBox();
	mListViewItemAccessoryTypeIntDetailCheckbox->setState(false);
	Label* accessoryTypeIntDetailLabel = new Label();
	accessoryTypeIntDetailLabel->setText("Detail");
	accessoryTypeIntLine2Layout->addChild(mListViewItemAccessoryTypeIntDetailCheckbox);
	accessoryTypeIntLine2Layout->addChild(accessoryTypeIntDetailLabel);
	mListViewItemAccessoryTypeIntCheckmarkCheckbox = new CheckBox();
	mListViewItemAccessoryTypeIntCheckmarkCheckbox->setState(false);
	Label* accessoryTypeIntCheckmarkLabel = new Label();
	accessoryTypeIntCheckmarkLabel->setText("Checkmark");
	accessoryTypeIntLine2Layout->addChild(mListViewItemAccessoryTypeIntCheckmarkCheckbox);
	accessoryTypeIntLine2Layout->addChild(accessoryTypeIntCheckmarkLabel);
	listViewItemPropertiesVerticalLayout->addChild(accessoryTypeIntLine2Layout);
}

void SettingsScreen::createListViewItemAccessoryTypeEditModeLayout(VerticalLayout* listViewItemPropertiesVerticalLayout)
{
	Label* accessoryTypeEditModeLabel = new Label();
	accessoryTypeEditModeLabel->setText("Accessory type edit mode");
	listViewItemPropertiesVerticalLayout->addChild(accessoryTypeEditModeLabel);

	HorizontalLayout* accessoryTypeEditModeLine1Layout = new HorizontalLayout();
	accessoryTypeEditModeLine1Layout->wrapContentVertically();
	mListViewItemAccessoryTypeEditModeNoneCheckbox = new CheckBox();
	mListViewItemAccessoryTypeEditModeNoneCheckbox->setState(true);
	Label* accessoryTypeEditModeNoneLabel = new Label();
	accessoryTypeEditModeNoneLabel->setText("None");
	accessoryTypeEditModeLine1Layout->addChild(mListViewItemAccessoryTypeEditModeNoneCheckbox);
	accessoryTypeEditModeLine1Layout->addChild(accessoryTypeEditModeNoneLabel);
	mListViewItemAccessoryTypeEditModeDisclosureCheckbox = new CheckBox();
	mListViewItemAccessoryTypeEditModeDisclosureCheckbox->setState(false);
	Label* accessoryTypeEditModeDisclosureLabel = new Label();
	accessoryTypeEditModeDisclosureLabel->setText("Disclosure");
	accessoryTypeEditModeLine1Layout->addChild(mListViewItemAccessoryTypeEditModeDisclosureCheckbox);
	accessoryTypeEditModeLine1Layout->addChild(accessoryTypeEditModeDisclosureLabel);
	listViewItemPropertiesVerticalLayout->addChild(accessoryTypeEditModeLine1Layout);

	HorizontalLayout* accessoryTypeEditModeLine2Layout = new HorizontalLayout();
	accessoryTypeEditModeLine2Layout->wrapContentVertically();
	mListViewItemAccessoryTypeEditModeDetailCheckbox = new CheckBox();
	mListViewItemAccessoryTypeEditModeDetailCheckbox->setState(false);
	Label* accessoryTypeEditModeDetailLabel = new Label();
	accessoryTypeEditModeDetailLabel->setText("Detail");
	accessoryTypeEditModeLine2Layout->addChild(mListViewItemAccessoryTypeEditModeDetailCheckbox);
	accessoryTypeEditModeLine2Layout->addChild(accessoryTypeEditModeDetailLabel);
	mListViewItemAccessoryTypeEditModeCheckmarkCheckbox = new CheckBox();
	mListViewItemAccessoryTypeEditModeCheckmarkCheckbox->setState(false);
	Label* accessoryTypeEditModeCheckmarkLabel = new Label();
	accessoryTypeEditModeCheckmarkLabel->setText("Checkmark");
	accessoryTypeEditModeLine2Layout->addChild(mListViewItemAccessoryTypeEditModeCheckmarkCheckbox);
	accessoryTypeEditModeLine2Layout->addChild(accessoryTypeEditModeCheckmarkLabel);
	listViewItemPropertiesVerticalLayout->addChild(accessoryTypeEditModeLine2Layout);
}

void SettingsScreen::createListViewItemEditStyleLayout(VerticalLayout* listViewItemPropertiesVerticalLayout)
{
	Label* listViewItemEditModeLabel = new Label();
	listViewItemEditModeLabel->setText("Item edit mode");
	listViewItemPropertiesVerticalLayout->addChild(listViewItemEditModeLabel);

	HorizontalLayout* listViewItemEditModeLine1Layout = new HorizontalLayout();
	listViewItemEditModeLine1Layout->wrapContentVertically();
	mListViewItemEditStyleNoneCheckbox = new CheckBox();
	mListViewItemEditStyleNoneCheckbox->setState(true);
	Label* listViewItemEditModeNoneLabel = new Label();
	listViewItemEditModeNoneLabel->setText("None");
	listViewItemEditModeLine1Layout->addChild(mListViewItemEditStyleNoneCheckbox);
	listViewItemEditModeLine1Layout->addChild(listViewItemEditModeNoneLabel);
	mListViewItemEditStyleDeleteCheckbox = new CheckBox();
	mListViewItemEditStyleDeleteCheckbox->setState(false);
	Label* mListViewItemEditModeDeleteLabel = new Label();
	mListViewItemEditModeDeleteLabel->setText("Delete");
	listViewItemEditModeLine1Layout->addChild(mListViewItemEditStyleDeleteCheckbox);
	listViewItemEditModeLine1Layout->addChild(mListViewItemEditModeDeleteLabel);
	listViewItemPropertiesVerticalLayout->addChild(listViewItemEditModeLine1Layout);

	HorizontalLayout* listViewItemEditModeLine2Layout = new HorizontalLayout();
	listViewItemEditModeLine2Layout->wrapContentVertically();
	mListViewItemEditStyleInsertCheckbox = new CheckBox();
	mListViewItemEditStyleInsertCheckbox->setState(false);
	Label* listViewItemEditModeInsertLabel = new Label();
	listViewItemEditModeInsertLabel->setText("Insert");
	listViewItemEditModeLine2Layout->addChild(mListViewItemEditStyleInsertCheckbox);
	listViewItemEditModeLine2Layout->addChild(listViewItemEditModeInsertLabel);
	listViewItemPropertiesVerticalLayout->addChild(listViewItemEditModeLine2Layout);
}

/**
 * Will create and add to the parent layout an info label and two buttons.
 * The buttons will be used to test getter and setter for
 * MAW_LIST_VIEW_ITEM_SELECTION_STYLE property.
 * @param parent Will contain the widgets.
 */
void SettingsScreen::createListViewItemSelectionStyleLayout(VerticalLayout* parent)
{
	Label* label = new Label();
	label->wrapContentHorizontally();
	label->wrapContentVertically();
	label->setText("Selection style");
	parent->addChild(label);

	mSetSelectionStyle = new Button();
	mSetSelectionStyle->fillSpaceHorizontally();
	mSetSelectionStyle->wrapContentVertically();
	mSetSelectionStyle->setText("Set selection style");
	parent->addChild(mSetSelectionStyle);

	mGetSelectionStyle = new Button();
	mGetSelectionStyle->fillSpaceHorizontally();
	mGetSelectionStyle->wrapContentVertically();
	mGetSelectionStyle->setText("Get used selection style");
	parent->addChild(mGetSelectionStyle);
}

/**
 * This method is called when a list item is clicked.
 * @param parentSection The parent section of the list view item clicked.
 * @param item The list view item clicked.
 */
void SettingsScreen::listScreenItemClicked(ListViewSection* &parentSection, ListViewItem* &listViewItem)
{
	mCurrentListViewItem = listViewItem;
	mCurrentListViewSection = parentSection;
	updateSelectedItemUI();
}


/**
 * Will show an options box so user can select the list view item selection
 * style.
 * Works on iOS platform only.
 */
void SettingsScreen::showSelectionStyleOptionBox()
{
	OptionsBox* optionBox = OptionsBox::getInstance();
	optionBox->resetDialog();
	optionBox->setTitle(L"Set selection style");
	optionBox->addButton(L"Blue");
	optionBox->addButton(L"Gray");
	optionBox->addButton(L"None");
	optionBox->setCancelButtonTitle(L"Cancel");
	optionBox->setCancelButtonVisible(true);
	optionBox->show();
}

/**
 * Show currently used selection style for an list view item.
 * Works on iOS platform.
 */
void SettingsScreen::showSelectionStyleMessageBox()
{
	MAUtil::String textToShow;
	if (mListView->countChildWidgets() == 0)
	{
		textToShow = "The list view section does not have sections.";
	}
	else
	{
		ListViewSection* section = (ListViewSection*) mListView->getChild(0);
		if (section->countChildWidgets() == 0)
		{
			textToShow = "The list view section does not have items.";
		}
		else
		{
			ListViewItem* item = (ListViewItem*) section->getChild(0);
			ListViewItemSelectionStyle selectionStyle =
				item->getSelectionStyle();
			textToShow = "The selection style is: " +
				getSelectionStyleString(selectionStyle);
		}
	}
	maAlert("ListViewItem selection style", textToShow.c_str(), "OK", NULL, NULL);
}

/**
 * Change all the list view's items selection style to a given value.
 * @param style Style to set.
 */
void SettingsScreen::changeSelectionStyle(ListViewItemSelectionStyle style)
{
	int sectionIndex = 0;
	int countSections = mListView->countChildWidgets();

	while (sectionIndex < countSections)
	{
		ListViewSection* section = (ListViewSection*) mListView->getChild(sectionIndex);
		int itemIndex = 0;
		int countItems = section->countChildWidgets();

		while (itemIndex < countItems)
		{
			ListViewItem* item = (ListViewItem*) section->getChild(itemIndex);
			item->setSelectionStyle(style);

			itemIndex++;
		}

		sectionIndex++;
	}
}

/**
 * This method is called when a button from options dialog was
 * clicked.
 * This method is not called if the destructive or cancel button were
 * clicked.
 * @param buttonIndex The index of the button that was clicked.
 * @param buttonTitle The title of the button that was clicked.
 */
void SettingsScreen::optionsBoxButtonClicked(const int buttonIndex,
	const MAUtil::WString& buttonTitle)
{
	ListViewItemSelectionStyle style;
	switch (buttonIndex)
	{
	case 0:
		style = LIST_VIEW_ITEM_SELECTION_STYLE_BLUE;
		break;
	case 1:
		style = LIST_VIEW_ITEM_SELECTION_STYLE_GRAY;
		break;
	case 2:
	default:
		style = LIST_VIEW_ITEM_SELECTION_STYLE_NONE;
		break;
	}
	this->changeSelectionStyle(style);
}

/**
 * This method is called when the return button was pressed.
 * On iphone platform the virtual keyboard is not hidden after
 * receiving this event.
 * @param editBox The edit box object that generated the event.
 */
void SettingsScreen::editBoxReturn(EditBox* editBox)
{
	if (isIOS())
	{
		editBox->hideKeyboard();
	}
}

/**
 * This method is called when the state of the check box was changed
 * by the user.
 * @param checkBox The check box object that generated the event.
 * @param state True if the check box is checked, false otherwise.
 */
void SettingsScreen::checkBoxStateChanged(
    CheckBox* checkBox,
    bool state)
{
	if (mListViewAllowSelectionCheckbox == checkBox )
	{
		if (state == true)
		{
			mListView->allowSelection(true);
		}
		else
		{
			mListView->allowSelection(false);
		}
	}
	else if (mDisplayCheckBox == checkBox)
	{
		if (state == true)
		{
			mEditCheckBox->setState(false);
			mListView->setMode(LIST_VIEW_MODE_DISPLAY);
		}
		else
		{
			mEditCheckBox->setState(true);
			mListView->setMode(LIST_VIEW_MODE_EDIT);
		}
	}
	else if (mEditCheckBox == checkBox)
	{
		if (state == true)
		{
			mDisplayCheckBox->setState(false);
			mListView->setMode(LIST_VIEW_MODE_EDIT);
		}
		else
		{
			mDisplayCheckBox->setState(true);
			mListView->setMode(LIST_VIEW_MODE_DISPLAY);
		}
	}
	else if (mListViewItemEditModeCheckbox == checkBox)
	{
		if (state == true && mCurrentListViewItem != NULL)
		{
			mCurrentListViewItem->setEditable(true);
		}
		else if (state == false && mCurrentListViewItem != NULL)
		{
			mCurrentListViewItem->setEditable(false);
		}
	}
	else if (mListViewItemSelectedCheckbox == checkBox)
	{
		if (state == true && mCurrentListViewItem != NULL)
		{
			mCurrentListViewItem->setSelected(true, true);
		}
		else if (state == false && mCurrentListViewItem != NULL)
		{
			mCurrentListViewItem->setSelected(true, false);
		}
	}
	else if (mListViewItemUnselectedCheckbox == checkBox)
	{
		if (state == true && mCurrentListViewItem != NULL)
		{
			mCurrentListViewItem->setSelected(false, true);
		}
		else if (state == false && mCurrentListViewItem != NULL)
		{
			mCurrentListViewItem->setSelected(false, false);
		}
	}
	else if (mListViewItemHighlightedCheckbox == checkBox)
	{
		if (state == true && mCurrentListViewItem != NULL)
		{
			mCurrentListViewItem->setHighlighted(true, true);
		}
		else if (state == false && mCurrentListViewItem != NULL)
		{
			mCurrentListViewItem->setHighlighted(true, false);
		}
	}
	else if (mListViewItemUnhighlightedCheckbox == checkBox)
	{
		if (state == true && mCurrentListViewItem != NULL)
		{
			mCurrentListViewItem->setHighlighted(false, true);
		}
		else if (state == false && mCurrentListViewItem != NULL)
		{
			mCurrentListViewItem->setHighlighted(false, false);
		}
	}
	else if (mListViewItemAccessoryTypeIntNoneCheckbox == checkBox)
	{
		if (state == true)
		{
			mListViewItemAccessoryTypeIntCheckmarkCheckbox->setState(false);
			mListViewItemAccessoryTypeIntDetailCheckbox->setState(false);
			mListViewItemAccessoryTypeIntDisclosureCheckbox->setState(false);
			setAccessoryTypeIntProperty(LIST_VIEW_ITEM_ACCESSORY_NONE);
		}
	}
	else if (mListViewItemAccessoryTypeIntDisclosureCheckbox == checkBox)
	{
		if (state == true)
		{
			mListViewItemAccessoryTypeIntCheckmarkCheckbox->setState(false);
			mListViewItemAccessoryTypeIntDetailCheckbox->setState(false);
			mListViewItemAccessoryTypeIntNoneCheckbox->setState(false);
			setAccessoryTypeIntProperty(LIST_VIEW_ITEM_ACCESSORY_DISCLOSURE);
		}
	}
	else if (mListViewItemAccessoryTypeIntDetailCheckbox == checkBox)
	{
		if (state == true)
		{
			mListViewItemAccessoryTypeIntCheckmarkCheckbox->setState(false);
			mListViewItemAccessoryTypeIntDisclosureCheckbox->setState(false);
			mListViewItemAccessoryTypeIntNoneCheckbox->setState(false);
			setAccessoryTypeIntProperty(LIST_VIEW_ITEM_ACCESSORY_DETAIL);
		}
	}
	else if (mListViewItemAccessoryTypeIntCheckmarkCheckbox == checkBox)
	{
		if (state == true)
		{
			mListViewItemAccessoryTypeIntDetailCheckbox->setState(false);
			mListViewItemAccessoryTypeIntDisclosureCheckbox->setState(false);
			mListViewItemAccessoryTypeIntNoneCheckbox->setState(false);
			setAccessoryTypeIntProperty(LIST_VIEW_ITEM_ACCESSORY_CHECKMARK);
		}
	}
	else if (mListViewItemAccessoryTypeEditModeNoneCheckbox == checkBox)
	{
		if (state == true)
		{
			mListViewItemAccessoryTypeEditModeDetailCheckbox->setState(false);
			mListViewItemAccessoryTypeEditModeDisclosureCheckbox->setState(false);
			mListViewItemAccessoryTypeEditModeCheckmarkCheckbox->setState(false);
			setAccessoryTypeEditModeProperty(LIST_VIEW_ITEM_ACCESSORY_NONE);
		}
	}
	else if (mListViewItemAccessoryTypeEditModeDisclosureCheckbox == checkBox)
	{
		if (state == true)
		{
			mListViewItemAccessoryTypeEditModeDetailCheckbox->setState(false);
			mListViewItemAccessoryTypeEditModeNoneCheckbox->setState(false);
			mListViewItemAccessoryTypeEditModeCheckmarkCheckbox->setState(false);
			setAccessoryTypeEditModeProperty(LIST_VIEW_ITEM_ACCESSORY_DISCLOSURE);
		}
	}
	else if (mListViewItemAccessoryTypeEditModeDetailCheckbox == checkBox)
	{
		if (state == true)
		{
			mListViewItemAccessoryTypeEditModeDisclosureCheckbox->setState(false);
			mListViewItemAccessoryTypeEditModeNoneCheckbox->setState(false);
			mListViewItemAccessoryTypeEditModeCheckmarkCheckbox->setState(false);
			setAccessoryTypeEditModeProperty(LIST_VIEW_ITEM_ACCESSORY_DETAIL);
		}
	}
	else if (mListViewItemAccessoryTypeEditModeCheckmarkCheckbox == checkBox)
	{
		if (state == true)
		{
			mListViewItemAccessoryTypeEditModeDetailCheckbox->setState(false);
			mListViewItemAccessoryTypeEditModeNoneCheckbox->setState(false);
			mListViewItemAccessoryTypeEditModeDisclosureCheckbox->setState(false);
			setAccessoryTypeEditModeProperty(LIST_VIEW_ITEM_ACCESSORY_CHECKMARK);
		}
	}
	else if (mListViewItemEditStyleNoneCheckbox == checkBox)
	{
		if (state == true)
		{
			mListViewItemEditStyleDeleteCheckbox->setState(false);
			mListViewItemEditStyleInsertCheckbox->setState(false);
			setEditModeProperty(LIST_VIEW_ITEM_EDIT_STYLE_NONE);
		}
	}
	else if (mListViewItemEditStyleDeleteCheckbox == checkBox)
	{
		if (state == true)
		{
			mListViewItemEditStyleNoneCheckbox->setState(false);
			mListViewItemEditStyleInsertCheckbox->setState(false);
			setEditModeProperty(LIST_VIEW_ITEM_EDIT_STYLE_DELETE);
		}
	}
	else if (mListViewItemEditStyleInsertCheckbox == checkBox)
	{
		if (state == true)
		{
			mListViewItemEditStyleNoneCheckbox->setState(false);
			mListViewItemEditStyleDeleteCheckbox->setState(false);
			setEditModeProperty(LIST_VIEW_ITEM_EDIT_STYLE_INSERT);
		}
	}
}

/**
 * Handles the accessory type int list view item property
 */
void SettingsScreen::setAccessoryTypeIntProperty(ListViewItemAccessory accessoryType)
{
	if (mCurrentListViewItem == NULL)
		return;
	mCurrentListViewItem->setAccessoryType(accessoryType);
}

/**
 * Handles the accessory type edit mode list view item property
 */
void SettingsScreen::setAccessoryTypeEditModeProperty(ListViewItemAccessory accessoryType)
{
	if (mCurrentListViewItem == NULL)
		return;
	mCurrentListViewItem->setAccessoryTypeEditingState(accessoryType);
}

/**
 * Sets the edit mode of the current selected list view item.
 */
void SettingsScreen::setEditModeProperty(ListViewItemEditStyle editMode)
{
	if (mCurrentListViewItem == NULL)
		return;
	mCurrentListViewItem->setEditStyle(editMode);
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * Platform: iOS, Android and Windows Phone 7.
 * @param button The button object that generated the event.
 */
void SettingsScreen::buttonClicked(Widget* button)
{
	if (mSetListViewSectionTitleButton == button)
	{
		if (mCurrentListViewSection != NULL && mCurrentListViewSectionTitleEditBox->getText() != "")
		{
			mCurrentListViewSection->setTitle(mCurrentListViewSectionTitleEditBox->getText());
		}
	}
	else if (mSetListViewSectionHeaderButton == button)
	{
		if (mCurrentListViewSection != NULL && mCurrentListViewSectionHeaderEditBox->getText() != "")
		{
			mCurrentListViewSection->setHeaderText(mCurrentListViewSectionHeaderEditBox->getText());
		}
	}
	else if (mSetListViewSectionFooterButton == button)
	{
		if (mCurrentListViewSection != NULL && mCurrentListViewSectionFooterEditBox->getText() != "")
		{
			mCurrentListViewSection->setFooterText(mCurrentListViewSectionFooterEditBox->getText());
		}
	}
	else if (mSetSectionHeaderBackgroundColor == button)
	{
		if (mCurrentListViewSection != NULL && mHeaderBackgroundColorEditBox->getText() != "")
		{
			mCurrentListViewSection->setProperty(MAW_LIST_VIEW_SECTION_HEADER_BACKGROUND,mHeaderBackgroundColorEditBox->getText().c_str());
		}
	}
	else if (mSetSectionHeaderFontSize == button)
	{
		if (mCurrentListViewSection != NULL && mHeaderFontSizeEditBox->getText() != "")
		{
			int fontSize = MAUtil::stringToInteger(mHeaderFontSizeEditBox->getText(),10);
			mCurrentListViewSection->setPropertyInt(MAW_LIST_VIEW_SECTION_HEADER_FONT_SIZE,fontSize);
		}
	}
	else if (mSetSectionHeaderFontColor == button)
	{
		if (mCurrentListViewSection != NULL && mHeaderFontColorEditBox->getText() != "")
		{
			mCurrentListViewSection->setProperty(MAW_LIST_VIEW_SECTION_HEADER_FONT_COLOR,mHeaderFontColorEditBox->getText().c_str());
		}
	}
	else if (mSetSectionFooterBackgroundColor == button)
	{
		if (mCurrentListViewSection != NULL && mFooterBackgroundColorEditBox->getText() != "")
		{
			mCurrentListViewSection->setProperty(MAW_LIST_VIEW_SECTION_FOOTER_BACKGROUND,mFooterBackgroundColorEditBox->getText().c_str());
		}
	}
	else if (mSetSectionFooterFontSize == button)
	{
		if (mCurrentListViewSection != NULL && mFooterFontSizeEditBox->getText() != "")
		{
			int fontSize = MAUtil::stringToInteger(mFooterFontSizeEditBox->getText(),10);
			mCurrentListViewSection->setPropertyInt(MAW_LIST_VIEW_SECTION_FOOTER_FONT_SIZE,fontSize);
		}
	}
	else if (mSetSectionFooterFontColor == button)
	{
		if (mCurrentListViewSection != NULL && mFooterFontColorEditBox->getText() != "")
		{
			mCurrentListViewSection->setProperty(MAW_LIST_VIEW_SECTION_FOOTER_FONT_COLOR,mFooterFontColorEditBox->getText().c_str());
		}
	}
	else if (mSetListViewItemTextButton == button)
	{
		if (mCurrentListViewItem != NULL && mSetListViewItemTextEditBox->getText() != "")
		{
			mCurrentListViewItem->setText(mSetListViewItemTextEditBox->getText());
		}
	}
	else if (mSetListViewItemFontColorButton == button)
	{
		if (mCurrentListViewItem != NULL && mSetListViewItemFontColorEditBox->getText() != "")
		{
			int fontColor = MAUtil::stringToInteger(mSetListViewItemFontColorEditBox->getText(),16);
			mCurrentListViewItem->setFontColor(fontColor);
		}
	}
	else if (mSetListViewItemFontSizeButton == button)
	{
		if (mCurrentListViewItem != NULL && mSetListViewItemFontSizeEditBox->getText() != "")
		{
			float fontSize = MAUtil::stringToDouble(mSetListViewItemFontSizeEditBox->getText());
			mCurrentListViewItem->setFontSize(fontSize);
		}
	}
	else if (mSetListViewItemDeleteTitleButton == button)
	{
		if (mCurrentListViewItem != NULL && mSetListViewItemDeleteTitleEditBox->getText() != "")
		{
			mCurrentListViewItem->setDeleteButtonTitle(mSetListViewItemDeleteTitleEditBox->getText());
		}
	}
	else if (mSetSelectionStyle == button)
	{
		this->showSelectionStyleOptionBox();
	}
	else if (mGetSelectionStyle == button)
	{
		this->showSelectionStyleMessageBox();
	}
}

/**
 * Updates the widgets related to the selected list view item.
 */
void SettingsScreen::updateSelectedItemUI()
{
	mCurrentListViewItemLabel->setText("Current item: " + mCurrentListViewItem->getPropertyString(MAW_LIST_VIEW_ITEM_TEXT));
	mListViewSectionLabel->setText("Current section: " + mCurrentListViewSection->getTitle());
}

/**
 * Sets the list view used on the ListView screen.
 */
void SettingsScreen::setListView(ListView* listView)
{
	mListView = listView;
}
