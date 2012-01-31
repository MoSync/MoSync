#ifndef	__SD_COMM_H__
#define	__SD_COMM_H__

typedef	enum	{
					sdm_Off,
					sdm_PC,sdm_AdLib
				}	SDMode;
typedef	enum	{
					smm_Off,smm_AdLib
				}	SMMode;
typedef	enum	{
					sds_Off,sds_PC,sds_SoundBlaster
				}	SDSMode;

extern boolean AdLibPresent, SoundBlasterPresent;

extern	SDMode		SoundMode;
extern	SDSMode		DigiMode;
extern	SMMode		MusicMode;

extern void SD_Startup();
extern void SD_Shutdown();

extern boolean SD_PlaySound(soundnames sound);
extern void SD_StopSound(),
				SD_WaitSoundDone(),
				SD_StartMusic(int music),
				SD_MusicOn(),
				SD_MusicOff();

extern boolean SD_SetSoundMode(SDMode mode), SD_SetMusicMode(SMMode mode);
		
extern word SD_SoundPlaying();

extern void SD_SetDigiDevice(SDSMode);

void PlaySoundLocGlobal(word s, int id, fixed gx, fixed gy);
void UpdateSoundLoc(fixed x, fixed y, int angle);


extern int DigiMap[];
void InitDigiMap();


void AudioBufferFill();

#endif
