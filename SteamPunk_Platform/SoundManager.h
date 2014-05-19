////////////////////////////////////////////////////////////////////////////////
// SoundManager.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _SOUNDMANAGER_H
#define _SOUNDMANAGER_H
#include "Sound.h"
class soundManager
{


public:
	soundManager();
	~soundManager();

	bool Initialize(HWND hwnd, DWORD coopLvl);
	inline LPDIRECTSOUND8 GetDirectSound()
	{
		return Sound;
	}
	bool SetPrimaryBufferFormat(DWORD primaryChannels, DWORD primaryFreq, DWORD primaryBitRate);

	bool Create(LPTSTR waveFilename, DWORD creationFlags = 0, GUID guid = GUID_NULL);
	void ShutDown();
	void Play();
	void Stop();
	directSound* soundType;

private:
	LPDIRECTSOUND8 Sound;

	//LPDIRECTSOUNDBUFFER8 Primary;

};
#endif