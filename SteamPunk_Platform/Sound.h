////////////////////////////////////////////////////////////////////////////////
// Sound.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _SOUND_H
#define _SOUND_H

#include <map>
#include <dsound.h>

class WaveFile
{

public:
WaveFile();
~WaveFile();

WAVEFORMATEX* waveFormat;

struct WaveStruct
{
HMMIO hmmio;
MMCKINFO ckInfo;
MMCKINFO ckRiff;
DWORD size;
MMIOINFO mmIoInfoOut;
DWORD flags;
bool readingFromMemory;
BYTE* data;
BYTE* dataCurrent;
ULONG dataSize;
CHAR* resourceBuffer;
};

bool Open(LPTSTR fileName, WAVEFORMATEX* wFormat, DWORD flags);
bool Close();

//bool Read(HMMIO mmio, MMCKINFO* riff, WAVEFORMATEX** formatInfo);
bool Read(BYTE* buffer, DWORD sizeToRead, DWORD* sizeRead);
//bool Write(UINT sizeToWrite, BYTE* data, UINT* sizeWrote);

DWORD GetSize()
{
	return wStruct.size;
}
MMCKINFO GetCkInfo()
{
	return wStruct.ckInfo;
}
//bool ResetFile();
bool Reset();
WAVEFORMATEX* GetFormat()
{
return waveFormat;
}
private:
WaveStruct wStruct;
bool ReadMMIO();
bool StartReadWave();
//bool WriteMMIO(WAVEFORMATEX* wFormatDest);
};

class directSound
{

public:
directSound(LPDIRECTSOUNDBUFFER* soundBuffer, DWORD bufferSize, DWORD numBuffers, WaveFile* waveFile, DWORD creationFlags);
directSound(LPTSTR fileName, LPDIRECTSOUND8 soundInterface);
~directSound();
//directSound(TCHAR *filename);
bool StoreSound(LPDIRECTSOUNDBUFFER8 soundBuffer, bool repeatWav);
LPDIRECTSOUNDBUFFER8 GetBuffer();
void Shutdown();
//	void Init();

bool Play(DWORD priority = 0, DWORD dwFlags = 0, LONG volume = 0, LONG freq = -1, LONG pan = 0);
bool Stop();
//bool Reset();
bool IsPlaying();

private:
LPDIRECTSOUNDBUFFER8 soundBuffer;
DWORD bufferSize;
WaveFile* waveFile;
DWORD numBuffers;
DWORD creationFlags;
std::map<WaveFile*, int> m_waveMap;

bool RestoreBuffer(LPDIRECTSOUNDBUFFER soundBuffer, bool* wasRestored);


};


#endif
