#include "wl_def.h"

#ifndef SPEAR

/*
=============================================================================

TEXT FORMATTING COMMANDS
------------------------
^C<hex digit>  			Change text color
^E[enter]				End of layout (all pages)
^G<y>,<x>,<pic>[enter]	Draw a graphic and push margins
^P[enter]				start new page, must be the first chars in a layout
^L<x>,<y>[ENTER]		Locate to a specific spot, x in pixels, y in lines

=============================================================================
*/

/*
=============================================================================

						 LOCAL CONSTANTS

=============================================================================
*/

#define BACKCOLOR		0x11

#define WORDLIMIT		80
#define FONTHEIGHT		10
#define	TOPMARGIN		16
#define BOTTOMMARGIN		32
#define LEFTMARGIN		16
#define RIGHTMARGIN		16
#define PICMARGIN		8
#define TEXTROWS		((200-TOPMARGIN-BOTTOMMARGIN)/FONTHEIGHT)
#define	SPACEWIDTH		7
#define SCREENPIXWIDTH		320
#define SCREENMID		(SCREENPIXWIDTH/2)

/*
=============================================================================

						 LOCAL VARIABLES

=============================================================================
*/

static int pagenum, numpages;

static unsigned leftmargin[TEXTROWS], rightmargin[TEXTROWS];
static const char *text;
static unsigned rowon;

static int picx, picy, picnum, picdelay;
static boolean layoutdone;

/* ======================================================================== */

/*
=====================
=
= RipToEOL
=
=====================
*/

void RipToEOL()
{
	while (*text++ != '\n')		// scan to end of line
	;
}


/*
=====================
=
= ParseNumber
=
=====================
*/

int ParseNumber()
{
	char ch;
	char num[80], *numptr;

//
// scan until a number is found
//
	ch = *text;
	while (ch < '0' || ch >'9')
		ch = *++text;

//
// copy the number out
//
	numptr = num;
	do
	{
		*numptr++ = ch;
		ch = *++text;
	} while (ch >= '0' && ch <= '9');
	*numptr = 0;

	return atoi(num);
}



/*
=====================
=
= ParsePicCommand
=
= Call with text pointing just after a ^P
= Upon exit text points to the start of next line
=
=====================
*/

void ParsePicCommand()
{
	picy = ParseNumber();
	picx = ParseNumber();
	picnum = ParseNumber();
	RipToEOL();
}


void ParseTimedCommand()
{
	picy = ParseNumber();
	picx = ParseNumber();
	picnum = ParseNumber();
	picdelay = ParseNumber();
	RipToEOL();
}


/*
=====================
=
= TimedPicCommand
=
= Call with text pointing just after a ^P
= Upon exit text points to the start of next line
=
=====================
*/

void TimedPicCommand()
{
	ParseTimedCommand();

//
// update the screen, and wait for time delay
//
	VW_UpdateScreen();

//
// wait for time
//
	set_TimeCount(0);
	while (get_TimeCount() < picdelay) ;

//
// draw pic
//
	VWB_DrawPic(picx&~7, picy, picnum);
}


/*
=====================
=
= HandleCommand
=
=====================
*/

void HandleCommand()
{
	int	i,margin,top,bottom;
	int	picwidth,picheight,picmid;

	switch (toupper(*++text))
	{
	case 'B':
		picy=ParseNumber();
		picx=ParseNumber();
		picwidth=ParseNumber();
		picheight=ParseNumber();
		VW_Bar(picx,picy,picwidth,picheight,BACKCOLOR);
		RipToEOL();
		break;
	case ';':		// comment
		RipToEOL();
		break;
	case 'P':		// ^P is start of next page, ^E is end of file
	case 'E':
		layoutdone = true;
		text--;    	// back up to the '^'
		break;

	case 'C':		// ^c<hex digit> changes text color
		i = toupper(*++text);
		if (i>='0' && i<='9')
			fontcolor = i-'0';
		else if (i>='A' && i<='F')
			fontcolor = i-'A'+10;

		fontcolor *= 16;
		i = toupper(*++text);
		if (i>='0' && i<='9')
			fontcolor += i-'0';
		else if (i>='A' && i<='F')
			fontcolor += i-'A'+10;
		text++;
		break;

	case '>':
		px = 160;
		text++;
		break;

	case 'L':
		py=ParseNumber();
		rowon = (py-TOPMARGIN)/FONTHEIGHT;
		py = TOPMARGIN+rowon*FONTHEIGHT;
		px=ParseNumber();
		while (*text++ != '\n')		// scan to end of line
		;
		break;

	case 'T':		// ^Tyyy,xxx,ppp,ttt waits ttt tics, then draws pic
		TimedPicCommand();
		break;

	case 'G':		// ^Gyyy,xxx,ppp draws graphic
		ParsePicCommand ();
		VWB_DrawPic (picx&~7,picy,picnum);
		picwidth = pictable[picnum-STARTPICS].width;
		picheight = pictable[picnum-STARTPICS].height;
		//
		// adjust margins
		//
		picmid = picx + picwidth/2;
		if (picmid > SCREENMID)
			margin = picx-PICMARGIN;			// new right margin
		else
			margin = picx+picwidth+PICMARGIN;	// new left margin

		top = (picy-TOPMARGIN)/FONTHEIGHT;
		if (top<0)
			top = 0;
		bottom = (picy+picheight-TOPMARGIN)/FONTHEIGHT;
		if (bottom>=TEXTROWS)
			bottom = TEXTROWS-1;

		for (i=top;i<=bottom;i++)
			if (picmid > SCREENMID)
				rightmargin[i] = margin;
			else
				leftmargin[i] = margin;

		//
		// adjust this line if needed
		//
		if (px < leftmargin[rowon])
			px = leftmargin[rowon];
		break;
	}
}


/*
=====================
=
= NewLine
=
=====================
*/

void NewLine()
{
	char	ch;

	if (++rowon == TEXTROWS)
	{
	//
	// overflowed the page, so skip until next page break
	//
		layoutdone = true;
		do
		{
			if (*text == '^')
			{
				ch = toupper(*(text+1));
				if (ch == 'E' || ch == 'P')
				{
					layoutdone = true;
					return;
				}
			}
			text++;

		} while (1);

	}
	px = leftmargin[rowon];
	py+= FONTHEIGHT;
}



/*
=====================
=
= HandleCtrls
=
=====================
*/

void HandleCtrls()
{
	char	ch;

	ch = *text++;			// get the character and advance

	if (ch == '\n')
	{
		NewLine ();
		return;
	}

}


/*
=====================
=
= HandleWord
=
=====================
*/

void HandleWord()
{
	char words[WORDLIMIT];
	int wordindex;
	word wwidth, wheight, newpos;

	//
	// copy the next word into words[]
	//
	words[0] = *text++;
	wordindex = 1;
	while (*text>32)
	{
		words[wordindex] = *text++;
		if (++wordindex == WORDLIMIT)
			Quit ("PageLayout: Word limit exceeded");
	}
	words[wordindex] = 0;		// stick a null at end for C

	/* see if it fits on this line */
	VW_MeasurePropString(words, &wwidth, &wheight);

	while (px+wwidth > rightmargin[rowon])
	{
		NewLine ();
		if (layoutdone)
			return;		// overflowed page
	}

	//
	// print it
	//
	newpos = px+wwidth;
	VW_DrawPropString(words);
	px = newpos;

	//
	// suck up any extra spaces
	//
	while (*text == ' ')
	{
		px += SPACEWIDTH;
		text++;
	}
}

/*
=====================
=
= PageLayout
=
= Clears the screen, draws the pics on the page, and word wraps the text.
= Returns a pointer to the terminating command
=
=====================
*/

void PageLayout(boolean shownumber)
{
	int		i,oldfontcolor;
	char	ch;

	oldfontcolor = fontcolor;

	fontcolor = 0;

//
// clear the screen
//
	VW_Bar(0,0,320,200,BACKCOLOR);
	VWB_DrawPic(0,0,H_TOPWINDOWPIC);
	VWB_DrawPic(0,8,H_LEFTWINDOWPIC);
	VWB_DrawPic(312,8,H_RIGHTWINDOWPIC);
	VWB_DrawPic(8,176,H_BOTTOMINFOPIC);


	for (i=0;i<TEXTROWS;i++)
	{
		leftmargin[i] = LEFTMARGIN;
		rightmargin[i] = SCREENPIXWIDTH-RIGHTMARGIN;
	}

	px = LEFTMARGIN;
	py = TOPMARGIN;
	rowon = 0;
	layoutdone = false;

//
// make sure we are starting layout text (^P first command)
//
	while (*text <= 32)
		text++;

	if (*text != '^' || toupper(*++text) != 'P')
		Quit ("PageLayout: Text not headed with ^P");

	while (*text++ != '\n')
	;


//
// process text stream
//
	do
	{
		ch = *text;

		if (ch == '^')
			HandleCommand ();
		else
		if (ch == 9)
		{
		 px = (px+8)&0xf8;
		 text++;
		}
		else if (ch <= 32)
			HandleCtrls ();
		else
			HandleWord ();

	} while (!layoutdone);

	pagenum++;

	if (shownumber)
	{
		strcpy (str,"pg ");
		itoa (pagenum,str2,10);
		strcat (str,str2);
		strcat (str," of ");
		py = 183;
		px = 213;
		itoa (numpages,str2,10);
		strcat (str,str2);
		fontcolor = 0x4f; 			   //12^BACKCOLOR;

		VW_DrawPropString (str);
	}

	fontcolor = oldfontcolor;
}

//===========================================================================

/*
=====================
=
= BackPage
=
= Scans for a previous ^P
=
=====================
*/

void BackPage()
{
	pagenum--;
	do
	{
		text--;
		if (*text == '^' && toupper(*(text+1)) == 'P')
			return;
	} while (1);
}


//===========================================================================


/*
=====================
=
= CacheLayoutGraphics
=
= Scans an entire layout file (until a ^E) marking all graphics used, and
= counting pages, then caches the graphics in
=
=====================
*/
void CacheLayoutGraphics()
{
	const char	*bombpoint, *textstart;
	char	ch;

	textstart = text;
	bombpoint = text+30000;
	numpages = pagenum = 0;

	do
	{
		if (*text == '^')
		{
			ch = toupper(*++text);
			if (ch == 'P')		// start of a page
				numpages++;
			if (ch == 'E')		// end of file, so load graphics and return
			{
				CA_CacheGrChunk(H_TOPWINDOWPIC);
				CA_CacheGrChunk(H_LEFTWINDOWPIC);
				CA_CacheGrChunk(H_RIGHTWINDOWPIC);
				CA_CacheGrChunk(H_BOTTOMINFOPIC);
				
				text = textstart;
				return;
			}
			if (ch == 'G')		// draw graphic command, so mark graphics
			{
				ParsePicCommand ();
				CA_CacheGrChunk(picnum);
			}
			if (ch == 'T')		// timed draw graphic command, so mark graphics
			{
				ParseTimedCommand();
				CA_CacheGrChunk(picnum);
			}
		}
		else
			text++;

	} while (text<bombpoint);

	Quit ("CacheLayoutGraphics: No ^E to terminate file!");
}

/*
=====================
=
= ShowArticle
=
=====================
*/

void ShowArticle(const char *article)
{
	unsigned oldfontnumber;
	boolean newpage, firstpage;


	text = article;
	oldfontnumber = fontnumber;
	fontnumber = 0;
	CA_CacheGrChunk(STARTFONT);
	VW_Bar (0,0,320,200,BACKCOLOR);
	CacheLayoutGraphics();

	newpage = true;
	firstpage = true;

	do
	{
		if (newpage)
		{
			newpage = false;
			PageLayout(true);
			VW_UpdateScreen();
			if (firstpage)
			{
				VL_FadeIn(0,255,gamepal,10);
				firstpage = false;
			}
		}

		LastScan = 0;
		while (!LastScan) IN_CheckAck(); /* update events */

		switch (LastScan)
		{
		case sc_UpArrow:
		case sc_PgUp:
		case sc_LeftArrow:
			if (pagenum>1)
			{
				BackPage ();
				BackPage ();
				newpage = true;
			}
			break;

		case sc_Enter:
		case sc_DownArrow:
		case sc_PgDn:
		case sc_RightArrow:		// the text already points at next page
			if (pagenum<numpages)
			{
				newpage = true;
			}
			break;
		}

		if (IN_KeyDown(sc_Tab) && IN_KeyDown(sc_P) && MS_CheckParm("debugmode"))
			PicturePause();

	} while (LastScan != sc_Escape);

	IN_ClearKeysDown ();
	fontnumber = oldfontnumber;
}


/* ======================================================================== */

#define endextern	T_ENDART1
#define helpextern	T_HELPART

/*
=================
=
= HelpScreens
=
=================
*/
void HelpScreens()
{
	int artnum;
	const char *text;

	artnum = helpextern;
	CA_CacheGrChunk(artnum);
	text = (const char *)grsegs[artnum];

	ShowArticle(text);

	CA_UnCacheGrChunk(artnum);
	
	VW_FadeOut();

	FreeMusic();
}

//
// END ARTICLES
//
void EndText()
{
	int artnum;
	const char *text;

	ClearMemory();

	artnum = endextern+gamestate.episode;
	CA_CacheGrChunk(artnum);
	text = (const char *)grsegs[artnum];

	ShowArticle(text);

	CA_UnCacheGrChunk(artnum);
	
	VW_FadeOut();
	SETFONTCOLOR(0,15);
	IN_ClearKeysDown();

	IN_GetMouseDelta(NULL, NULL); // Clear accumulated mouse movement
	
	FreeMusic();
}

#endif
