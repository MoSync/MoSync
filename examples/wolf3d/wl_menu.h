#ifndef __WL_MENU_H__
#define __WL_MENU_H__

#ifdef SPEAR

#define BORDCOLOR	0x99
#define BORD2COLOR	0x93
#define DEACTIVE	0x9b
#define BKGDCOLOR	0x9d
//#define STRIPE		0x9c

#define MenuFadeOut()	VL_FadeOut(0,255,0,0,51,10)

#else

#define BORDCOLOR	0x29
#define BORD2COLOR	0x23
#define DEACTIVE	0x2b
#define BKGDCOLOR	0x2d
#define STRIPE		0x2c

#define MenuFadeOut()	VL_FadeOut(0,255,43,0,0,10)

#endif

#define READCOLOR	0x4a
#define READHCOLOR	0x47
#define VIEWCOLOR	0x7f
#define TEXTCOLOR	0x17
#define HIGHLIGHT	0x13
#define MenuFadeIn()	VL_FadeIn(0,255,gamepal,10)


#define MENUSONG	WONDERIN_MUS

#ifndef SPEAR
#define INTROSONG	NAZI_NOR_MUS
#else
#define INTROSONG	XTOWER2_MUS
#endif

#define SENSITIVE	60
#define CENTER		SENSITIVE*2

#define MENU_X	76
#define MENU_Y	55
#define MENU_W	178
#if defined(UPLOAD)
#define MENU_H	13*10+6
#else
#define MENU_H	13*9+6
#endif

#define SM_X	48
#define SM_W	250

#define SM_Y1	20
#define SM_H1	4*13-7
#define SM_Y2	SM_Y1+5*13
#define SM_H2	4*13-7
#define SM_Y3	SM_Y2+5*13
#define SM_H3	3*13-7

#define CTL_X	24
#define CTL_Y	70
#define CTL_W	284
#define CTL_H	13*7-7

#define LSM_X	85
#define LSM_Y	55
#define LSM_W	175
#define LSM_H	10*13+10

#define NM_X	50
#define NM_Y	100
#define NM_W	225
#define NM_H	13*4+15

#define NE_X	10
#define NE_Y	23
#define NE_W	320-NE_X*2
#define NE_H	200-NE_Y*2

#define CST_X		20
#define CST_Y		48
#define CST_START	60
#define CST_SPC		60


//
// TYPEDEFS
//
typedef struct {
	int x,y,amount,curpos,indent;
} CP_iteminfo;

typedef void (* MenuFunc)(int temp1);

typedef struct {
	int active;
	char string[36];
	MenuFunc routine;
} CP_itemtype;

typedef struct {
	int allowed[4];
} CustomCtrls;

extern CP_itemtype MainMenu[], NewEMenu[];
extern CP_iteminfo MainItems;

//
// FUNCTION PROTOTYPES
//
void SetupControlPanel(void);
void CleanupControlPanel(void);

void DrawMenu(CP_iteminfo *item_i,CP_itemtype *items);
int HandleMenu(CP_iteminfo *item_i, CP_itemtype *items, void (*routine)(int w));
void ClearMScreen(void);
void DrawWindow(int x,int y,int w,int h,int wcolor);
void DrawOutline(int x,int y,int w,int h,int color1,int color2);
void WaitKeyUp(void);
void ReadAnyControl(ControlInfo *ci);
void TicDelay(int count);
void CacheLump(int lumpstart,int lumpend);
void UnCacheLump(int lumpstart,int lumpend);
void StartCPMusic(int song);
int  Confirm(const char *string);
void Message(const char *string);
void CheckPause(void);
void ShootSnd(void);
void FreeMusic(void);
void BossKey(void);

void DrawGun(CP_iteminfo *item_i,CP_itemtype *items,int x,int *y,int which,int basey,void (*routine)(int w));
void DrawHalfStep(int x,int y);
void EraseGun(CP_iteminfo *item_i,CP_itemtype *items,int x,int y,int which);
void SetMenuTextColor(CP_itemtype *items,int hlight);
void DrawMenuGun(CP_iteminfo *iteminfo);
void DrawStripes(int y);

void DefineMouseBtns(void);
void DefineJoyBtns(void);
void DefineKeyBtns(void);
void DefineKeyMove(void);
void EnterCtrlData(int index,CustomCtrls *cust,void (*DrawRtn)(int),void (*PrintRtn)(int),int type);

void DrawMainMenu(void);
void DrawSoundMenu(void);
void DrawLoadSaveScreen(int loadsave);
void DrawNewEpisode(void);
void DrawNewGame(void);
void DrawChangeView(int view);
void DrawMouseSens(void);
void DrawCtlScreen(void);
void DrawCustomScreen(void);
void DrawLSAction(int which);
void DrawCustMouse(int hilight);
void DrawCustJoy(int hilight);
void DrawCustKeybd(int hilight);
void DrawCustKeys(int hilight);
void PrintCustMouse(int i);
void PrintCustJoy(int i);
void PrintCustKeybd(int i);
void PrintCustKeys(int i);

void PrintLSEntry(int w,int color);
void TrackWhichGame(int w);
void DrawNewGameDiff(int w);
void FixupCustom(int w);

void CP_NewGame(void);
void CP_Sound(void);
int  CP_LoadGame(int quick);
int  CP_SaveGame(int quick);
void CP_Control(void);
void CP_ChangeView(void);
void CP_ExitOptions(void);
void CP_Quit(void);
void CP_ViewScores(void);
int  CP_EndGame(void);
int  CP_CheckQuick(unsigned scancode);
void CustomControls(void);
void MouseSensitivity(void);

void CheckForEpisodes(void);

//
// VARIABLES
//
#ifdef __cplusplus
extern "C"{
#endif
extern int SaveGamesAvail[10],StartGame,SoundStatus;
extern char SaveGameNames[10][32],SaveName[13];
#ifdef __cplusplus
}
#endif

typedef // added by Niklas
enum
{
	newgame,
	soundmenu,
	control,
	loadgame,
	savegame,
	changeview,
#ifdef UPLOAD
	readthis,
#endif
	viewscores,
	backtodemo,
	quit
} menuitems;

//
// WL_INTER
//
typedef struct {
	int kill,secret,treasure;
	long time;
} LRstruct;

extern LRstruct LevelRatios[];

void Write(int x,int y,const char *string);
int GetYorN(int x,int y,int pic);

#endif
