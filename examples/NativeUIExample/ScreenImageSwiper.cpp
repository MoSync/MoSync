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

	VerticalLayout *mainLayout = new VerticalLayout();
	HorizontalLayout *imageLayout = new HorizontalLayout();
	VerticalLayout *infoLayout = new VerticalLayout();

//	Image** images = new Image*[RES_IMAGE_NUM];
//	for (int i=0; i<RES_IMAGE_NUM; i++)
//	{
//		images[i] = new Image();
//		images[i]->setResource(RES_IMAGE1 + i);
//		imageLayout->addChild(images[i]);
//	}

	//mainLayout->setSize(100, 100);
	mainLayout->setBackgroundColor(0x00FF00);
//	imageLayout->setBackgroundColor(0x00FF00);
//	infoLayout->setBackgroundColor(0x0000FF);
//
//	mainLayout->addChild(imageLayout);
//	mainLayout->addChild(infoLayout);

	screen->setMainWidget(mainLayout);

	return screen;
}
