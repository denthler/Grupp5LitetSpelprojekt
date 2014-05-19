#include <Windows.h>
#include <MMSystem.h>
#include <DxErr.h>
#include "SoundManager.h"

soundManager::soundManager()
{
	Sound = 0;
	//Primary = 0;
	soundType = 0;
}

soundManager::~soundManager()
{

}

void soundManager::ShutDown()
{
	if (Sound)
	{
		Sound->Release();
		Sound = 0;
	}

	if (soundType)
	{
		soundType->Shutdown();
		delete soundType;
		soundType = 0;
	}
	/*
	if(Primary)
	{
	Primary->Release();
	Primary = 0;
	}
	*/

}

bool soundManager::Initialize(HWND hwnd, DWORD coopLvl)
{
	HRESULT result;

	soundManager::ShutDown();

	result = DirectSoundCreate8(NULL, &Sound, NULL);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Error creating DirectSound8",
			TEXT("ERROR"), MB_OK);
		return false;
	}

	//result = Sound->SetCooperativeLevel(hwnd, coopLvl);
	result = Sound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Error DirectSound CoopLevel",
			TEXT("ERROR"), MB_OK);
		return false;
	}

	return true;
}


bool soundManager::SetPrimaryBufferFormat(DWORD primaryChannels, DWORD primaryFreq, DWORD primaryBitRate)
{

	HRESULT result;
	LPDIRECTSOUNDBUFFER primary = 0;

	if (Sound == NULL)
		return false;

	DSBUFFERDESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(DSBUFFERDESC));
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.lpwfxFormat = NULL;

	result = Sound->CreateSoundBuffer(&bufferDesc, &primary, NULL);
	if (FAILED(result))
	{
		return false;
	}

	WAVEFORMATEX wfx;
	ZeroMemory(&wfx, sizeof(PWAVEFORMATEX));
	wfx.wFormatTag = (WORD)WAVE_FORMAT_PCM;
	wfx.nChannels = (WORD)primaryChannels;
	wfx.nSamplesPerSec = (DWORD)primaryFreq;
	wfx.wBitsPerSample = (WORD)primaryBitRate;
	wfx.nBlockAlign = (WORD)((primaryBitRate / 8) * primaryChannels);
	wfx.nAvgBytesPerSec = (DWORD)(primaryFreq * wfx.nBlockAlign);

	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = 2;
	wfx.nSamplesPerSec = 22050;
	wfx.wBitsPerSample = 16;
	wfx.nBlockAlign = wfx.wBitsPerSample / 8 * wfx.nChannels;
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;


	result = primary->SetFormat(&wfx);
	if (FAILED(result))
	{
		return false;
	}

	if (primary)
	{
		primary->Release();
		primary = 0;
	}

	return true;
}

void soundManager::Play()
{
	soundType->Play();
}

void soundManager::Stop()
{
	soundType->Stop();
}

bool soundManager::Create(LPTSTR waveFilename, DWORD creationFlags, GUID guid)
{

	HRESULT result;
	DWORD i;
	//LPDIRECTSOUNDBUFFER8 dBuffer = 0;
	DWORD buffSize = 0;
	WaveFile* waveFile = 0;

	if (!Sound)
		return false;

	if (waveFilename == NULL)
		return false;

	soundType = new directSound(waveFilename, Sound);

	
	return true;
}