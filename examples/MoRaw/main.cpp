/** \file main.cpp
*
* Port of RAW, another world interpreter
*
* Copyright (c) Mobile Sorcery AB 2005-2007
*
* \author Niklas Nummelin
*
*/
#include <ma.h>
//#include <mastdlib.h>
//#include <mavsprintf.h>
#include "MAHeaders.h"
//#include <event.h>

#include "engine.h"
#include "systemstub.h"
#include "util.h"
#include <MAFS/File.h>

#include <conprint.h>


extern "C" {
	/**
	* \brief The entry point.
	*/

	int MAMain()
	{
		const char *dataPath = ".";
		const char *savePath = ".";

		setCurrentFileSystem(FILESYSTEM, 0);

		gConsoleLogging = 0;

		g_debugMask = 0;
		//g_debugMask = DBG_INFO | DBG_LOGIC | DBG_BANK | DBG_VIDEO | DBG_SER | DBG_SND;
		//g_debugMask = DBG_INFO | DBG_LOGIC | DBG_VIDEO | DBG_SER | DBG_SND;
		//g_debugMask = DBG_SND;

		SystemStub *stub = SystemStub_MoSync_create();
		Engine *e = new Engine(stub, dataPath, savePath);
		e->run();
		delete e;
		delete stub;

		freeCurrentFileSystem();

		return 0;

		/// wait for key press or close event
		/*
		while(1) {

			maUpdateScreen();
			EventHandler::updateEvents();
			if(EventHandler::quit) break;
		}
		return 0;
		*/
	}
}
