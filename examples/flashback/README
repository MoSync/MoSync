
REminiscence README
Release version: 0.1.9 (Mar 16 2007)
-------------------------------------------------------------------------------


About:
------

REminiscence is a re-implementation of the engine used in the game Flashback
made by Delphine Software and released in 1992. More informations about the
game can be found at [1], [2] and [3].


Supported Versions:
-------------------

Only the PC DOS versions are supported. The engine has been reported to work
with english, french, german and spanish versions of the game.


Compiling:
----------

Tweak the Makefile if needed and type make (only gcc3 has been tested so far).
The SDL and zlib libraries are required.


Data Files:
-----------

You will need the original files, here is the required list :

	FB_TXT.FNT
	GLOBAL.ICN
	GLOBAL.FIB
	GLOBAL.SPC
	*.OFF
	*.SPR
	*.MAP
	*.PAL
	*.ANI
	*.CT
	*.MBK
	*.OBJ
	*.PGE
	*.RP
	*.TBN
	*.CMD
	*.POL
	*CINE.*

If you have a version distributed by SSI, you'll have to rename some files :

	logosssi.cmd -> logos.cmd
	logosssi.pol -> logos.pol
	menu1ssi.map -> menu1.map
	menu1ssi.pal -> menu1.pal

In order to hear music, you'll need the original music files (.mod) of the
amiga version. Copy them to the DATA directory and rename them like this :

	mod.flashback-ascenseur
	mod.flashback-ceinturea
	mod.flashback-chute
	mod.flashback-desintegr
	mod.flashback-donneobjt
	mod.flashback-fin
	mod.flashback-fin2
	mod.flashback-game_over
	mod.flashback-holocube
	mod.flashback-introb
	mod.flashback-jungle
	mod.flashback-logo
	mod.flashback-memoire
	mod.flashback-missionca
	mod.flashback-options1
	mod.flashback-options2
	mod.flashback-reunion
	mod.flashback-taxi
	mod.flashback-teleport2
	mod.flashback-teleporta
	mod.flashback-voyage


Running:
--------

By default, the engine will try to load the game data files from the 'DATA'
directory (as the original game did). The savestates are saved in the current
directory. These paths can be changed using command line switches :

	Usage: rs [OPTIONS]...
  	--datapath=PATH   Path to data files (default 'DATA')
  	--savepath=PATH   Path to save files (default '.')

In-game hotkeys :

    Arrow Keys      move Conrad
    Enter           use the current inventory object
    Shift           talk / use / run / shoot
    Escape          display the options
    Backspace       display the inventory
    Alt Enter       toggle windowed/fullscreen mode
    Alt + and -     change video scaler
    Ctrl S          save game state
    Ctrl L          load game state
    Ctrl + and -    change game state slot
    Ctrl R          toggle input keys record
    Ctrl P          toggle input keys replay

Debug hotkeys :

    Ctrl F          toggle fast mode
    Ctrl I          set Conrad life counter to 32767
    Ctrl B          toggle display of updated dirty blocks
    Ctrl M			mirror mode (just a hack, really)


Credits:
--------

Delphine Software, obviously, for making another great game.
Yaz0r, Pixel and gawd for sharing information they gathered on the game.


Contact:
--------

Gregory Montoir, cyx@users.sourceforge.net


URLs:
-----

[1] http://www.mobygames.com/game/flashback-the-quest-for-identity
[2] http://en.wikipedia.org/wiki/Flashback:_The_Quest_for_Identity
[3] http://ramal.free.fr/fb_en.htm
