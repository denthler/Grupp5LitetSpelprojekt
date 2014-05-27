///////////////////////////////////////////////////////////////////////////////
// Filename: soundclass.h
///////////////////////////////////////////////////////////////////////////////
#ifndef _SOUNDCLASS_H_
#define _SOUNDCLASS_H_

/////////////
// LINKING //
/////////////
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

//////////////
// INCLUDES //
//////////////
#include <Windows.h>
#include <MMSystem.h>
#include <dsound.h>
#include <stdio.h>
#include <vector>
///////////////////////////////////////////////////////////////////////////////
// Class name: SoundClass
///////////////////////////////////////////////////////////////////////////////
class WaveFile
{
public:
	WaveFile();
	WaveFile(const WaveFile&);
	~WaveFile();

private:
	MMCKINFO m_ckInRiff;
	WAVEFORMATEX* m_pwfx;
	HMMIO         m_hmmioIn;
	MMCKINFO      m_ckIn;

	int m_bufferSize;
	IDirectSoundBuffer8* soundBuff;
public:
	IDirectSoundBuffer8* GetBuffer(){ return soundBuff; }
	bool LoadWaveFile(TCHAR*, IDirectSound8*);
	bool StartRead();
	bool ReadFile(UINT, BYTE*, MMCKINFO*, UINT*);
	//void ShutdownWaveFile();
	bool Fill();
	bool IsPlaying();
};

class SoundClass
{
private:

public:
	SoundClass();
	SoundClass(const SoundClass&);
	~SoundClass();

	bool Initialize(HWND);
	void Shutdown();
	void AddSound(TCHAR*);
	bool PlayWaveFile(bool, int);
	void Stop(int);
	bool IsPlaying(int);
private:
	bool Restore(int);
private:
	IDirectSound8* m_DirectSound;
	IDirectSoundBuffer* m_primaryBuffer;

	//IDirectSoundBuffer8* m_secondaryBuffer1;

	std::vector<WaveFile*> waveList;
};
#endif
