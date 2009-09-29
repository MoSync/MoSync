#include "wl_def.h"

/*
=============================================================================

						 GLOBAL VARIABLES

=============================================================================
*/

boolean		madenoise;		// true when shooting or screaming

exit_t		playstate;

int		DebugOk;

objtype 	objlist[MAXACTORS],*new,*obj,*player,*lastobj,
			*objfreelist,*killerobj;

unsigned	farmapylookup[MAPSIZE];

boolean		singlestep,godmode,noclip;

byte		tilemap[MAPSIZE][MAPSIZE];	// wall values only
byte		spotvis[MAPSIZE][MAPSIZE];
int		actorat[MAPSIZE][MAPSIZE];

int tics;

//
// control info
//
boolean		mouseenabled,joystickenabled,joypadenabled;
int			joystickport;
int			dirscan[4] = {sc_UpArrow,sc_RightArrow,sc_DownArrow,sc_LeftArrow};
int			buttonscan[NUMBUTTONS] =
			{sc_Control,sc_Alt,sc_RShift,sc_Space,sc_1,sc_2,sc_3,sc_4};
int			buttonmouse[4]={bt_attack,bt_strafe,bt_use,bt_nobutton};
int			buttonjoy[4]={bt_attack,bt_strafe,bt_use,bt_run};

boolean		buttonheld[NUMBUTTONS];

boolean		demorecord,demoplayback;
byte		*demoptr, *lastdemoptr;
memptr		demobuffer;

//
// curent user input
//
int		controlx,controly;	/* range from -100 to 100 per tic */
boolean		buttonstate[NUMBUTTONS];

static void RemoveObj(objtype *gone);

/*
=============================================================================

						 LOCAL VARIABLES

=============================================================================
*/

/* LIST OF SONGS FOR EACH VERSION */
static const int songs[]=
{
#ifndef SPEAR
/* Episode One */
 GETTHEM_MUS,
 SEARCHN_MUS,
 POW_MUS,
 SUSPENSE_MUS,
 GETTHEM_MUS,
 SEARCHN_MUS,
 POW_MUS,
 SUSPENSE_MUS,
 WARMARCH_MUS,	/* Boss level */
 CORNER_MUS,	/* Secret level */

/* Episode Two */
 NAZI_OMI_MUS,
 PREGNANT_MUS,
 GOINGAFT_MUS,
 HEADACHE_MUS,
 NAZI_OMI_MUS,
 PREGNANT_MUS,
 HEADACHE_MUS,
 GOINGAFT_MUS,
 WARMARCH_MUS,	/* Boss level */
 DUNGEON_MUS,	/* Secret level */

/*  Episode Three */
 INTROCW3_MUS,
 NAZI_RAP_MUS,
 TWELFTH_MUS,
 ZEROHOUR_MUS,
 INTROCW3_MUS,
 NAZI_RAP_MUS,
 TWELFTH_MUS,
 ZEROHOUR_MUS,
 ULTIMATE_MUS,	/* Boss level */
 PACMAN_MUS,	/* Secret level */

/* Episode Four */
 GETTHEM_MUS,
 SEARCHN_MUS,
 POW_MUS,
 SUSPENSE_MUS,
 GETTHEM_MUS,
 SEARCHN_MUS,
 POW_MUS,
 SUSPENSE_MUS,
 WARMARCH_MUS,	/* Boss level */
 CORNER_MUS,	/* Secret level */

/* Episode Five */
 NAZI_OMI_MUS,
 PREGNANT_MUS,
 GOINGAFT_MUS,
 HEADACHE_MUS,
 NAZI_OMI_MUS,
 PREGNANT_MUS,
 HEADACHE_MUS,
 GOINGAFT_MUS,
 WARMARCH_MUS,	/* Boss level */
 DUNGEON_MUS,	/* Secret level */

 /* Episode Six */
 INTROCW3_MUS,
 NAZI_RAP_MUS,
 TWELFTH_MUS,
 ZEROHOUR_MUS,
 INTROCW3_MUS,
 NAZI_RAP_MUS,
 TWELFTH_MUS,
 ZEROHOUR_MUS,
 ULTIMATE_MUS,	/* Boss level */
 FUNKYOU_MUS	/* Secret level */
#else

 //////////////////////////////////////////////////////////////
 //
 // SPEAR OF DESTINY TRACKS
 //
 //////////////////////////////////////////////////////////////
 XTIPTOE_MUS,
 XFUNKIE_MUS,
 XDEATH_MUS,
 XGETYOU_MUS,
 ULTIMATE_MUS,	/* Trans Gr”sse */

 DUNGEON_MUS,
 GOINGAFT_MUS,
 POW_MUS,
 TWELFTH_MUS,
 ULTIMATE_MUS,	/* Barnacle Wilhelm BOSS */

 NAZI_OMI_MUS,
 GETTHEM_MUS,
 SUSPENSE_MUS,
 SEARCHN_MUS,
 ZEROHOUR_MUS,
 ULTIMATE_MUS,	/* Super Mutant BOSS */

 XPUTIT_MUS,
 ULTIMATE_MUS,	/* Death Knight BOSS */

 XJAZNAZI_MUS,	/* Secret level */
 XFUNKIE_MUS,	/* Secret level */

 XEVIL_MUS	/* Angel of Death BOSS */

#endif
};


/*
=============================================================================

						  USER CONTROL

=============================================================================
*/


#define BASEMOVE		35
#define RUNMOVE			70

/*
===================
=
= PollKeyboardButtons
=
===================
*/

void PollKeyboardButtons()
{
	int i;

	for (i=0;i<NUMBUTTONS;i++)
		if (IN_KeyDown(buttonscan[i]))
			buttonstate[i] = true;
}


/*
===================
=
= PollMouseButtons
=
===================
*/

void PollMouseButtons()
{
	int	buttons;

	buttons = IN_MouseButtons();

	if (buttons&1)
		buttonstate[buttonmouse[0]] = true;
	if (buttons&2)
		buttonstate[buttonmouse[1]] = true;
	if (buttons&4)
		buttonstate[buttonmouse[2]] = true;
}


/*
===================
=
= PollJoystickButtons
=
===================
*/

void PollJoystickButtons()
{
	int	buttons;

	buttons = IN_JoyButtons();

	if (joystickport && !joypadenabled)
	{
		if (buttons&4)
			buttonstate[buttonjoy[0]] = true;
		if (buttons&8)
			buttonstate[buttonjoy[1]] = true;
	}
	else
	{
		if (buttons&1)
			buttonstate[buttonjoy[0]] = true;
		if (buttons&2)
			buttonstate[buttonjoy[1]] = true;
		if (joypadenabled)
		{
			if (buttons&4)
				buttonstate[buttonjoy[2]] = true;
			if (buttons&8)
				buttonstate[buttonjoy[3]] = true;
		}
	}
}

/*
===================
=
= PollKeyboardMove
=
===================
*/

void PollKeyboardMove()
{
	if (buttonstate[bt_run])
	{
		if (IN_KeyDown(dirscan[di_north]))
			controly -= RUNMOVE*tics;
		if (IN_KeyDown(dirscan[di_south]))
			controly += RUNMOVE*tics;
		if (IN_KeyDown(dirscan[di_west]))
			controlx -= RUNMOVE*tics;
		if (IN_KeyDown(dirscan[di_east]))
			controlx += RUNMOVE*tics;
	}
	else
	{
		if (IN_KeyDown(dirscan[di_north]))
			controly -= BASEMOVE*tics;
		if (IN_KeyDown(dirscan[di_south]))
			controly += BASEMOVE*tics;
		if (IN_KeyDown(dirscan[di_west]))
			controlx -= BASEMOVE*tics;
		if (IN_KeyDown(dirscan[di_east]))
			controlx += BASEMOVE*tics;
	}
}


/*
===================
=
= PollMouseMove
=
===================
*/

void PollMouseMove()
{
	int mousexmove = 0, mouseymove = 0;

	IN_GetMouseDelta(&mousexmove, &mouseymove);

	controlx += mousexmove*10/(13-mouseadjustment);
	controly += mouseymove*20/(13-mouseadjustment);
}

/*
===================
=
= PollJoystickMove
=
===================
*/

void PollJoystickMove()
{
	int joyx, joyy;

	INL_GetJoyDelta(joystickport,&joyx,&joyy);

	if (buttonstate[bt_run])
	{
		if (joyx > 64)
			controlx += RUNMOVE*tics;
		else if (joyx < -64)
			controlx -= RUNMOVE*tics;
		if (joyy > 64)
			controly += RUNMOVE*tics;
		else if (joyy < -64)
			controly -= RUNMOVE*tics;
	}
	else
	{
		if (joyx > 64)
			controlx += BASEMOVE*tics;
		else if (joyx < -64)
			controlx -= BASEMOVE*tics;
		if (joyy > 64)
			controly += BASEMOVE*tics;
		else if (joyy < -64)
			controly -= BASEMOVE*tics;
	}
}

void UpdateInput()
{
//
// get button states
//
	PollKeyboardButtons();

	if (mouseenabled)
		PollMouseButtons();

	if (joystickenabled)
		PollJoystickButtons();

//
// get movements
//
	PollKeyboardMove();

	if (mouseenabled)
		PollMouseMove();

	if (joystickenabled)
		PollJoystickMove();
}

/*
===================
=
= PollControls
=
= Gets user or demo input, call once each frame
=
= controlx		set between -100 and 100 per tic
= controly
= buttonheld[]	the state of the buttons LAST frame
= buttonstate[]	the state of the buttons THIS frame
=
===================
*/

void PollControls()
{
	int max, min, i;
	byte buttonbits;

	controlx = 0;
	controly = 0;
	memcpy(buttonheld, buttonstate, sizeof(buttonstate));
	memset(buttonstate, 0, sizeof(buttonstate));

	if (demoplayback) {
	//
	// read commands from demo buffer
	//
		buttonbits = *demoptr++;
		for (i = 0; i < NUMBUTTONS; i++)
		{
			buttonstate[i] = buttonbits&1;
			buttonbits >>= 1;
		}

		controlx = (signed char)*demoptr++;
		controly = (signed char)*demoptr++;

		if (demoptr == lastdemoptr)
			playstate = ex_completed;		// demo is done

		controlx *= (int)tics;
		controly *= (int)tics;

		return;
	}

	/* Update keys */
	IN_CheckAck();

	UpdateInput();

//
// bound movement to a maximum
//
	max = 100*tics;
	min = -max;
	if (controlx > max)
		controlx = max;
	else if (controlx < min)
		controlx = min;

	if (controly > max)
		controly = max;
	else if (controly < min)
		controly = min;

	if (demorecord)
	{
	//
	// save info out to demo buffer
	//
		controlx /= (int)tics;
		controly /= (int)tics;

		buttonbits = 0;

		for (i=NUMBUTTONS-1;i>=0;i--)
		{
			buttonbits <<= 1;
			if (buttonstate[i])
				buttonbits |= 1;
		}

		*demoptr++ = buttonbits;
		*demoptr++ = controlx;
		*demoptr++ = controly;

		if (demoptr >= lastdemoptr)
			Quit("Demo buffer overflowed!");

		controlx *= (int)tics;
		controly *= (int)tics;
	}
}



//==========================================================================



///////////////////////////////////////////////////////////////////////////
//
//	CenterWindow() - Generates a window of a given width & height in the
//		middle of the screen
//
///////////////////////////////////////////////////////////////////////////

#define MAXX	320
#define MAXY	160

void CenterWindow(word w,word h)
{
	US_DrawWindow(((MAXX / 8) - w) / 2,((MAXY / 8) - h) / 2,w,h);
}

//===========================================================================


/*
=====================
=
= CheckKeys
=
=====================
*/

void CheckKeys()
{
	byte	scan;

	if (screenfaded || demoplayback)	// don't do anything with a faded screen
		return;

	scan = LastScan;

	//
	// SECRET CHEAT CODE: TAB-G-F10
	//
	if (IN_KeyDown(sc_Tab) && IN_KeyDown(sc_G) && IN_KeyDown(sc_F10)) {
		WindowH = 160;
		if (godmode)
		{
			Message ("God mode OFF");
			SD_PlaySound (NOBONUSSND);
		}
		else
		{
			Message ("God mode ON");
			SD_PlaySound (ENDBONUS2SND);
		}

		IN_Ack();
		godmode ^= 1;
		DrawPlayBorderSides();
		IN_ClearKeysDown();
		return;
	}

	//
	// SECRET CHEAT CODE: 'MLI'
	//
	if (IN_KeyDown(sc_M) &&	IN_KeyDown(sc_L) && IN_KeyDown(sc_I)) {
		gamestate.health = 100;
		gamestate.ammo = 99;
		gamestate.keys = 3;
		gamestate.score = 0;
		gamestate.TimeCount += 42000L;
		GiveWeapon(wp_chaingun);

		DrawWeapon();
		DrawHealth();
		DrawKeys();
		DrawAmmo();
		DrawScore();

		ClearMemory();
		CA_CacheGrChunk(STARTFONT+1);
		ClearSplitVWB();

		Message(STR_CHEATER);

		CA_UnCacheGrChunk(STARTFONT+1);
		IN_ClearKeysDown();
		IN_Ack();

		DrawPlayBorder();
	}

	//
	// OPEN UP DEBUG KEYS
	//
	if (IN_KeyDown(sc_BackSpace) && IN_KeyDown(sc_LShift) &&
		IN_KeyDown(sc_Alt) && MS_CheckParm("debugmode")) {
	 ClearMemory();
	 CA_CacheGrChunk(STARTFONT+1);
	 ClearSplitVWB();

	 Message(STR_DEBUG);

	 CA_UnCacheGrChunk(STARTFONT+1);
	 IN_ClearKeysDown();
	 IN_Ack();

	 DrawPlayBorderSides();
	 DebugOk=1;
	}

	//
	// TRYING THE KEEN CHEAT CODE!
	//
	if (IN_KeyDown(sc_B) && IN_KeyDown(sc_A) && IN_KeyDown(sc_T)) {
		ClearMemory();
		CA_CacheGrChunk(STARTFONT+1);
		ClearSplitVWB();

		Message(STR_KEEN);

		CA_UnCacheGrChunk(STARTFONT+1);
		IN_ClearKeysDown();
		IN_Ack();

		DrawPlayBorder();
	}

	if (Paused) {
		VWB_DrawPic(128, 64, PAUSEDPIC);
		VW_UpdateScreen();

		SD_MusicOff();
		IN_Ack();
		IN_ClearKeysDown();
		SD_MusicOn();

		IN_GetMouseDelta(NULL, NULL); // Clear accumulated mouse movement

		Paused = false;
		return;
	}


//
// F1-F7/ESC to enter control panel
//
	if (
		scan == sc_F10 ||
		scan == sc_F9 ||
		scan == sc_F7 ||
		scan == sc_F8)			// pop up quit dialog
	{
		ClearMemory();
		ClearSplitVWB();
		US_ControlPanel(scan);

		DrawPlayBorderSides();
		VW_UpdateScreen();

		if (scan == sc_F9)
			StartMusic();

		SETFONTCOLOR(0,15);
		IN_ClearKeysDown();

		lasttimecount = get_TimeCount();

		return;
	}

	if ( (scan >= sc_F1 && scan <= sc_F9) || scan == sc_Escape)
	{
		StopMusic();
		ClearMemory();
		VW_FadeOut();

		US_ControlPanel(scan);

		SETFONTCOLOR(0,15);
		IN_ClearKeysDown();
		DrawPlayScreen();
		VW_UpdateScreen();

		if (!startgame && !loadedgame)
		{
			VW_FadeIn();
			StartMusic();
		}
		if (loadedgame)
			playstate = ex_abort;
		lasttimecount = get_TimeCount();

		IN_GetMouseDelta(NULL, NULL); // Clear accumulated mouse movement

		return;
	}

//
// TAB-? debug keys
//
	if (IN_KeyDown(sc_Tab) && DebugOk) {
		CA_CacheGrChunk (STARTFONT);
		fontnumber=0;
		SETFONTCOLOR(0,15);
		DebugKeys();
		lasttimecount = get_TimeCount();

		IN_GetMouseDelta(NULL, NULL); // Clear accumulated mouse movement

		return;
	}

}


//===========================================================================

/*
#############################################################################

				  The objlist data structure

#############################################################################

objlist containt structures for every actor currently playing.  The structure
is accessed as a linked list starting at *player, ending when ob->next ==
NULL.  GetNewObj inserts a new object at the end of the list, meaning that
if an actor spawn another actor, the new one WILL get to think and react the
same frame.  RemoveObj unlinks the given object and returns it to the free
list, but does not damage the objects ->next pointer, so if the current object
removes itself, a linked list following loop can still safely get to the
next element.

<backwardly linked free list>

#############################################################################
*/


/*
=========================
=
= InitActorList
=
= Call to clear out the actor object lists returning them all to the free
= list.  Allocates a special spot for the player.
=
=========================
*/

void InitActorList()
{
	int	i;

//
// init the actor lists
//
	for (i = 0; i < MAXACTORS; i++)
	{
		objlist[i].id = i;
		objlist[i].prev = &objlist[i+1];
		objlist[i].next = NULL;
	}

	objlist[MAXACTORS-1].prev = NULL;

	objfreelist = &objlist[0];
	lastobj = NULL;

/* give the player the first free spots */
	GetNewActor();
	player = new;

}

//===========================================================================

/*
=========================
=
= GetNewActor
=
= Sets the global variable new to point to a free spot in objlist.
= The free spot is inserted at the end of the liked list
=
=========================
*/

void GetNewActor()
{
	int id;

	if (!objfreelist)
		Quit("GetNewActor: No free spots in objlist!");

	new = objfreelist;
	id = new->id;
	objfreelist = new->prev;

	memset(new, 0, sizeof(*new));
	new->id = id;

	if (lastobj)
		lastobj->next = new;
	new->prev = lastobj;	// new->next is already NULL from memset

	new->active = ac_no;
	lastobj = new;
}

//===========================================================================

/*
=========================
=
= RemoveObj
=
= Add the given object back into the free list, and unlink it from its
= neighbors
=
=========================
*/

static void RemoveObj(objtype *gone)
{
	if (gone == player)
		Quit("RemoveObj: Tried to remove the player!");

	gone->state = s_none;

//
// fix the next object's back link
//
	if (gone == lastobj)
		lastobj = gone->prev;
	else
		gone->next->prev = gone->prev;

//
// fix the previous object's forward link
//
	gone->prev->next = gone->next;

//
// add it back in to the free list
//
	gone->prev = objfreelist;
	objfreelist = gone;

}

/*
=============================================================================

						MUSIC STUFF

=============================================================================
*/


/*
=================
=
= StopMusic
=
=================
*/

void StopMusic()
{
	FreeMusic();
}

/*
=================
=
= StartMusic
=
=================
*/

void StartMusic()
{
	StartCPMusic(songs[gamestate.mapon+gamestate.episode*10]);
}


/*
=============================================================================

					PALETTE SHIFTING STUFF

=============================================================================
*/

#define NUMREDSHIFTS	6
#define REDSTEPS		8

#define NUMWHITESHIFTS	3
#define WHITESTEPS		20
#define WHITETICS		6


byte	redshifts[NUMREDSHIFTS][768];
byte	whiteshifts[NUMREDSHIFTS][768];

int	damagecount,bonuscount;
boolean	palshifted;

/*
=====================
=
= InitRedShifts
=
=====================
*/

void InitRedShifts()
{
	byte *workptr;
	const byte *baseptr;

	int	i,j,delta;


//
// fade through intermediate frames
//
	for (i=1;i<=NUMREDSHIFTS;i++)
	{
		workptr = (byte *)&redshifts[i-1][0];
		baseptr = gamepal;

		for (j=0;j<=255;j++)
		{
			delta = 64-*baseptr;
			*workptr++ = *baseptr++ + delta * i / REDSTEPS;
			delta = -*baseptr;
			*workptr++ = *baseptr++ + delta * i / REDSTEPS;
			delta = -*baseptr;
			*workptr++ = *baseptr++ + delta * i / REDSTEPS;
		}
	}

	for (i=1;i<=NUMWHITESHIFTS;i++)
	{
		workptr = (byte *)&whiteshifts[i-1][0];
		baseptr = gamepal;

		for (j=0;j<=255;j++)
		{
			delta = 64-*baseptr;
			*workptr++ = *baseptr++ + delta * i / WHITESTEPS;
			delta = 62-*baseptr;
			*workptr++ = *baseptr++ + delta * i / WHITESTEPS;
			delta = 0-*baseptr;
			*workptr++ = *baseptr++ + delta * i / WHITESTEPS;
		}
	}
}


/*
=====================
=
= ClearPaletteShifts
=
=====================
*/

void ClearPaletteShifts()
{
	bonuscount = damagecount = 0;
}


/*
=====================
=
= StartBonusFlash
=
=====================
*/

void StartBonusFlash()
{
	bonuscount = NUMWHITESHIFTS*WHITETICS;		// white shift palette
}


/*
=====================
=
= StartDamageFlash
=
=====================
*/

void StartDamageFlash(int damage)
{
	damagecount += damage;
}


/*
=====================
=
= UpdatePaletteShifts
=
=====================
*/

void UpdatePaletteShifts()
{
	int red, white;

	if (bonuscount)
	{
		white = bonuscount/WHITETICS +1;
		if (white>NUMWHITESHIFTS)
			white = NUMWHITESHIFTS;
		bonuscount -= tics;
		if (bonuscount < 0)
			bonuscount = 0;
	}
	else
		white = 0;


	if (damagecount)
	{
		red = damagecount/10 +1;
		if (red>NUMREDSHIFTS)
			red = NUMREDSHIFTS;

		damagecount -= tics;
		if (damagecount < 0)
			damagecount = 0;
	}
	else
		red = 0;

	if (red)
	{
		VL_SetPalette(redshifts[red-1]);
		palshifted = true;
	}
	else if (white)
	{
		VL_SetPalette(whiteshifts[white-1]);
		palshifted = true;
	}
	else if (palshifted)
	{
		VL_SetPalette(gamepal);		// back to normal
		palshifted = false;
	}
}


/*
=====================
=
= FinishPaletteShifts
=
= Resets palette to normal if needed
=
=====================
*/

void FinishPaletteShifts()
{
	if (palshifted)
	{
		palshifted = 0;
		VL_SetPalette(gamepal);
	}
}


/*
=============================================================================

						CORE PLAYLOOP

=============================================================================
*/


/*
=====================
=
= DoActor
=
=====================
*/

void DoActor(objtype *ob)
{
	void (*think)(objtype *);

	if (!ob->active && !areabyplayer[ob->areanumber])
		return;

	if (!(ob->flags & (FL_NONMARK|FL_NEVERMARK)))
		actorat[ob->tilex][ob->tiley] = 0;

//
// non transitional object
//

	if (!ob->ticcount)
	{
		think =	gamestates[ob->state].think;
		if (think)
		{
			think(ob);
			if (ob->state == s_none)
			{
				RemoveObj (ob);
				return;
			}
		}

		if (ob->flags&FL_NEVERMARK)
			return;

		if ((ob->flags&FL_NONMARK) && actorat[ob->tilex][ob->tiley])
			return;

		actorat[ob->tilex][ob->tiley] = ob->id | 0x8000;
		return;
	}

//
// transitional object
//
	ob->ticcount-=tics;
	while (ob->ticcount <= 0)
	{
		think = gamestates[ob->state].action;	// end of state action
		if (think)
		{
			think(ob);
			if (ob->state == s_none)
			{
				RemoveObj(ob);
				return;
			}
		}

		ob->state = gamestates[ob->state].next;

		if (ob->state == s_none)
		{
			RemoveObj(ob);
			return;
		}

		if (!gamestates[ob->state].tictime)
		{
			ob->ticcount = 0;
			goto think;
		}

		ob->ticcount += gamestates[ob->state].tictime;
	}

think:
	//
	// think
	//
	think =	gamestates[ob->state].think;
	if (think)
	{
		think(ob);
		if (ob->state == s_none)
		{
			RemoveObj(ob);
			return;
		}
	}

	if (ob->flags&FL_NEVERMARK)
		return;

	if ((ob->flags&FL_NONMARK) && actorat[ob->tilex][ob->tiley])
		return;

	actorat[ob->tilex][ob->tiley] = ob->id | 0x8000;
}

//==========================================================================


/*
===================
=
= PlayLoop
=
===================
*/

long funnyticount;

void PlayLoop()
{
	playstate = lasttimecount = 0;

	frameon = 0;
	anglefrac = 0;
	facecount = 0;
	funnyticount = 0;

	memset (buttonstate,0,sizeof(buttonstate));
	ClearPaletteShifts();

	IN_GetMouseDelta(NULL, NULL); // Clear accumulated mouse movement

	if (demoplayback)
		IN_StartAck();

	set_TimeCount(0);

	do
	{
		/* get timing info for last frame */
		CalcTics();

		/* handle input */
		PollControls();

//
// actor thinking
//
		madenoise = false;

		MoveDoors();
		MovePWalls();

		for (obj = player; obj; obj = obj->next)
			DoActor(obj);

		UpdatePaletteShifts();

		ThreeDRefresh();

		//
		// MAKE FUNNY FACE IF BJ DOESN'T MOVE FOR AWHILE
		//
		#ifdef SPEAR
		funnyticount += tics;
		if (funnyticount > 30l*70)
		{
			funnyticount = 0;
			StatusDrawPic (17,4,BJWAITING1PIC+(US_RndT()&1));
			facecount = 0;
		}
		#endif

		gamestate.TimeCount += tics;


 		UpdateSoundLoc(player->x, player->y, player->angle);

		if (screenfaded)
			VW_FadeIn();


		CheckKeys();

//
// debug aids
//
		if (singlestep)
		{
			VW_WaitVBL(14);
			lasttimecount = get_TimeCount();
		}

		if (demoplayback)
		{
			if (IN_CheckAck())
			{
				IN_ClearKeysDown();
				playstate = ex_abort;
			}
		}
	} while (!playstate && !startgame);

	if (playstate != ex_died)
		FinishPaletteShifts();
}
