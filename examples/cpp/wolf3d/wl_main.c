#include "wl_def.h"

/*
=============================================================================

						   WOLFENSTEIN 3-D

					  An Id Software production

						   by John Carmack

=============================================================================
*/

#define FOCALLENGTH     0x5800		/* in global coordinates */

char str[80], str2[20];

int viewwidth, viewheight;
int viewwidthwin, viewheightwin; /* for borders */
int xoffset, yoffset;
int vwidth, vheight; /* size of screen */
int viewsize;

int centerx;
int shootdelta;			/* pixels away from centerx a target can be */

boolean startgame,loadedgame;
int mouseadjustment;

long frameon;
long lasttimecount;
fixed viewsin, viewcos;
//fixed viewx, viewy;		/* the focal point */
int pixelangle[MAXVIEWWIDTH];
long finetangent[FINEANGLES/4];
int horizwall[MAXWALLTILES], vertwall[MAXWALLTILES];

char configname[13] = "config.";

fixed sintable[ANGLES+ANGLES/4+1], *costable = sintable+(ANGLES/4);

int _argc;
char **_argv;

/*
========================
=
= FixedByFrac (FixedMul)
=
= multiply two 16/16 bit, 2's complement fixed point numbers
=
========================
*/
#ifdef __SYMBIAN32__
#ifdef __WINS__
fixed FixedByFrac(fixed a, fixed b)
{
	signed long long ra = a;
	signed long long rb = b;
	signed long long r;

	r = ra * rb;
	r >>= TILESHIFT;
	return (fixed)r;
}
#else
fixed FixedByFrac(fixed a, fixed b)
{
    int ret;

    // multiply the values -> r6:r7
    asm volatile ( "SMULL r6, r7, %0, %1" : :
                   "r"(a), "r"(b) : "r6", "r7" );
    // shift away the lowest 16 bits of result precision
    asm volatile ( "MOV r6, r6, LSR #16" : : : "r6" );
    // or the halfwords together
    asm volatile ( "ORR r6, r6, r7, LSL #16" : : : "r6" );
    // store in memory for return
    asm volatile ( "STR r6, %0" : "=m"(ret) );

    return ret;
}
#endif
#else
#ifdef NOASM
#if defined(__INTEL_COMPILER)
typedef _int64 int64_t;
#endif
fixed FixedByFrac(fixed a, fixed b)
{
	int64_t ra = a;
	int64_t rb = b;
	int64_t r;

	r = ra * rb;
	r >>= TILESHIFT;
	return (fixed)r;
}
#endif
#endif

/*
=====================
=
= CalcTics
=
=====================
*/

void CalcTics()
{
	int newtime;
	int ticcount;

	if (demoplayback || demorecord)
		ticcount = DEMOTICS - 1; /* [70/4] 17.5 Hz */
	else
		ticcount = 0 + 1; /* 35 Hz */

	do {
		newtime = get_TimeCount();
		tics = newtime - lasttimecount;
	} while (tics <= ticcount);

	lasttimecount = newtime;

	if (demoplayback || demorecord)
		tics = DEMOTICS;
	else if (tics > MAXTICS)
		tics = MAXTICS;
}

/* ======================================================================== */

static void DiskFlopAnim(int x, int y)
{
	static char which = 0;

	if (!x && !y)
		return;

	VWB_DrawPic(x, y, C_DISKLOADING1PIC+which);
	VW_UpdateScreen();

	which ^= 1;
}

static int32_t CalcFileChecksum(int fd, int len)
{
	int8_t buf[4096];

	int32_t cs;
	int i, j;
	int8_t c1;

	cs = 0;
	c1 = ReadInt8(fd);

	len--;
	while (len > 0) {
		i = 4096;
		if (len < i) {
			i = len;
		}

		ReadBytes(fd,(unsigned char*) buf, i);

		for (j = 0; j < i; j++) {
			cs += c1 ^ buf[j];
			c1 = buf[j];
		}

		len -= 4096;
	}

	return cs;
}

int WriteConfig()
{
	int i;
	int fd;
	int32_t cs;

	fd = OpenWrite(configname);

	if (fd != -1) {
		WriteBytes(fd, (byte *)GAMEHDR, 8);	/* WOLF3D, 0, 0 */
		WriteBytes(fd, (byte *)CFGTYPE, 4);	/* CFG, 0 */
	/**/	WriteInt32(fd, 0xFFFFFFFF);		/* Version (integer) */
		WriteBytes(fd, (byte *)GAMETYPE, 4);	/* XXX, 0 */
		WriteInt32(fd, time(NULL));		/* Time */
		WriteInt32(fd, 0x00000000);		/* Padding */
		WriteInt32(fd, 0x00000000);		/* Checksum (placeholder) */

		for (i = 0; i < 7; i++) { /* MaxScores = 7 */
			WriteBytes(fd, (byte *)Scores[i].name, 58);
			WriteInt32(fd, Scores[i].score);
			WriteInt32(fd, Scores[i].completed);
			WriteInt32(fd, Scores[i].episode);
		}

		WriteInt32(fd, viewsize);
		WriteInt8(fd,SoundMode);
		WriteInt8(fd,DigiMode);
		WriteInt8(fd,MusicMode);
		for(i=0;i<4;i++)
		{
			WriteInt16(fd,buttonjoy[i]);
			WriteInt16(fd,buttonmouse[i]);
			WriteInt16(fd,dirscan[i]);
			WriteInt16(fd,buttonscan[i*2]);
			WriteInt16(fd,buttonscan[i*2+1]);
		}
		CloseWrite(fd);

		fd = OpenRead(configname);
		ReadSeek(fd, 32, SEEK_SET);
		cs = CalcFileChecksum(fd, ReadLength(fd) - 32);
		CloseRead(fd);

		fd = OpenWriteAppend(configname);
		WriteSeek(fd, 28, SEEK_SET);
		WriteInt32(fd, cs);

		CloseWrite(fd);
	}

	return 0;
}

static void SetDefaults()
{
	viewsize = 15;

	mouseenabled = false;

	joystickenabled = false;
	joypadenabled = false;
	joystickport = 0;

	mouseadjustment = 5;

	SD_SetMusicMode(smm_AdLib);
	SD_SetSoundMode(sdm_AdLib);
	SD_SetDigiDevice(sds_SoundBlaster);
}

int ReadConfig()
{
	int fd, configokay;
	char buf[8];
	int32_t v;
	int i;
	SDMode SndMode = sdm_AdLib;
	SDSMode DigMode = sds_SoundBlaster;
	SMMode MusMode = smm_AdLib;
	configokay = 0;

	fd = OpenRead(configname);

	if (fd != -1) {
		ReadBytes(fd, (byte *)buf, 8);
		if (strncmp(buf, GAMEHDR, 8))
			goto configend;

		ReadBytes(fd, (byte *)buf, 4);
		if (strncmp(buf, CFGTYPE, 4))
			goto configend;

		v = ReadInt32(fd);
	/**/	if (v != 0xFFFFFFFF)
			goto configend;

		ReadBytes(fd, (byte *)buf, 4);
		if (strncmp(buf, GAMETYPE, 4))
			goto configend;

		ReadInt32(fd);	/* skip over time */
		ReadInt32(fd);	/* skip over padding */

		v = ReadInt32(fd);	/* get checksum */
		if (v != CalcFileChecksum(fd, ReadLength(fd) - 32))
			goto configend;

		ReadSeek(fd, 32, SEEK_SET);

		for (i = 0; i < 7; i++) { /* MaxScores = 7 */
			ReadBytes(fd, (byte *)Scores[i].name, 58);
			Scores[i].score = ReadInt32(fd);
			Scores[i].completed = ReadInt32(fd);
			Scores[i].episode = ReadInt32(fd);
		}

		viewsize = ReadInt32(fd);
		SndMode = ReadInt8(fd);
		DigMode = ReadInt8(fd);
		MusMode = ReadInt8(fd);
		for(i=0;i<4;i++)
		{
			buttonjoy[i] = ReadInt16(fd);
			buttonmouse[i] = ReadInt16(fd);
			dirscan[i] = ReadInt16(fd);
			buttonscan[i*2] = ReadInt16(fd);
			buttonscan[i*2+1] = ReadInt16(fd);
		}
#ifdef UPLOAD
		MainMenu[readthis].active = 1;
		MainItems.curpos = 0;
#endif

		configokay = 1;
	}

configend:

	if (fd != -1)
		CloseRead(fd);

	if (!configokay) {
//		printf("Config: Setting defaults..\n");
		SetDefaults();
	}
	if(Controls[0] == ctrl_Mouse)
		mouseenabled = true;
	else
		mouseenabled = false;

	joystickenabled = false;
	joypadenabled = false;
	joystickport = 0;

	mouseadjustment = 5;

	SD_SetMusicMode(MusMode);
	SD_SetSoundMode(SndMode);
	SD_SetDigiDevice(DigMode);

	return 0;
}

int SaveTheGame(const char *fn, const char *tag, int dx, int dy)
{
	objtype *ob;
	int fd, i, x, y;
	int32_t cs;

	fd = OpenWrite(fn);

	if (fd != -1) {
		WriteBytes(fd, (byte *)GAMEHDR, 8);
		WriteBytes(fd, (byte *)SAVTYPE, 4);
		WriteInt32(fd, 0xFFFFFFFF); /* write version */
		WriteBytes(fd, (byte *)GAMETYPE, 4);

		WriteInt32(fd, time(NULL));
		WriteInt32(fd, 0x00000000);

		WriteInt32(fd, 0x00000000); /* write checksum (placeholder) */

		WriteBytes(fd, (byte *)tag, 32); /* write savegame name */

		DiskFlopAnim(dx, dy);

		WriteInt32(fd, gamestate.difficulty);
		WriteInt32(fd, gamestate.mapon);
		WriteInt32(fd, gamestate.oldscore);
		WriteInt32(fd, gamestate.score);
		WriteInt32(fd, gamestate.nextextra);
		WriteInt32(fd, gamestate.lives);
		WriteInt32(fd, gamestate.health);
		WriteInt32(fd, gamestate.ammo);
		WriteInt32(fd, gamestate.keys);
		WriteInt32(fd, gamestate.bestweapon);
		WriteInt32(fd, gamestate.weapon);
		WriteInt32(fd, gamestate.chosenweapon);
		WriteInt32(fd, gamestate.faceframe);
		WriteInt32(fd, gamestate.attackframe);
		WriteInt32(fd, gamestate.attackcount);
		WriteInt32(fd, gamestate.weaponframe);
		WriteInt32(fd, gamestate.episode);
		WriteInt32(fd, gamestate.secretcount);
		WriteInt32(fd, gamestate.treasurecount);
		WriteInt32(fd, gamestate.killcount);
		WriteInt32(fd, gamestate.secrettotal);
		WriteInt32(fd, gamestate.treasuretotal);
		WriteInt32(fd, gamestate.killtotal);
		WriteInt32(fd, gamestate.TimeCount);
		WriteInt32(fd, gamestate.killx);
		WriteInt32(fd, gamestate.killy);
		WriteInt8(fd, gamestate.victoryflag);

		DiskFlopAnim(dx, dy);

#ifdef SPEAR
		for (i = 0; i < 20; i++) {
#else
		for (i = 0; i < 8; i++) {
#endif
			WriteInt32(fd, LevelRatios[i].kill);
			WriteInt32(fd, LevelRatios[i].secret);
			WriteInt32(fd, LevelRatios[i].treasure);
			WriteInt32(fd, LevelRatios[i].time);
		}

		DiskFlopAnim(dx, dy);

		WriteBytes(fd, (byte *)tilemap, 64*64); /* MAPSIZE * MAPSIZE */

		DiskFlopAnim(dx, dy);

		for (x = 0; x < 64; x++)
			for (y = 0; y < 64; y++)
				WriteInt32(fd, actorat[x][y]);

		DiskFlopAnim(dx, dy);

		WriteBytes(fd, (byte *)areaconnect, 37*37); /* NUMAREAS * NUMAREAS */

		DiskFlopAnim(dx, dy);

		for (i = 0; i < 37; i++)
			WriteInt8(fd, areabyplayer[i]);

		for (ob = player; ob; ob = ob->next) {
			DiskFlopAnim(dx, dy);

			WriteInt32(fd, ob->id);
			WriteInt32(fd, ob->active);
			WriteInt32(fd, ob->ticcount);
			WriteInt32(fd, ob->obclass);
			WriteInt32(fd, ob->state);
			WriteInt8(fd,  ob->flags);
			WriteInt32(fd, ob->distance);
			WriteInt32(fd, ob->dir);
			WriteInt32(fd, ob->x);
			WriteInt32(fd, ob->y);
			WriteInt32(fd, ob->tilex);
			WriteInt32(fd, ob->tiley);
			WriteInt8(fd,  ob->areanumber);
			WriteInt32(fd, ob->viewx);
			WriteInt32(fd, ob->viewheight);
			WriteInt32(fd, ob->transx);
			WriteInt32(fd, ob->transy);
			WriteInt32(fd, ob->angle);
			WriteInt32(fd, ob->hitpoints);
			WriteInt32(fd, ob->speed);
			WriteInt32(fd, ob->temp1);
			WriteInt32(fd, ob->temp2);
			WriteInt32(fd, ob->temp3);
		}

		WriteInt32(fd, 0xFFFFFFFF); /* end of actor list */

		DiskFlopAnim(dx, dy);

		WriteInt32(fd, laststatobj - statobjlist); /* ptr offset */

		for (i = 0; i < 400; i++) { /* MAXSTATS */
			WriteInt8(fd,  statobjlist[i].tilex);
			WriteInt8(fd,  statobjlist[i].tiley);
			WriteInt32(fd, statobjlist[i].shapenum);
			WriteInt8(fd,  statobjlist[i].flags);
			WriteInt8(fd,  statobjlist[i].itemnumber);
		}

		DiskFlopAnim(dx, dy);

		for (i = 0; i < 64; i++) { /* MAXDOORS */
			WriteInt32(fd, doorposition[i]);
		}

		DiskFlopAnim(dx, dy);

		for (i = 0; i < 64; i++) { /* MAXDOORS */
			WriteInt8(fd,  doorobjlist[i].tilex);
			WriteInt8(fd,  doorobjlist[i].tiley);
			WriteInt8(fd,  doorobjlist[i].vertical);
			WriteInt8(fd,  doorobjlist[i].lock);
			WriteInt8(fd,  doorobjlist[i].action);
			WriteInt32(fd, doorobjlist[i].ticcount);
		}

		DiskFlopAnim(dx, dy);

		WriteInt32(fd, pwallstate);
		WriteInt32(fd, pwallx);
		WriteInt32(fd, pwally);
		WriteInt32(fd, pwalldir);
		WriteInt32(fd, pwallpos);

		DiskFlopAnim(dx, dy);

		CloseWrite(fd);

		fd = OpenRead(fn);
		ReadSeek(fd, 64, SEEK_SET);
		cs = CalcFileChecksum(fd, ReadLength(fd) - 64);
		CloseRead(fd);

		fd = OpenWriteAppend(fn);
		WriteSeek(fd, 28, SEEK_SET);
		WriteInt32(fd, cs);

		CloseWrite(fd);
	} else {
		Message(STR_NOSPACE1"\n"
			STR_NOSPACE2);

		IN_ClearKeysDown();
		IN_Ack();

		return -1;
	}

	return 0;
}

int ReadSaveTag(const char *fn, const char *tag)
{
	char buf[8];
	int fd;
	int32_t v;

	fd = OpenRead(fn);
	if (fd == -1)
		goto rstfail;

	ReadBytes(fd, (byte *)buf, 8);
	if (strncmp(buf, GAMEHDR, 8))
		goto rstfail;

	ReadBytes(fd, (byte *)buf, 4);
	if (strncmp(buf, SAVTYPE, 4))
		goto rstfail;

	v = ReadInt32(fd);
	if (v != 0xFFFFFFFF && v != 0x00000000) /* -1 and 0 are the same */
		goto rstfail;

	ReadBytes(fd, (byte *)buf, 4);
	if (strncmp(buf, GAMETYPE, 4))
		goto rstfail;

	ReadInt32(fd);
	ReadInt32(fd);

	v = ReadInt32(fd); /* get checksum */

	ReadSeek(fd, 64, SEEK_SET);
	if (v != CalcFileChecksum(fd, ReadLength(fd) - 64))
		goto rstfail;

	ReadSeek(fd, 32, SEEK_SET);
	ReadBytes(fd, (byte *)tag, 32);

	CloseRead(fd);

	return 0;
rstfail:
	if (fd != -1)
		CloseRead(fd);

	return -1;
}

int LoadTheGame(const char *fn, int dx, int dy)
{
	char buf[8];
	int fd, i, x, y, id;
	int32_t v;

	fd = OpenRead(fn);

	if (fd == -1)
		goto loadfail;

	ReadBytes(fd, (byte *)buf, 8);
	if (strncmp(buf, GAMEHDR, 8))
		goto loadfail;

	ReadBytes(fd, (byte *)buf, 4);
	if (strncmp(buf, SAVTYPE, 4))
		goto loadfail;

	v = ReadInt32(fd);
	if (v != 0xFFFFFFFF && v != 0x00000000) /* -1 and 0 are the same */
		goto loadfail;

	ReadBytes(fd, (byte *)buf, 4);
	if (strncmp(buf, GAMETYPE, 4))
		goto loadfail;

	ReadInt32(fd);
	ReadInt32(fd);

	v = ReadInt32(fd); /* get checksum */

	ReadSeek(fd, 64, SEEK_SET);
	if (v != CalcFileChecksum(fd, ReadLength(fd) - 64))
		goto loadfail;

	ReadSeek(fd, 64, SEEK_SET);

	DiskFlopAnim(dx, dy);

	gamestate.difficulty	= ReadInt32(fd);
	gamestate.mapon		= ReadInt32(fd);
	gamestate.oldscore	= ReadInt32(fd);
	gamestate.score		= ReadInt32(fd);
	gamestate.nextextra	= ReadInt32(fd);
	gamestate.lives		= ReadInt32(fd);
	gamestate.health	= ReadInt32(fd);
	gamestate.ammo		= ReadInt32(fd);
	gamestate.keys		= ReadInt32(fd);
	gamestate.bestweapon	= ReadInt32(fd);
	gamestate.weapon	= ReadInt32(fd);
	gamestate.chosenweapon	= ReadInt32(fd);
	gamestate.faceframe	= ReadInt32(fd);
	gamestate.attackframe	= ReadInt32(fd);
	gamestate.attackcount	= ReadInt32(fd);
	gamestate.weaponframe	= ReadInt32(fd);
	gamestate.episode	= ReadInt32(fd);
	gamestate.secretcount	= ReadInt32(fd);
	gamestate.treasurecount	= ReadInt32(fd);
	gamestate.killcount	= ReadInt32(fd);
	gamestate.secrettotal	= ReadInt32(fd);
	gamestate.treasuretotal = ReadInt32(fd);
	gamestate.killtotal	= ReadInt32(fd);
	gamestate.TimeCount	= ReadInt32(fd);
	gamestate.killx		= ReadInt32(fd);
	gamestate.killy		= ReadInt32(fd);
	gamestate.victoryflag	= ReadInt8(fd);

	DiskFlopAnim(dx, dy);

#ifdef SPEAR
	for (i = 0; i < 20; i++) {
#else
	for (i = 0; i < 8; i++) {
#endif
		LevelRatios[i].kill	= ReadInt32(fd);
		LevelRatios[i].secret	= ReadInt32(fd);
		LevelRatios[i].treasure	= ReadInt32(fd);
		LevelRatios[i].time	= ReadInt32(fd);
	}

	DiskFlopAnim(dx, dy);

	SetupGameLevel();

	DiskFlopAnim(dx, dy);

	ReadBytes(fd, (byte *)tilemap, 64*64); /* MAPSIZE * MAPSIZE */

	DiskFlopAnim(dx, dy);

	for (x = 0; x < 64; x++)
		for (y = 0; y < 64; y++)
			actorat[x][y] = ReadInt32(fd);

	DiskFlopAnim(dx, dy);

	ReadBytes(fd, (byte *)areaconnect, 37*37); /* NUMAREAS * NUMAREAS */

	DiskFlopAnim(dx, dy);

	for (i = 0; i < 37; i++)
		areabyplayer[i] = ReadInt8(fd);

	DiskFlopAnim(dx, dy);

	InitActorList();

	DiskFlopAnim(dx, dy);

	/* player ptr already set up */
	id			= ReadInt32(fd); /* get id */
	player->active		= ReadInt32(fd);
	player->ticcount	= ReadInt32(fd);
	player->obclass		= ReadInt32(fd);
	player->state		= ReadInt32(fd);
	player->flags		= ReadInt8(fd);
	player->distance	= ReadInt32(fd);
	player->dir		= ReadInt32(fd);
	player->x		= ReadInt32(fd);
	player->y		= ReadInt32(fd);
	player->tilex		= ReadInt32(fd);
	player->tiley		= ReadInt32(fd);
	player->areanumber	= ReadInt8(fd);
	player->viewx		= ReadInt32(fd);
	player->viewheight	= ReadInt32(fd);
	player->transx		= ReadInt32(fd);
	player->transy		= ReadInt32(fd);
	player->angle		= ReadInt32(fd);
	player->hitpoints	= ReadInt32(fd);
	player->speed		= ReadInt32(fd);
	player->temp1		= ReadInt32(fd);
	player->temp2		= ReadInt32(fd);
	player->temp3		= ReadInt32(fd);

	/* update the id */
	for (x = 0; x < 64; x++)
		for (y = 0; y < 64; y++)
			if (actorat[x][y] == (id | 0x8000))
				actorat[x][y] = player->id | 0x8000;

	while (1) {
		DiskFlopAnim(dx, dy);

		id			= ReadInt32(fd);

		if (id == 0xFFFFFFFF)
			break;

		GetNewActor();

		new->active		= ReadInt32(fd);
		new->ticcount		= ReadInt32(fd);
		new->obclass		= ReadInt32(fd);
		new->state		= ReadInt32(fd);
		new->flags		= ReadInt8(fd);
		new->distance		= ReadInt32(fd);
		new->dir		= ReadInt32(fd);
		new->x			= ReadInt32(fd);
		new->y			= ReadInt32(fd);
		new->tilex		= ReadInt32(fd);
		new->tiley		= ReadInt32(fd);
		new->areanumber		= ReadInt8(fd);
		new->viewx		= ReadInt32(fd);
		new->viewheight		= ReadInt32(fd);
		new->transx		= ReadInt32(fd);
		new->transy		= ReadInt32(fd);
		new->angle		= ReadInt32(fd);
		new->hitpoints		= ReadInt32(fd);
		new->speed		= ReadInt32(fd);
		new->temp1		= ReadInt32(fd);
		new->temp2		= ReadInt32(fd);
		new->temp3		= ReadInt32(fd);

		for (x = 0; x < 64; x++)
			for (y = 0; y < 64; y++)
				if (actorat[x][y] == (id | 0x8000))
					actorat[x][y] = new->id | 0x8000;
	}

	DiskFlopAnim(dx, dy);

	laststatobj = statobjlist + ReadInt32(fd); /* ptr offset */
	for (i = 0; i < 400; i++) { /* MAXSTATS */
		statobjlist[i].tilex		= ReadInt8(fd);
		statobjlist[i].tiley		= ReadInt8(fd);
		statobjlist[i].shapenum		= ReadInt32(fd);
		statobjlist[i].flags		= ReadInt8(fd);
		statobjlist[i].itemnumber	= ReadInt8(fd);
		statobjlist[i].visspot 		= &spotvis[statobjlist[i].tilex][statobjlist[i].tiley];
	}

	DiskFlopAnim(dx, dy);

	for (i = 0; i < 64; i++) { /* MAXDOORS */
		doorposition[i] 		= ReadInt32(fd);
	}

	DiskFlopAnim(dx, dy);

	for (i = 0; i < 64; i++) { /* MAXDOORS */
		doorobjlist[i].tilex	= ReadInt8(fd);
		doorobjlist[i].tiley	= ReadInt8(fd);
		doorobjlist[i].vertical = ReadInt8(fd);
		doorobjlist[i].lock	= ReadInt8(fd);
		doorobjlist[i].action	= ReadInt8(fd);
		doorobjlist[i].ticcount	= ReadInt32(fd);
	}

	DiskFlopAnim(dx, dy);

	pwallstate 	= ReadInt32(fd);
	pwallx		= ReadInt32(fd);
	pwally		= ReadInt32(fd);
	pwalldir	= ReadInt32(fd);
	pwallpos	= ReadInt32(fd);

	DiskFlopAnim(dx, dy);

	CloseRead(fd);

	return 0;

loadfail:
	if (fd != -1)
		CloseRead(fd);

	Message(STR_SAVECHT1"\n"
		STR_SAVECHT2"\n"
		STR_SAVECHT3"\n"
		STR_SAVECHT4);

	IN_ClearKeysDown();
	IN_Ack();

	NewGame(1, 0);
	return -1;
}

/* ======================================================================== */

/*
=================
=
= MS_CheckParm
=
=================
*/

int MS_CheckParm(const char *check)
{
	int i;
	char *parm;

	for (i = 1; i < _argc; i++) {
		parm = _argv[i];

		while (!isalpha(*parm))		// skip - / \ etc.. in front of parm
			if (!*parm++)
				break;		// hit end of string without an alphanum

		if (!stricmp(check, parm))
			return i;
	}
	return 0;
}

/* ======================================================================== */

/*
==================
=
= BuildTables
=
==================
*/

static const double radtoint = (double)FINEANGLES/2.0/PI;

void BuildTables()
{
	int i;
	double tang, angle, anglestep;
	fixed value;

/* calculate fine tangents */

	finetangent[0] = 0;
	for (i = 1; i < FINEANGLES/8; i++) {
		tang = tan((double)i/radtoint);
		finetangent[i] = tang*TILEGLOBAL;
		finetangent[FINEANGLES/4-1-i] = TILEGLOBAL/tang;
	}

	/* fight off asymptotic behaviour at 90 degrees */
	finetangent[FINEANGLES/4-1] = finetangent[FINEANGLES/4-2]+1;

//
// costable overlays sintable with a quarter phase shift
// ANGLES is assumed to be divisable by four
//

	angle = 0.0;
	anglestep = PI/2.0/ANGLEQUAD;
	for (i = 0; i <= ANGLEQUAD; i++) {
		value = GLOBAL1*sin(angle);

		sintable[i] =
		sintable[i+ANGLES] =
		sintable[ANGLES/2-i] = value;

		sintable[ANGLES-i] =
		sintable[ANGLES/2+i] = -value;

		angle += anglestep;
	}
}

/*
===================
=
= SetupWalls
=
= Map tile values to scaled pics
=
===================
*/

void SetupWalls()
{
	int i;

	for (i=1;i<MAXWALLTILES;i++)
	{
		horizwall[i]=(i-1)*2;
		vertwall[i]=(i-1)*2+1;
	}
}

void ShowViewSize(int width)
{
	int oldwidth,oldheight;

	oldwidth = viewwidthwin;
	oldheight = viewheightwin;

	viewwidthwin = width*16;
	viewheightwin = width*16*HEIGHTRATIO;
	DrawPlayBorder();

	viewheightwin = oldheight;
	viewwidthwin = oldwidth;
}

void NewViewSize(int width)
{
	if (width > 20)
		width = 20;
	if (width < 4)
		width = 4;

	width *= vwidth / 320;

	if ((width*16) > vwidth)
		width = vwidth / 16;

	if ((width*16*HEIGHTRATIO) > (vheight - 40*vheight/200))
		width = (vheight - 40*vheight/200)/8;

	viewwidthwin = width*16*320/vwidth;
	viewheightwin = width*16*HEIGHTRATIO*320/vwidth;
	viewsize = width*320/vwidth;

	viewwidth = width*16;
	viewheight = width*16*HEIGHTRATIO;

	centerx = viewwidth/2-1;
	shootdelta = viewwidth/10;

	yoffset = (vheight-STATUSLINES*vheight/200-viewheight)/2;
	xoffset = (vwidth-viewwidth)/2;

//
// calculate trace angles and projection constants
//
	CalcProjection(FOCALLENGTH);

}

//===========================================================================

#ifndef SPEARDEMO

#ifndef SPEAR
CP_iteminfo MusicItems={CTL_X,CTL_Y,6,0,32};
CP_itemtype MusicMenu[]=
{
	{1,"Get Them!",0},
	{1,"Searching",0},
	{1,"P.O.W.",0},
	{1,"Suspense",0},
	{1,"War March",0},
	{1,"Around The Corner!",0},

	{1,"Nazi Anthem",0},
	{1,"Lurking...",0},
	{1,"Going After Hitler",0},
	{1,"Pounding Headache",0},
	{1,"Into the Dungeons",0},
	{1,"Ultimate Conquest",0},

	{1,"Kill the S.O.B.",0},
	{1,"The Nazi Rap",0},
	{1,"Twelfth Hour",0},
	{1,"Zero Hour",0},
	{1,"Ultimate Conquest",0},
	{1,"Wolfpack",0}
};
#else
CP_iteminfo MusicItems={CTL_X,CTL_Y-20,9,0,32};
CP_itemtype MusicMenu[]=
{
	{1,"Funky Colonel Bill",0},
	{1,"Death To The Nazis",0},
	{1,"Tiptoeing Around",0},
	{1,"Is This THE END?",0},
	{1,"Evil Incarnate",0},
	{1,"Jazzin' Them Nazis",0},
	{1,"Puttin' It To The Enemy",0},
	{1,"The SS Gonna Get You",0},
	{1,"Towering Above",0}
};
#endif

static const int songs[] =
{
#ifndef SPEAR
	GETTHEM_MUS,
	SEARCHN_MUS,
	POW_MUS,
	SUSPENSE_MUS,
	WARMARCH_MUS,
	CORNER_MUS,

	NAZI_OMI_MUS,
	PREGNANT_MUS,
	GOINGAFT_MUS,
	HEADACHE_MUS,
	DUNGEON_MUS,
	ULTIMATE_MUS,

	INTROCW3_MUS,
	NAZI_RAP_MUS,
	TWELFTH_MUS,
	ZEROHOUR_MUS,
	ULTIMATE_MUS,
	PACMAN_MUS
#else
	XFUNKIE_MUS,             // 0
	XDEATH_MUS,              // 2
	XTIPTOE_MUS,             // 4
	XTHEEND_MUS,             // 7
	XEVIL_MUS,               // 17
	XJAZNAZI_MUS,            // 18
	XPUTIT_MUS,              // 21
	XGETYOU_MUS,             // 22
	XTOWER2_MUS              // 23
#endif
};

void DoJukebox()
{
	int which,lastsong=-1;
	unsigned start;

	IN_ClearKeysDown();
	if (!AdLibPresent && !SoundBlasterPresent)
		return;

	MenuFadeOut();

#if !defined(SPEAR) || !defined(UPLOAD)
	start = (US_RndT() % 3) * 6;
#else
	start = 0;
#endif

	CA_CacheGrChunk(STARTFONT+1);
#ifdef SPEAR
	CacheLump(BACKDROP_LUMP_START, BACKDROP_LUMP_END);
#else
	CacheLump(CONTROLS_LUMP_START, CONTROLS_LUMP_END);
#endif
	CA_LoadAllSounds();

	fontnumber=1;
	ClearMScreen();
	VWB_DrawPic(112,184,C_MOUSELBACKPIC);
	DrawStripes(10);
	SETFONTCOLOR (TEXTCOLOR,BKGDCOLOR);

#ifndef SPEAR
	DrawWindow (CTL_X-2,CTL_Y-6,280,13*7,BKGDCOLOR);
#else
	DrawWindow (CTL_X-2,CTL_Y-26,280,13*10,BKGDCOLOR);
#endif

	DrawMenu (&MusicItems,&MusicMenu[start]);

	SETFONTCOLOR (READHCOLOR,BKGDCOLOR);
	PrintY = 15;
	WindowX = 0;
	WindowY = 320;
	US_CPrint("Robert's Jukebox");

	SETFONTCOLOR (TEXTCOLOR,BKGDCOLOR);
	VW_UpdateScreen();
	MenuFadeIn();

	do
	{
		which = HandleMenu(&MusicItems,&MusicMenu[start],NULL);
		if (which>=0)
		{
			if (lastsong >= 0)
				MusicMenu[start+lastsong].active = 1;

			StartCPMusic(songs[start + which]);
			MusicMenu[start+which].active = 2;
			DrawMenu (&MusicItems,&MusicMenu[start]);
			VW_UpdateScreen();
			lastsong = which;
		}
	} while(which>=0);

	MenuFadeOut();
	IN_ClearKeysDown();
#ifdef SPEAR
	UnCacheLump(BACKDROP_LUMP_START, BACKDROP_LUMP_END);
#else
	UnCacheLump(CONTROLS_LUMP_START, CONTROLS_LUMP_END);
#endif
}
#endif

/* ======================================================================== */

/*
==========================
=
= ShutdownId
=
= Shuts down all ID_?? managers
=
==========================
*/

void ShutdownId()
{
	US_Shutdown();
	SD_Shutdown();
	IN_Shutdown();
	VW_Shutdown();
	CA_Shutdown();
	PM_Shutdown();
	MM_Shutdown();
}

/*
=====================
=
= NewGame
=
= Set up new game to start from the beginning
=
=====================
*/

void NewGame(int difficulty, int episode)
{
	memset(&gamestate, 0, sizeof(gamestate));

	gamestate.difficulty = difficulty;
	gamestate.weapon = gamestate.bestweapon
		= gamestate.chosenweapon = wp_pistol;
	gamestate.health = 100;
	gamestate.ammo = STARTAMMO;
	gamestate.lives = 3;
	gamestate.nextextra = EXTRAPOINTS;
	gamestate.episode = episode;

	startgame = true;
}

/*
==========================
=
= InitGame
=
= Load a few things right away
=
==========================
*/

void InitGame()
{
	int i;

	MM_Startup();
	PM_Startup();
	CA_Startup();
	VW_Startup();
	IN_Startup();
	SD_Startup();
	US_Startup();

//
// build some tables
//

	for (i = 0;i < MAPSIZE; i++)
	{
		farmapylookup[i] = i*64;
	}

	ReadConfig();

/* load in and lock down some basic chunks */

	CA_CacheGrChunk(STARTFONT);
	CA_CacheGrChunk(STARTTILE8);
	for (i = LATCHPICS_LUMP_START; i <= LATCHPICS_LUMP_END; i++)
		CA_CacheGrChunk(i);

	BuildTables();
	SetupWalls();

	NewViewSize(viewsize);


//
// initialize variables
//
	InitRedShifts();

	IN_CheckAck();
//
// HOLDING DOWN 'M' KEY?
//
#ifndef SPEARDEMO
	if (IN_KeyDown(sc_M))
		DoJukebox();
#endif
}

/*
=====================
=
= DemoLoop
=
=====================
*/

void DemoLoop()
{
	static int LastDemo;

	int i;
//
// main game cycle
//

	LastDemo = 0;

	/*
	StartCPMusic(INTROSONG);

	if (!NoWait)
		PG13();

	i = MS_CheckParm("playdemo");
	if (i && ((i+1) < _argc)) {
		i++;
		for (; i < _argc; i++) {
			if (_argv[i][0] == '-')
				break;
			IN_ClearKeysDown();
			if (PlayDemoFromFile(_argv[i]))
				IN_UserInput(3 * 70);
		}
		VW_FadeOut();
	}

	if (MS_CheckParm("demotest")) {
	#ifndef SPEARDEMO
		for (i = 0; i < 4; i++)
			PlayDemo(i);
	#else
		PlayDemo(0);
	#endif
	}
	*/

	while (1)
	{
		/*
		while (!NoWait)
		{
//
// title page
//
			MM_SortMem ();
#ifdef SPEAR
			CA_CacheGrChunk (TITLEPALETTE);

			CA_CacheGrChunk (TITLE1PIC);
			VWB_DrawPic (0,0,TITLE1PIC);
			CA_UnCacheGrChunk (TITLE1PIC);

			CA_CacheGrChunk (TITLE2PIC);
			VWB_DrawPic (0,80,TITLE2PIC);
			CA_UnCacheGrChunk(TITLE2PIC);
			VW_UpdateScreen();
			VL_FadeIn(0,255,grsegs[TITLEPALETTE],30);

			CA_UnCacheGrChunk (TITLEPALETTE);
#else
			VL_CacheScreen(TITLEPIC);
			VW_UpdateScreen ();
			VW_FadeIn();
#endif
			if (IN_UserInput(TickBase*15))
				break;
			VW_FadeOut();
//
// credits page
//
			VL_CacheScreen(CREDITSPIC);
			VW_UpdateScreen();
			VW_FadeIn ();
			if (IN_UserInput(TickBase*10))
				break;
			VW_FadeOut ();
//
// high scores
//
			DrawHighScores();
			VW_UpdateScreen();
			VW_FadeIn();

			if (IN_UserInput(TickBase*10))
				break;
//
// demo
//
			#ifndef SPEARDEMO
			PlayDemo(LastDemo++%4);
			#else
			PlayDemo(0);
			#endif

			if (playstate == ex_abort)
				break;

			StartCPMusic(INTROSONG);
		}

		VW_FadeOut();

		if (IN_KeyDown(sc_Tab) && MS_CheckParm("debugmode"))
			RecordDemo();
		else
			*/
			US_ControlPanel(0);

		if (startgame || loadedgame)
		{
			GameLoop();
			VW_FadeOut();
			StartCPMusic(INTROSONG);
		}
	}
}


/* ======================================================================== */


/*
==========================
=
= WolfMain
=
==========================
*/

int WolfMain(int argc, char *argv[])
{
	_argc = argc;
	_argv = argv;

	if (MS_CheckParm("version")) {
		printf("Game: %s\n", GAMENAME);
		Quit(NULL);
	}

//	printf("Now Loading %s\n", GAMENAME);

	CheckForEpisodes();

	InitGame();

	DemoLoop();

	Quit("Demo loop exited???");

	return 0;
}
