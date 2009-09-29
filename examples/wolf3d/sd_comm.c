#include "wl_def.h"

/*
=====================
=
= InitDigiMap
=
=====================
*/

int DigiMap[LASTSOUND];

static int wolfdigimap[] =
{
#ifndef SPEAR
	HALTSND,                0,
	DOGBARKSND,             1,
	CLOSEDOORSND,           2,
	OPENDOORSND,            3,
	ATKMACHINEGUNSND,       4,
	ATKPISTOLSND,           5,
	ATKGATLINGSND,          6,
	SCHUTZADSND,            7,
	GUTENTAGSND,            8,
	MUTTISND,               9,
	BOSSFIRESND,            10,
	SSFIRESND,              11,
	DEATHSCREAM1SND,        12,
	DEATHSCREAM2SND,        13,
	DEATHSCREAM3SND,        13,
	TAKEDAMAGESND,          14,
	PUSHWALLSND,            15,
	LEBENSND,               20,
	NAZIFIRESND,            21,
	SLURPIESND,             22,
	YEAHSND,		32,
#ifndef UPLOAD
	DOGDEATHSND,            16,
	AHHHGSND,               17,
	DIESND,                 18,
	EVASND,                 19,
	TOT_HUNDSND,            23,
	MEINGOTTSND,            24,
	SCHABBSHASND,           25,
	HITLERHASND,            26,
	SPIONSND,               27,
	NEINSOVASSND,           28,
	DOGATTACKSND,           29,
	LEVELDONESND,           30,
	MECHSTEPSND,		31,

	SCHEISTSND,		33,
	DEATHSCREAM4SND,	34,		/* AIIEEE */
	DEATHSCREAM5SND,	35,		/* DEE-DEE */
	DONNERSND,		36,		/* EPISODE 4 BOSS DIE */
	EINESND,		37,		/* EPISODE 4 BOSS SIGHTING */
	ERLAUBENSND,		38,		/* EPISODE 6 BOSS SIGHTING */
	DEATHSCREAM6SND,	39,		/* FART */
	DEATHSCREAM7SND,	40,		/* GASP */
	DEATHSCREAM8SND,	41,		/* GUH-BOY! */
	DEATHSCREAM9SND,	42,		/* AH GEEZ! */
	KEINSND,		43,		/* EPISODE 5 BOSS SIGHTING */
	MEINSND,		44,		/* EPISODE 6 BOSS DIE */
	ROSESND,		45,		/* EPISODE 5 BOSS DIE */
#endif

#else /* SPEAR OF DESTINY DIGISOUNDS */
	HALTSND,                0,
	CLOSEDOORSND,           2,
	OPENDOORSND,            3,
	ATKMACHINEGUNSND,       4,
	ATKPISTOLSND,           5,
	ATKGATLINGSND,          6,
	SCHUTZADSND,            7,
	BOSSFIRESND,            8,
	SSFIRESND,              9,
	DEATHSCREAM1SND,        10,
	DEATHSCREAM2SND,        11,
	TAKEDAMAGESND,          12,
	PUSHWALLSND,            13,
	AHHHGSND,               15,
	LEBENSND,               16,
	NAZIFIRESND,            17,
	SLURPIESND,             18,
	LEVELDONESND,           22,
	DEATHSCREAM4SND,	23,		// AIIEEE
	DEATHSCREAM3SND,        23,		// DOUBLY-MAPPED!!!
	DEATHSCREAM5SND,	24,		// DEE-DEE
	DEATHSCREAM6SND,	25,		// FART
	DEATHSCREAM7SND,	26,		// GASP
	DEATHSCREAM8SND,	27,		// GUH-BOY!
	DEATHSCREAM9SND,	28,		// AH GEEZ!
	GETGATLINGSND,		38,		// Got Gat replacement
#ifndef SPEARDEMO
	DOGBARKSND,             1,
	DOGDEATHSND,            14,
	SPIONSND,               19,
	NEINSOVASSND,           20,
	DOGATTACKSND,           21,
	TRANSSIGHTSND,		29,		// Trans Sight
	TRANSDEATHSND,		30,		// Trans Death
	WILHELMSIGHTSND,	31,		// Wilhelm Sight
	WILHELMDEATHSND,	32,		// Wilhelm Death
	UBERDEATHSND,		33,		// Uber Death
	KNIGHTSIGHTSND,		34,		// Death Knight Sight
	KNIGHTDEATHSND,		35,		// Death Knight Death
	ANGELSIGHTSND,		36,		// Angel Sight
	ANGELDEATHSND,		37,		// Angel Death
	GETSPEARSND,		39,		// Got Spear replacement
#endif
#endif
	LASTSOUND
};

void InitDigiMap()
{
	int *map, i;

	for (i = 0; i < LASTSOUND; i++)
		DigiMap[i] = -1;

	for (map = wolfdigimap; *map != LASTSOUND; map += 2)
		DigiMap[map[0]] = map[1];
}
