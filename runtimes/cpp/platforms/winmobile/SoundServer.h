/*********************************************************************************

	SoundServer - Written by Arnaud Carré (aka Leonard / OXYGENE)
	Part of the "Leonard Homepage Articles".

	PART 1: Using WaveOut API.
	How to make a SoundServer under windows, to play various sound.
	WARNING: This sample plays a 44.1Khz, 16bits, mono sound. Should be quite easy to modify ! :-)

	Read the complete article on my web page:

	http://leonard.oxg.free.fr
	arnaud.carre@freesurf.fr

	WARNING: You have to link this with the Windows MultiMedia library. (WINMM.LIB)

*********************************************************************************/


#ifndef	__SOUNDSERVER__
#define	__SOUNDSERVER__

#include <windows.h>

#define	REPLAY_RATE				44100
#define	REPLAY_DEPTH			16
#define	REPLAY_SAMPLELEN		(REPLAY_DEPTH/8)
#define	REPLAY_NBSOUNDBUFFER	2

typedef void (*USER_CALLBACK) (void *pBuffer,long bufferLen);

class	CSoundServer
{

public:

		CSoundServer();
		~CSoundServer();

		BOOL	open(	int sampleRate,
						int bitDepth,
						int numChannels,
						USER_CALLBACK	pUserCallback,
						long totalBufferedSoundLen=16384);		// Buffered sound, in bytes.. (in ms before)

		void	close(void);

		
		// Do *NOT* call this internal function:
		void	fillNextBuffer(void);

private:
	
		BOOL	m_bServerRunning;
		HWND	m_hWnd;
		long	m_bufferSize;
		long	m_currentBuffer;
		HWAVEOUT		m_hWaveOut;
		WAVEHDR			m_waveHeader[REPLAY_NBSOUNDBUFFER];
		void			*m_pSoundBuffer[REPLAY_NBSOUNDBUFFER];
		USER_CALLBACK	m_pUserCallback;

		HANDLE hAudioThread;
		DWORD audioThreadId;

		CRITICAL_SECTION mCS;
};


#endif
