/*********************************************************************************

	SoundServer - Written by Arnaud Carré (aka Leonard / OXYGENE)
	Part of the "Leonard Homepage Articles".
	http://leonard.oxg.free.fr
	Read the complete article on my web page:

	PART 1: Using WaveOut API.
	How to make a SoundServer under windows, to play various sound.
	WARNING: This sample plays a 44.1Khz, 16bits, mono sound. Should be quite easy to modify ! :-)

	arnaud.carre@freesurf.fr

	WARNING: You have to link this with the Windows MultiMedia library. (WINMM.LIB)

*********************************************************************************/

#include <windows.h>
#include "SoundServer.h"

#include <ThreadPool.h>
#include <helpers/platforms/windows/CriticalSection.h>

/*
CRITICAL_SECTION gFreeBuffersCS;
int gFreeBuffers;

void setNumFreeBuffers(int i) {
	EnterCriticalSection(&gFreeBuffersCS);
	gFreeBuffers=i;
	LeaveCriticalSection(&gFreeBuffersCS);	
}

void incNumFreeBuffers(int i) {
	EnterCriticalSection(&gFreeBuffersCS);
	gFreeBuffers+=i;
	if(gFreeBuffers>REPLAY_NBSOUNDBUFFER) gFreeBuffers = REPLAY_NBSOUNDBUFFER;
	if(gFreeBuffers<0) gFreeBuffers = 0;
	LeaveCriticalSection(&gFreeBuffersCS);
}

void waitForFreeBuffers() {
	while(1) {
		EnterCriticalSection(&gFreeBuffersCS);
		if(gFreeBuffers>0) break;
		LeaveCriticalSection(&gFreeBuffersCS);
	}
	LeaveCriticalSection(&gFreeBuffersCS);
}
*/

// Internal WaveOut API callback function. We just call our sound handler ("playNextBuffer")
static	void CALLBACK waveOutProc(HWAVEOUT hwo,UINT uMsg,DWORD dwInstance,DWORD dwParam1,DWORD dwParam2)
{
		if (uMsg == WOM_DONE)
		{
			CSoundServer *pServer = (CSoundServer*)dwInstance;
			//audioSem->post();
			//incNumFreeBuffers(1);
			pServer->fillNextBuffer();

		}
}
/*
DWORD WINAPI audioThread(LPVOID lp) {
	CSoundServer *pServer = (CSoundServer*)lp;
	if(!pServer) return 1;

	//audioSem->wait();

	while(1) {
		//audioSem->wait();
		waitForFreeBuffers();
		pServer->fillNextBuffer();
	}

	return 0;
}
*/

CSoundServer::CSoundServer()
{
		m_pUserCallback = NULL;
		m_bServerRunning = FALSE;
		m_currentBuffer = 0;

		InitializeCriticalSection(&mCS);
}

CSoundServer::~CSoundServer()
{
		close();
}

BOOL	CSoundServer::open(int sampleRate, int bitDepth, int numChannels, USER_CALLBACK pUserCallback,long totalBufferedSoundLen)
{
/*
		m_pUserCallback = pUserCallback;
		m_bufferSize = ((totalBufferedSoundLen * REPLAY_RATE) / 1000) * REPLAY_SAMPLELEN;
		m_bufferSize /= REPLAY_NBSOUNDBUFFER;
*/
		/*
		WAVEFORMATEX	wfx;
		wfx.wFormatTag = 1;		// PCM standart.
		wfx.nChannels = 2;		// Mono
		wfx.nSamplesPerSec = REPLAY_RATE;
		wfx.nBlockAlign = REPLAY_SAMPLELEN*wfx.nChannels;
		wfx.nAvgBytesPerSec = wfx.nSamplesPerSec*wfx.nBlockAlign;
		wfx.wBitsPerSample = REPLAY_DEPTH;
		wfx.cbSize = 0;
		MMRESULT errCode = waveOutOpen(	&m_hWaveOut,
										WAVE_MAPPER,
										&wfx,
										(DWORD)waveOutProc,
										(DWORD)this,					// User data.
										(DWORD)CALLBACK_FUNCTION);
		*/

		m_pUserCallback = pUserCallback;
		m_bufferSize = totalBufferedSoundLen; //((totalBufferedSoundLen * sampleRate) / 1000) * (bitDepth/8);
		//m_bufferSize /= REPLAY_NBSOUNDBUFFER;
		//gFreeBuffers = REPLAY_NBSOUNDBUFFER;

		WAVEFORMATEX	wfx;
		wfx.wFormatTag = WAVE_FORMAT_PCM;		// PCM standart.
		wfx.nChannels = numChannels;
		wfx.nSamplesPerSec = sampleRate;
		wfx.nBlockAlign = (bitDepth/8)*wfx.nChannels;
		wfx.nAvgBytesPerSec = wfx.nSamplesPerSec*wfx.nBlockAlign;
		wfx.wBitsPerSample = bitDepth;
		wfx.cbSize = sizeof(WAVEFORMATEX);


		//audioSem = new MoSyncSemaphore();
		//InitializeCriticalSection(&gFreeBuffersCS);

		UINT numDevs = waveOutGetNumDevs();
		UINT id;
		for (id = 0; id < numDevs; id++) { 
		   if (waveOutOpen(&m_hWaveOut, id, &wfx, (DWORD)waveOutProc, (DWORD)this, (DWORD)CALLBACK_FUNCTION) == MMSYSERR_NOERROR) {
			  break;
		   }
		} 
		if(id == numDevs) return FALSE;

		// Alloc the sample buffers.
		for (int i=0;i<REPLAY_NBSOUNDBUFFER;i++)
		{
			m_pSoundBuffer[i] = malloc(m_bufferSize);
			memset(&m_waveHeader[i],0,sizeof(WAVEHDR));
		}

		// Fill all the sound buffers
		m_currentBuffer = 0;
		m_bServerRunning = TRUE;
		for (int i=0;i<REPLAY_NBSOUNDBUFFER;i++)
		{
			fillNextBuffer();
		}

		/*
		hAudioThread = CreateThread (NULL, 0, audioThread, (LPVOID)this, 0, &audioThreadId);
		SetThreadPriority (hAudioThread, THREAD_PRIORITY_HIGHEST);
		*/

		return TRUE;
}

void	CSoundServer::close(void)
{
	CriticalSectionHandler cs(&mCS);

		if (m_bServerRunning)
		{
			//CloseHandle(hAudioThread);

			// this seems to impose deadlock.
			//waveOutReset(m_hWaveOut);					// Reset tout.
			for (int i=0;i<REPLAY_NBSOUNDBUFFER;i++)
			{
				if (m_waveHeader[m_currentBuffer].dwFlags & WHDR_PREPARED)
					waveOutUnprepareHeader(m_hWaveOut,&m_waveHeader[i],sizeof(WAVEHDR));

				free(m_pSoundBuffer[i]);
			}
			waveOutClose(m_hWaveOut);
			m_bServerRunning = FALSE;

			m_pUserCallback = NULL;
			m_currentBuffer = 0;
			m_hWaveOut = 0;

			//delete audioSem;
			//audioSem = NULL;
		}
}

void	CSoundServer::fillNextBuffer(void)
{
	CriticalSectionHandler cs(&mCS);

		// check if the buffer is already prepared (should not !)
		if (m_waveHeader[m_currentBuffer].dwFlags&WHDR_PREPARED)
			waveOutUnprepareHeader(m_hWaveOut,&m_waveHeader[m_currentBuffer],sizeof(WAVEHDR));

		// Call the user function to fill the buffer with anything you want ! :-)
		if (m_pUserCallback)
			m_pUserCallback(m_pSoundBuffer[m_currentBuffer],m_bufferSize);

		// Prepare the buffer to be sent to the WaveOut API
		m_waveHeader[m_currentBuffer].lpData = (char*)m_pSoundBuffer[m_currentBuffer];
		m_waveHeader[m_currentBuffer].dwBufferLength = m_bufferSize;
		waveOutPrepareHeader(m_hWaveOut,&m_waveHeader[m_currentBuffer],sizeof(WAVEHDR));

		// Send the buffer the the WaveOut queue
		waveOutWrite(m_hWaveOut,&m_waveHeader[m_currentBuffer],sizeof(WAVEHDR));

		m_currentBuffer++;
		if (m_currentBuffer >= REPLAY_NBSOUNDBUFFER) m_currentBuffer = 0;
}
