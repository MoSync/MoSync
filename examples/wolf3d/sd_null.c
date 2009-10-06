#include "wl_def.h"

#include <IX_AUDIOBUFFER.h>

//#include <pthread.h>
//#include <sys/ioctl.h>
//#include <SDL.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <fcntl.h>
//#include <unistd.h>
                     
#include "fmopl.h"

#define PACKED __attribute__((packed))

typedef	struct {
	longword length;
	word priority;
} PACKED SoundCommon;

typedef	struct {
	SoundCommon common;
	byte data[1];
} PACKED PCSound;

typedef	struct {
	byte mChar, cChar, mScale, cScale, mAttack, cAttack, mSus, cSus,
		mWave, cWave, nConn, voice, mode, unused[3];
} PACKED Instrument;

typedef	struct {
	SoundCommon common;
	Instrument inst;
	byte block, data[1];
} PACKED AdLibSound;

typedef	struct {
	word length, values[1];
} PACKED MusicGroup;


MAAudioBufferInfo abInfo;

boolean AdLibPresent, SoundBlasterPresent;
	
SDMode SoundMode;
SMMode MusicMode;
SDSMode DigiMode;

static volatile boolean sqActive;

static fixed globalsoundx, globalsoundy;
static int leftchannel, rightchannel;

static volatile boolean SoundPositioned;

static word *DigiList;

static volatile boolean SD_Started;
static volatile int audiofd = -1;

static volatile int NextSound;
static volatile int SoundPlaying;
static volatile int SoundPlayPos;
static volatile int SoundPlayLen;
static volatile int SoundPage;
static volatile int SoundLen;
static volatile int L;
static volatile int R;
static byte *SoundData;

static FM_OPL *OPL;

static MusicGroup *Music;
static volatile int NewMusic;

static volatile int NewAdlib;
static volatile int AdlibPlaying;

typedef struct pthread_t_ *pthread_t;
static pthread_t hSoundThread;

static int CurDigi;
static int CurAdlib;

static boolean SPHack;

#define SAMPLERATE 11025
#define SAMPLES_PER_UPDATE ((64*SAMPLERATE)/44100)

static short int sndbuf[4096];
static short int musbuf[(sizeof(sndbuf)/sizeof(sndbuf[0]))/2];

#ifdef ARM_WINCE
static FM_OPL *ym3812_L = NULL;
static FM_OPL *ym3812_R = NULL;

int synth_mixer (word *buffer, int count)
{
  word *ptr = buffer;

  if (!buffer)
  {
	  fprintf(stderr, "synth_mixer(): !buffer \n");
      return 0;
  }


  YM3812UpdateOne (ym3812_L, ptr, count);
  YM3812UpdateOne (ym3812_R, ptr+1, count);

  return count;
}

/* returns # of frames, not bytes */
int mix_sound(int count)
{
	int i;

	if (count > sizeof(sndbuf))
		count = sizeof(sndbuf);

	i = synth_mixer(sndbuf, count);

	if (i <= 0)
		i = count;

	return i;
}

/* SDL wants its buffer to be filled completely, so we generate samples
   to fill the whole buffer.
*/
static void fill_audio (void *udata, byte *stream, int len)
{
  int remain = 0;
  int shift = 2;

  while (len > 0) {
    remain = mix_sound(len >> shift) << shift;
    memcpy(stream, sndbuf, remain);
    stream += remain;
    len -= remain;
  }
}
#endif

int MusicLength;
int MusicCount;
word *MusicData;
byte AdlibBlock;
byte *AdlibData;
int AdlibLength;

void InitSoundThread() {
	MusicLength = 0;
	MusicCount = 0;
	MusicData = NULL;
	AdlibBlock = 0;
	AdlibData = NULL;
	AdlibLength = -1;

	OPLWrite(OPL, 0x01, 0x20); /* Set WSE=1 */
	OPLWrite(OPL, 0x08, 0x00); /* Set CSM=0 & SEL=0 */
}

static void *SoundThread(void *data)
{
	int i, snd;
	short int samp;
	word dat;
	AdLibSound *AdlibSnd;
	Instrument *inst;

/* Yeah, one day I'll rewrite this... */
	
//	while (SD_Started) {
		if (audiofd != -1) {
			if (NewAdlib != -1) {
				AdlibPlaying = NewAdlib;
				AdlibSnd = (AdLibSound *)audiosegs[STARTADLIBSOUNDS+AdlibPlaying];
				inst = (Instrument *)&AdlibSnd->inst;
#define alChar		0x20
#define alScale		0x40
#define alAttack	0x60
#define alSus		0x80
#define alFeedCon	0xC0
#define alWave		0xE0

				OPLWrite(OPL, 0 + alChar, 0);
				OPLWrite(OPL, 0 + alScale, 0);
				OPLWrite(OPL, 0 + alAttack, 0);
				OPLWrite(OPL, 0 + alSus, 0);
				OPLWrite(OPL, 0 + alWave, 0);
				OPLWrite(OPL, 3 + alChar, 0);
				OPLWrite(OPL, 3 + alScale, 0);
				OPLWrite(OPL, 3 + alAttack, 0);
				OPLWrite(OPL, 3 + alSus, 0);
				OPLWrite(OPL, 3 + alWave, 0);
				OPLWrite(OPL, 0xA0, 0);
				OPLWrite(OPL, 0xB0, 0);
				
				OPLWrite(OPL, 0 + alChar, inst->mChar);
				OPLWrite(OPL, 0 + alScale, inst->mScale);
				OPLWrite(OPL, 0 + alAttack, inst->mAttack);
				OPLWrite(OPL, 0 + alSus, inst->mSus);
				OPLWrite(OPL, 0 + alWave, inst->mWave);
				OPLWrite(OPL, 3 + alChar, inst->cChar);
				OPLWrite(OPL, 3 + alScale, inst->cScale);
				OPLWrite(OPL, 3 + alAttack, inst->cAttack);
				OPLWrite(OPL, 3 + alSus, inst->cSus);
				OPLWrite(OPL, 3 + alWave, inst->cWave);

				//OPLWrite(OPL, alFeedCon, inst->nConn);
				OPLWrite(OPL, alFeedCon, 0);
				
				AdlibBlock = ((AdlibSnd->block & 7) << 2) | 0x20;
				AdlibData = (byte *)&AdlibSnd->data;
				AdlibLength = AdlibSnd->common.length*5;
				//OPLWrite(OPL, 0xB0, AdlibBlock);
				NewAdlib = -1;
			}
			
			if (NewMusic != -1) {
				NewMusic = -1;
				MusicLength = Music->length;
				MusicData = Music->values;
				MusicCount = 0;
			}
			for (i = 0; i < (sizeof(musbuf)/(sizeof(musbuf[0])))/SAMPLES_PER_UPDATE/*4*/; i++) {
				if (sqActive) {
					while (MusicCount <= 0) {
						dat = *MusicData++;
						MusicCount = *MusicData++;
						MusicLength -= 4;
						OPLWrite(OPL, dat & 0xFF, dat >> 8);
					}
					if (MusicLength <= 0) {
						NewMusic = 1;
					}
					MusicCount--;
				}

				if (AdlibPlaying != -1) {
					if (AdlibLength == 0) {
						//OPLWrite(OPL, 0xB0, AdlibBlock);
					} else if (AdlibLength == -1) {
						OPLWrite(OPL, 0xA0, 00);
						OPLWrite(OPL, 0xB0, AdlibBlock);
						AdlibPlaying = -1;
					} else if ((AdlibLength % 5) == 0) {
						OPLWrite(OPL, 0xA0, *AdlibData);
						OPLWrite(OPL, 0xB0, AdlibBlock & ~2);
						AdlibData++;
					}
					AdlibLength--;
				}

				YM3812UpdateOne(OPL, &musbuf[i*SAMPLES_PER_UPDATE], SAMPLES_PER_UPDATE);
			}
			if (NextSound != -1) {
				SoundPlaying = NextSound;
				SoundPage = DigiList[(SoundPlaying * 2) + 0];
				SoundData = PM_GetSoundPage(SoundPage);
				SoundLen = DigiList[(SoundPlaying * 2) + 1];
				SoundPlayLen = (SoundLen < 4096) ? SoundLen : 4096;
				SoundPlayPos = 0;
				NextSound = -1;
			}
			for (i = 0; i < (sizeof(sndbuf)/sizeof(sndbuf[0])); i += 2) {
				if (SoundPlaying != -1) {
					if (SoundPositioned) {
						samp = (SoundData[(SoundPlayPos >> 16)] << 8)^0x8000;
						snd = samp*(16-L)/32+musbuf[i/2];
						//snd = (((signed short)((SoundData[(SoundPlayPos >> 16)] << 8)^0x8000))*(16-L)>>5)+musbuf[i/2];
						if (snd > 32767)
							snd = 32767;
						if (snd < -32768)
							snd = -32768;
						sndbuf[i+0] = snd;
						samp = (SoundData[(SoundPlayPos >> 16)] << 8)^0x8000;
						snd = samp*(16-R)/32+musbuf[i/2];
						//snd = (((signed short)((SoundData[(SoundPlayPos >> 16)] << 8)^0x8000))*(16-R)>>5)+musbuf[i/2];
						if (snd > 32767)
							snd = 32767;
						if (snd < -32768)
							snd = -32768;
						sndbuf[i+1] = snd;
					} else {
						snd = (((signed short)((SoundData[(SoundPlayPos >> 16)] << 8)^0x8000))>>2)+musbuf[i/2];
						if (snd > 32767)
							snd = 32767;
						if (snd < -32768)
							snd = -32768;
						sndbuf[i+0] = snd;
						snd = (((signed short)((SoundData[(SoundPlayPos >> 16)] << 8)^0x8000))>>2)+musbuf[i/2];
						if (snd > 32767)
							snd = 32767;
						if (snd < -32768)
							snd = -32768;
						sndbuf[i+1] = snd;
					}
					SoundPlayPos += (7000 << 16) / SAMPLERATE; //10402; /* 7000 / 44100 * 65536 */
					if ((SoundPlayPos >> 16) >= SoundPlayLen) {
						//SoundPlayPos = 0;
						SoundPlayPos -= (SoundPlayLen << 16);
						SoundLen -= 4096;
						SoundPlayLen = (SoundLen < 4096) ? SoundLen : 4096;
						if (SoundLen <= 0) {
							SoundPlaying = -1;
							SoundPositioned = false;
						} else {
							SoundPage++;
							SoundData = PM_GetSoundPage(SoundPage);
						}
					}
				} else {
					sndbuf[i+0] = musbuf[i/2];
					sndbuf[i+1] = musbuf[i/2];
				}
			}
			//write(audiofd, sndbuf, sizeof(sndbuf));
		}
	//}
	return NULL;
}

void AudioBufferFill() {
	SoundThread(NULL);
	maAudioBufferReady();
}

static void Blah()
{
        memptr  list;
        word    *p, pg;
        int     i;

        MM_GetPtr(&list,PMPageSize);
        p = PM_GetPage(ChunksInFile - 1);
        memcpy((void *)list,(void *)p,PMPageSize);
        
        pg = PMSoundStart;
        for (i = 0;i < PMPageSize / (sizeof(word) * 2);i++,p += 2)
        {
                if (pg >= ChunksInFile - 1)
                        break;
                pg += (p[1] + (PMPageSize - 1)) / PMPageSize;
        }
        MM_GetPtr((memptr *)&DigiList, i * sizeof(word) * 2);
        memcpy((void *)DigiList, (void *)list, i * sizeof(word) * 2);
        MM_FreePtr(&list);        
}

void SD_Startup()
{
	//audio_buf_info info;
	int want, set;
	
	if (SD_Started)
		return;

	Blah();

	InitDigiMap();
	
	OPL = OPLCreate(OPL_TYPE_YM3812, 3579545, SAMPLERATE);

	//audiofd = open("/dev/dsp", O_WRONLY);
	//if (audiofd == -1) {
	//	perror("open(\"/dev/dsp\")");
	//	return;
	//}

	/*
	SDL_AudioSpec a;

  	if (SDL_Init(SDL_INIT_AUDIO|SDL_INIT_NOPARACHUTE) != 0) {
    		printf (stderr, "SDL: %s\n", SDL_GetError());
    		return;
  	}


  	a.freq = 44100;
	a.format = AUDIO_FMT_S16;
  	a.channels = 2;
  	a.samples = sizeof(sndbuf) * 2;
  	a.callback = fill_audio;
  	a.userdata = NULL;

  	if (SDL_OpenAudio (&a, NULL) < 0) {
    		printf (stderr, "SDL: %s\n", SDL_GetError());
    		return;
  	}

  	SDL_PauseAudio (0);
*/
	abInfo.sampleRate = SAMPLERATE;
	abInfo.fmt = AUDIO_FMT_S16;
	abInfo.numChannels = 2;
	abInfo.bufferSize = sizeof(sndbuf);
	abInfo.buffer = sndbuf;

  	audiofd = 0;

	set = (8 << 16) | 10;
	//if (ioctl(audiofd, SNDCTL_DSP_SETFRAGMENT, &set) == -1) {
	//	perror("ioctl SNDCTL_DSP_SETFRAGMENT");
	//	return;
	//}

	//want = set = AFMT_S16_LE;
	//if (ioctl(audiofd, SNDCTL_DSP_SETFMT, &set) == -1) {
	//	perror("ioctl SNDCTL_DSP_SETFMT");
	//	return;
	//}
	//if (want != set) {
	//	fprintf(stderr, "Format: Wanted %d, Got %d\n", want, set);
	//	return;
	//}

	want = set = 1;
	//if (ioctl(audiofd, SNDCTL_DSP_STEREO, &set) == -1) {
	//	perror("ioctl SNDCTL_DSP_STEREO");
	//	return;
	//}
	//if (want != set) {
	//	fprintf(stderr, "Stereo: Wanted %d, Got %d\n", want, set);
	//	return;
	//}

	//want = set = 44100;
	//if (ioctl(audiofd, SNDCTL_DSP_SPEED, &set) == -1) {
	//	perror("ioctl SNDCTL_DSP_SPEED");
	//	return;
	//}
	//if (want != set) {
	//	fprintf(stderr, "Speed: Wanted %d, Got %d\n", want, set);
	//	return;
	//}

	//if (ioctl(audiofd, SNDCTL_DSP_GETOSPACE, &info) == -1) {
	//	perror("ioctl SNDCTL_DSP_GETOSPACE");
	//	return;
	//}
	//printf("Fragments: %d\n", info.fragments);
	//printf("FragTotal: %d\n", info.fragstotal);
	//printf("Frag Size: %d\n", info.fragsize);
	//printf("Bytes    : %d\n", info.bytes);

	NextSound = -1;
	SoundPlaying = -1;
	CurDigi = -1;
	CurAdlib = -1;
	NewAdlib = -1;
	NewMusic = -1;
	AdlibPlaying = -1;
	sqActive = false;
	
	SD_Started = true;
	
	/*
	if (pthread_create(&hSoundThread, NULL, SoundThread, NULL) != 0) {
		SD_Started = false;

		perror("pthread_create");
		return;
	}
	*/

	InitSoundThread();

	maAudioBufferInit(&abInfo);
}

void SD_Shutdown()
{
	if (!SD_Started)
		return;

	SD_MusicOff();
	SD_StopSound();

	SD_Started = false;
	
	if (audiofd != -1) {
//		SDL_PauseAudio (1);
//		SDL_CloseAudio();
		maAudioBufferReady();
		maAudioBufferClose();
	}

	audiofd = -1;
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_PlaySound() - plays the specified sound on the appropriate hardware
//
///////////////////////////////////////////////////////////////////////////
boolean SD_PlaySound(soundnames sound)
{
	SoundCommon *s;
	
	s = (SoundCommon *)audiosegs[STARTADLIBSOUNDS + sound];

	if (DigiMap[sound] != -1) {
		if ((SoundPlaying == -1) || (CurDigi == -1) || 
		(s->priority >= ((SoundCommon *)audiosegs[STARTADLIBSOUNDS+CurDigi])->priority) ) {
			if (SPHack) {
				SPHack = false;
			} else {
				SoundPositioned = false;
			}
			CurDigi = sound;
			NextSound = DigiMap[sound];
			return true;
		}
		return false;
	}
	
	if ((AdlibPlaying == -1) || (CurAdlib == -1) || 
	(s->priority >= ((SoundCommon *)audiosegs[STARTADLIBSOUNDS+CurAdlib])->priority) ) {
		CurAdlib = sound;
		NewAdlib = sound;
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_SoundPlaying() - returns the sound number that's playing, or 0 if
//		no sound is playing
//
///////////////////////////////////////////////////////////////////////////
word SD_SoundPlaying()
{
	if (SoundPlaying != -1)
		return CurDigi;
	if (AdlibPlaying != -1)
		return CurAdlib;
	return 0;
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_StopSound() - if a sound is playing, stops it
//
///////////////////////////////////////////////////////////////////////////
void SD_StopSound()
{
	SoundPlaying = -1;
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_WaitSoundDone() - waits until the current sound is done playing
//
///////////////////////////////////////////////////////////////////////////
void SD_WaitSoundDone()
{
	while (SD_SoundPlaying()) ;
}

/*
==========================
=
= SetSoundLoc - Given the location of an object (in terms of global
=	coordinates, held in globalsoundx and globalsoundy), munges the values
=	for an approximate distance from the left and right ear, and puts
=	those values into leftchannel and rightchannel.
=
= JAB
=
==========================
*/

#define ATABLEMAX 15
static byte righttable[ATABLEMAX][ATABLEMAX * 2] = {
{ 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 7, 7, 7, 6, 0, 0, 0, 0, 0, 1, 3, 5, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 7, 7, 6, 4, 0, 0, 0, 0, 0, 2, 4, 6, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 7, 6, 6, 4, 1, 0, 0, 0, 1, 2, 4, 6, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 7, 6, 5, 4, 2, 1, 0, 1, 2, 3, 5, 7, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 6, 5, 4, 3, 2, 2, 3, 3, 5, 6, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 6, 6, 5, 4, 4, 4, 4, 5, 6, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 6, 6, 5, 5, 5, 6, 6, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 6, 6, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8}
};
static byte lefttable[ATABLEMAX][ATABLEMAX * 2] = {
{ 8, 8, 8, 8, 8, 8, 8, 8, 5, 3, 1, 0, 0, 0, 0, 0, 6, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 6, 4, 2, 0, 0, 0, 0, 0, 4, 6, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 6, 4, 2, 1, 0, 0, 0, 1, 4, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 7, 5, 3, 2, 1, 0, 1, 2, 4, 5, 6, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 6, 5, 3, 3, 2, 2, 3, 4, 5, 6, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 6, 5, 4, 4, 4, 4, 5, 6, 6, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 6, 6, 5, 5, 5, 6, 6, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 6, 6, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8}
};

static void SetSoundLoc(fixed gx, fixed gy)
{
	fixed xt, yt;
	int x, y;

// translate point to view centered coordinates
//
	gx -= viewx;
	gy -= viewy;

//
// calculate newx
//
	xt = FixedByFrac(gx,viewcos);
	yt = FixedByFrac(gy,viewsin);
	x = (xt - yt) >> TILESHIFT;

//
// calculate newy
//
	xt = FixedByFrac(gx,viewsin);
	yt = FixedByFrac(gy,viewcos);
	y = (yt + xt) >> TILESHIFT;

	if (y >= ATABLEMAX)
		y = ATABLEMAX - 1;
	else if (y <= -ATABLEMAX)
		y = -ATABLEMAX;
	if (x < 0)
		x = -x;
	if (x >= ATABLEMAX)
		x = ATABLEMAX - 1;

	leftchannel  =  lefttable[x][y + ATABLEMAX];
	rightchannel = righttable[x][y + ATABLEMAX];
}

/*
==========================
=
= SetSoundLocGlobal - Sets up globalsoundx & globalsoundy and then calls
=	UpdateSoundLoc() to transform that into relative channel volumes. Those
=	values are then passed to the Sound Manager so that they'll be used for
=	the next sound played (if possible).
=
==========================
*/

void PlaySoundLocGlobal(word s, int id, fixed gx, fixed gy)
{
	SetSoundLoc(gx, gy);
	
	SPHack = true;
	if (SD_PlaySound(s)) {
		SoundPositioned = true;
		L = leftchannel;
		R = rightchannel;
		globalsoundx = gx;
		globalsoundy = gy;
	}
}

void UpdateSoundLoc(fixed x, fixed y, int angle)
{
	if (SoundPositioned)
	{
		SetSoundLoc(globalsoundx, globalsoundy);
		L = leftchannel;
		R = rightchannel;
	}
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_MusicOn() - turns on the sequencer
//
///////////////////////////////////////////////////////////////////////////
void SD_MusicOn()
{
	sqActive = true;
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_MusicOff() - turns off the sequencer and any playing notes
//
///////////////////////////////////////////////////////////////////////////
void SD_MusicOff()
{
	sqActive = false;
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_StartMusic() - starts playing the music pointed to
//
///////////////////////////////////////////////////////////////////////////
void SD_StartMusic(int music)
{
	music += STARTMUSIC;
	
	CA_CacheAudioChunk(music);
	
	SD_MusicOff();
	SD_MusicOn();
	Music = (MusicGroup *)audiosegs[music];
	NewMusic = 1;
}

void SD_SetDigiDevice(SDSMode mode)
{
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_SetSoundMode() - Sets which sound hardware to use for sound effects
//
///////////////////////////////////////////////////////////////////////////
boolean SD_SetSoundMode(SDMode mode)
{
	return false;
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_SetMusicMode() - sets the device to use for background music
//
///////////////////////////////////////////////////////////////////////////
boolean SD_SetMusicMode(SMMode mode)
{
	return false;
}
