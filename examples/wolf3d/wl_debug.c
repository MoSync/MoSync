#include "wl_def.h"

/*
==================
=
= CountObjects
=
==================
*/

void CountObjects()
{
	int i, total, count, active, inactive, doors;
	objtype	*obj;

	CenterWindow (16,7);
	active = inactive = count = doors = 0;

	US_Print ("Total statics :");
	total = laststatobj-&statobjlist[0];
	US_PrintUnsigned (total);

	US_Print ("\nIn use statics:");
	for (i=0;i<total;i++)
		if (statobjlist[i].shapenum != -1)
			count++;
		else
			doors++;	//debug
	US_PrintUnsigned (count);

	US_Print ("\nDoors         :");
	US_PrintUnsigned (doornum);

	for (obj=player->next;obj;obj=obj->next)
	{
		if (obj->active)
			active++;
		else
			inactive++;
	}

	US_Print ("\nTotal actors  :");
	US_PrintUnsigned (active+inactive);

	US_Print ("\nActive actors :");
	US_PrintUnsigned (active);

	VW_UpdateScreen();
	IN_Ack();
}

/*
================
=
= PicturePause
=
================
*/

void PicturePause()
{

	FinishPaletteShifts();

	IN_Ack();
	
	if (LastScan != sc_Enter)
	{
		return;
	}

	/* TODO: save picture to file */
	
	VL_SetPalette(gamepal);

	VW_WaitVBL(70);
	VW_WaitVBL(70);
	Quit(NULL);
}

/*
================
=
= ShapeTest
=
================
*/

void ShapeTest()
{
}

/*
================
=
= DebugKeys
=
================
*/

int DebugKeys()
{
	boolean esc;
	int level;

	if (IN_KeyDown(sc_C))		// C = count objects
	{
		CountObjects();
		return 1;
	}

	if (IN_KeyDown(sc_E))		// E = quit level
	{
		playstate = ex_completed;
//		gamestate.mapon++;
	}

	if (IN_KeyDown(sc_F))		// F = facing spot
	{
		CenterWindow (14,4);
		US_Print ("X:");
		US_PrintUnsigned (player->x);
		US_Print ("\nY:");
		US_PrintUnsigned (player->y);
		US_Print ("\nA:");
		US_PrintUnsigned (player->angle);
		VW_UpdateScreen();
		IN_Ack();
		return 1;
	}

	if (IN_KeyDown(sc_G))		// G = god mode
	{
		CenterWindow (12,2);
		if (godmode)
		  US_PrintCentered ("God mode OFF");
		else
		  US_PrintCentered ("God mode ON");
		VW_UpdateScreen();
		IN_Ack();
		godmode ^= 1;
		return 1;
	}
	if (IN_KeyDown(sc_H))		// H = hurt self
	{
		IN_ClearKeysDown ();
		TakeDamage (16,NULL);
	}
	else if (IN_KeyDown(sc_I))			// I = item cheat
	{
		CenterWindow (12,3);
		US_PrintCentered ("Free items!");
		VW_UpdateScreen();
		GivePoints(100000);
		HealSelf(99);
		if (gamestate.bestweapon<wp_chaingun)
			GiveWeapon (gamestate.bestweapon+1);
		gamestate.ammo += 50;
		if (gamestate.ammo > 99)
			gamestate.ammo = 99;
		DrawAmmo ();
		IN_Ack ();
		return 1;
	}
	else if (IN_KeyDown(sc_N))			// N = no clip
	{
		noclip^=1;
		CenterWindow (18,3);
		if (noclip)
			US_PrintCentered ("No clipping ON");
		else
			US_PrintCentered ("No clipping OFF");
		VW_UpdateScreen();
		IN_Ack ();
		return 1;
	}
	else if (IN_KeyDown(sc_P))			// P = pause with no screen disruptioon
	{
		PicturePause ();
		return 1;
	}
	else if (IN_KeyDown(sc_Q))			// Q = fast quit
		Quit(NULL);
	else if (IN_KeyDown(sc_S))			// S = slow motion
	{
		singlestep^=1;
		CenterWindow (18,3);
		if (singlestep)
			US_PrintCentered ("Slow motion ON");
		else
			US_PrintCentered ("Slow motion OFF");
		VW_UpdateScreen();
		IN_Ack ();
		return 1;
	}
	else if (IN_KeyDown(sc_T))			// T = shape test
	{
		ShapeTest();
		return 1;
	}
	else if (IN_KeyDown(sc_W))			// W = warp to level
	{
		CenterWindow(26,3);
		PrintY+=6;
#ifndef SPEAR
		US_Print("  Warp to which level(1-10):");
#elif defined(SPEARDEMO)
		US_Print("  Warp to which level(1-2):");
#else
		US_Print("  Warp to which level(1-21):");
#endif
		VW_UpdateScreen();
		esc = !US_LineInput (px,py,str,NULL,true,2,0);
		if (!esc)
		{
			level = atoi (str);
#ifndef SPEAR
			if (level>0 && level<11)
#elif defined(SPEARDEMO)
			if (level>0 && level<2)
#else
			if (level>0 && level<22)
#endif
			{
				gamestate.mapon = level-1;
				playstate = ex_warped;
			}
		}
		return 1;
	}

	DrawPlayBorder();
	
	return 0;
}
