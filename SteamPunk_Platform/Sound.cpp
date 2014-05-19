
#include <Windows.h>
#include <MMSystem.h>
#include <DxErr.h>
#include "Sound.h"


directSound::directSound(LPDIRECTSOUNDBUFFER* soundBuff, DWORD buffSize, DWORD numBuffers, WaveFile* waveFile, DWORD creationFlags)
{

DWORD i;
/*
soundBuffer = new LPDIRECTSOUNDBUFFER[numBuffers];
if (NULL != soundBuffer)
{
for (i = 0; i < numBuffers; i++)
{
soundBuffer[i] = soundBuff[i];
}

bufferSize = buffSize;
this->numBuffers = numBuffers;
this->waveFile = waveFile;
this->creationFlags = creationFlags;

StoreSound(soundBuffer[0], false);
}
*/
}

directSound::directSound(LPTSTR fileName, LPDIRECTSOUND8 soundInterface)
{
	waveFile = NULL;

	soundBuffer = NULL;

	waveFile = new WaveFile();
	m_waveMap[waveFile] = 1;

	bool res = waveFile->Open(fileName, NULL, 0);
	
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwFlags = DSBCAPS_STATIC;
	dsbd.dwBufferBytes = waveFile->GetCkInfo().cksize;
	dsbd.lpwfxFormat = waveFile->GetFormat();

	LPDIRECTSOUNDBUFFER tempB = NULL;

	soundInterface->CreateSoundBuffer(&dsbd, &tempB, NULL);

	tempB->QueryInterface(IID_IDirectSoundBuffer8, (void**)&soundBuffer);

	tempB->Release();

	bufferSize = dsbd.dwBufferBytes;

	StoreSound(soundBuffer, false);

}

directSound::~directSound()
{

}



void directSound::Shutdown()
{
DWORD i;
/*
for (i = 0; i < this->numBuffers; i++)
{
if (this->soundBuffer[i])
{
this->soundBuffer[i]->Release();
this->soundBuffer[i] = 0;
}

if (this->soundBuffer)
{
delete[] this->soundBuffer;
this->soundBuffer = NULL;
}
if (this->waveFile)
{
delete this->waveFile;
this->waveFile = 0;
}
}
*/
int count = m_waveMap[waveFile];
if (count == 1)
{
	delete waveFile;
}
else
{
	m_waveMap[waveFile] = count - 1;
}
if (soundBuffer)
{
	soundBuffer->Release();
	delete soundBuffer;
	soundBuffer = 0;
}

}

bool directSound::StoreSound(LPDIRECTSOUNDBUFFER8 soundBuffer, bool repeatWav)
{

HRESULT result;
VOID* lockedBuffer = NULL;
DWORD lockedBufferSize = 0;
DWORD dwWavDataRead = 0;

if (soundBuffer == NULL)
return false;

result = RestoreBuffer(soundBuffer, NULL);
if (FAILED(result))
{
return false;
}

result = soundBuffer->Lock(0, this->bufferSize, &lockedBuffer, &lockedBufferSize, NULL, NULL, 0L);
if (FAILED(result))
{
return false;
}

//this->waveFile->ResetFile();
this->waveFile->Reset();

result = this->waveFile->Read((BYTE*)lockedBuffer, lockedBufferSize, &dwWavDataRead);
if (FAILED(result))
{
return false;
}

if (dwWavDataRead == 0)
{
FillMemory((BYTE*)lockedBuffer, lockedBufferSize, (BYTE)(this->waveFile->GetFormat()->wBitsPerSample == 8 ? 128 : 0));
}
else if (dwWavDataRead < lockedBufferSize)
{
if (repeatWav)
{
DWORD readSoFar = dwWavDataRead;

while (readSoFar < lockedBufferSize)
{

result = this->waveFile->Reset();

result = this->waveFile->Read((BYTE*)lockedBuffer + readSoFar, lockedBufferSize - readSoFar, &dwWavDataRead);
if (FAILED(result))
{
return false;
}

readSoFar += dwWavDataRead;
}

}
else
{
FillMemory((BYTE*)lockedBuffer, lockedBufferSize, (BYTE)(this->waveFile->GetFormat()->wBitsPerSample == 8 ? 128 : 0));
}
}

soundBuffer->Unlock(lockedBuffer, lockedBufferSize, NULL, 0);

return true;
}

bool directSound::RestoreBuffer(LPDIRECTSOUNDBUFFER soundBuffer, bool* wasRestored)
{
HRESULT result;

if (soundBuffer == NULL)
return false;
if (wasRestored)
*wasRestored = false;

DWORD status;
result = soundBuffer->GetStatus(&status);
if (FAILED(result))
{
return false;
}

if (status & DSBSTATUS_BUFFERLOST)
{
do
{
result = soundBuffer->Restore();
if (result == DSERR_BUFFERLOST)
Sleep(10);
} while ((result = soundBuffer->Restore()) == DSERR_BUFFERLOST);

if (wasRestored != NULL)
*wasRestored = true;

return true;
}
else
return false;
}

LPDIRECTSOUNDBUFFER8 directSound::GetBuffer()
{
if (this->soundBuffer == NULL)
return FALSE;

DWORD i;
//for (i = 0; i < numBuffers; i++)

if (this->soundBuffer)
{
	DWORD status = 0;
	this->soundBuffer->GetStatus(&status);
	if ((status & DSBSTATUS_PLAYING) == 0)
		return NULL;
	else return this->soundBuffer;
}

/*
if (i != numBuffers)
return this->soundBuffer[i];
else
return this->soundBuffer[rand() % numBuffers];
*/
}

bool directSound::Play(DWORD priority, DWORD dwFlags, LONG volume, LONG freq, LONG pan)
{

HRESULT result;

bool restored;

if (this->soundBuffer = NULL)
return false;

LPDIRECTSOUNDBUFFER8 dsb = GetBuffer();

if (!dsb)
return false;

if (!directSound::RestoreBuffer(dsb, &restored))
return false;

if (restored)
{
if (!StoreSound(dsb, FALSE))
return false;
}

if (creationFlags & DSBCAPS_CTRLVOLUME)
{
dsb->SetVolume(volume);
}

if (freq != -1 && (creationFlags & DSBCAPS_CTRLFREQUENCY))
{
dsb->SetFrequency(freq);
}

if (creationFlags & DSBCAPS_CTRLPAN)
{
dsb->SetPan(pan);
}

return dsb->Play(0, priority, dwFlags);
}

bool directSound::Stop()
{
if (this->soundBuffer == NULL)
return false;

HRESULT result = 0;

//for (DWORD i = 0; i < numBuffers; i++)
{
this->soundBuffer->Stop();
}

return true;
}

bool WaveFile::ReadMMIO() // HMMIO mmio, MMCKINFO* riff, WAVEFORMATEX** formatInfo
{
MMRESULT mResult;
MMCKINFO tempInfo;
PCMWAVEFORMAT tempformat;

waveFormat = NULL;

mResult = mmioDescend(wStruct.hmmio, &wStruct.ckRiff, NULL, 0);
if (mResult != 0)
{
return false;
}

if ((wStruct.ckRiff.ckid != FOURCC_RIFF) || (wStruct.ckRiff.fccType != mmioFOURCC('W', 'A', 'V', 'E')))
{
return false;
}

tempInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');

mResult = mmioDescend(wStruct.hmmio, &tempInfo, &wStruct.ckRiff, MMIO_FINDCHUNK);
if (mResult != 0)
{
return false;
}

if (tempInfo.cksize < (long)sizeof(PCMWAVEFORMAT))
{
return false;
}

if ((mmioRead(wStruct.hmmio, (HPSTR)&tempformat, sizeof(PCMWAVEFORMAT))) != sizeof(PCMWAVEFORMAT))
return false;

if (tempformat.wf.wFormatTag == WAVE_FORMAT_PCM)
{
waveFormat = new WAVEFORMATEX;
if (!waveFormat)
return false;

memcpy(waveFormat, &tempformat, sizeof(PCMWAVEFORMAT));

waveFormat->cbSize = 0;
}
else
{
WORD extraBytes = 0L;

if ((mmioRead(wStruct.hmmio, (CHAR*)&extraBytes, sizeof(WORD))) != sizeof(WORD))
return false;

waveFormat = (WAVEFORMATEX*)new char[sizeof(WAVEFORMATEX)+extraBytes];
if (!waveFormat)
return false;

memcpy(waveFormat, &tempformat, sizeof(PCMWAVEFORMAT));

waveFormat->cbSize = extraBytes;

LONG debug = mmioRead(wStruct.hmmio, (CHAR*)(((BYTE*)&((waveFormat)->cbSize)) + sizeof(WORD)),
extraBytes);
if (debug != extraBytes)
{
delete waveFormat;
waveFormat = NULL;
return false;
}

}

if (mmioAscend(wStruct.hmmio, &tempInfo, 0))
{
delete waveFormat;
waveFormat = NULL;
return false;
}
return true;
}

bool WaveFile::Open(LPTSTR fileName, WAVEFORMATEX* wFormat, DWORD flags)
{

HMMIO tempHmmio = NULL;

tempHmmio = mmioOpen(fileName, NULL, MMIO_ALLOCBUF | MMIO_READ);

if (tempHmmio == NULL)
return false;

bool result = ReadMMIO();// HMMIO mmio, MMCKINFO* riff, WAVEFORMATEX** formatInfo
if (result == false)
{
mmioClose(tempHmmio, 0);
return false;
}

wStruct.hmmio = tempHmmio;
return true;
}

bool WaveFile::StartReadWave()
{
LONG dBug = mmioSeek(wStruct.hmmio, wStruct.ckRiff.dwDataOffset + sizeof(FOURCC), SEEK_SET);
if (dBug == -1)
return false;

wStruct.ckInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
if ((mmioDescend(wStruct.hmmio, &wStruct.ckInfo, &wStruct.ckRiff, MMIO_FINDCHUNK)) != 0)
return false;

return true;
}

bool WaveFile::Read(BYTE* buffer, DWORD sizeToRead, DWORD* sizeRead)
{

MMIOINFO mmioInfo;
MMRESULT res;
*sizeRead = 0;

res = mmioGetInfo(wStruct.hmmio, &mmioInfo, 0);
if (res != 0)
return false;


DWORD data = sizeToRead;
if (data > wStruct.ckInfo.cksize)
data = wStruct.ckInfo.cksize;

wStruct.ckInfo.cksize -= data;

for (DWORD i = 0; i < data; i++)
{
if (mmioInfo.pchNext == mmioInfo.pchEndRead)
{
if (mmioAdvance(wStruct.hmmio, &mmioInfo, MMIO_READ) != 0)
return false;
if (mmioInfo.pchNext == mmioInfo.pchEndRead)
{
return false;
}
}

*((BYTE*)buffer + i) = *((BYTE*)mmioInfo.pchNext);
mmioInfo.pchNext++;
}

res = mmioSetInfo(wStruct.hmmio, &mmioInfo, 0);
if (res != 0)
return false;

*sizeRead = data;
}

WaveFile::WaveFile()
{
	waveFormat = NULL;
	ZeroMemory(&wStruct, sizeof(WaveStruct));
}

bool WaveFile::Close()
{
mmioClose(wStruct.hmmio, 0);
return true;
}

WaveFile::~WaveFile()
{
Close();
if (waveFormat)
{
delete waveFormat;
waveFormat = 0;
}
}

/*
Open( TCHAR* strFilename )
{

}
*/


bool WaveFile::Reset()
{
return StartReadWave();
}


