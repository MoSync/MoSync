/*
Copyright (C) 2011 MoSync AB

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
 * @file ScreenImageSwiper.h
 * @author TODO: Add your name.
 *
 * TODO: Add comment.
 */

#include "MAHeaders.h"
#include "ScreenImageSwiper.h"

/**
 * TODO: Add comment.
 */
Screen* ScreenImageSwiper::create()
{
	Screen* screen = new Screen();
	screen->setTitle("Images");
	screen->setIcon(RES_TAB_ICON_IMAGE_SWIPER);

	//int SCREEN_WIDTH = screen->getPropertyInt(MAW_WIDGET_WIDTH);
	printf("SCREEN_WIDTH = %d", screen->getPropertyInt(MAW_WIDGET_WIDTH));

	VerticalLayout* mainLayout = new VerticalLayout();
	HorizontalLayout* imageLayout = new HorizontalLayout();

	RelativeLayout* leftImageLayout = new RelativeLayout();
	leftImageLayout->setSize(SCREEN_WIDTH / 6, MAW_CONSTANT_FILL_AVAILABLE_SPACE);
	//leftImageLayout->setBackgroundColor(0xFF0000);

	RelativeLayout* centerImageLayout = new RelativeLayout();
	centerImageLayout->setSize(2 * SCREEN_WIDTH / 3, MAW_CONSTANT_FILL_AVAILABLE_SPACE);
	//centerImageLayout->setBackgroundColor(0x00FF00);

	RelativeLayout* rightImageLayout = new RelativeLayout();
	rightImageLayout->setSize(SCREEN_WIDTH / 6, MAW_CONSTANT_FILL_AVAILABLE_SPACE);
	//rightImageLayout->setBackgroundColor(0xFF0000);

	int imageSize = SCREEN_WIDTH - (SCREEN_WIDTH / 3) - SCREEN_WIDTH / 6;

	Image** images = new Image*[DISPLAYED_IMAGES];
	for (int i=0; i<DISPLAYED_IMAGES; i++)
	{
		images[i] = new Image();
		images[i]->setResource(RES_IMAGE1 + i);
		images[i]->setSize(imageSize, MAW_CONSTANT_WRAP_CONTENT);
	}

	images[LEFT_IMAGE]->setPosition(SCREEN_WIDTH / 6 - imageSize - SCREEN_WIDTH / 12, 10);
	leftImageLayout->addChild(images[LEFT_IMAGE]);

	images[CENTER_IMAGE]->setPosition(SCREEN_WIDTH / 12, 10);
	centerImageLayout->addChild(images[CENTER_IMAGE]);

	images[RIGHT_IMAGE]->setPosition(SCREEN_WIDTH / 12, 10);
	rightImageLayout->addChild(images[RIGHT_IMAGE]);



	//imageLayout->setBackgroundColor(0x0000FF);

	imageLayout->addChild(leftImageLayout);
	imageLayout->addChild(centerImageLayout);
	imageLayout->addChild(rightImageLayout);

	mainLayout->addChild(imageLayout);

	screen->setMainWidget(mainLayout);

	return screen;
}
