///////////////////////////////////////////////////////////////////////////////
// Filename: soundclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "Sound.h"

SoundClass::SoundClass()
{
	m_DirectSound = 0;
	m_primaryBuffer = 0;
}

SoundClass::SoundClass(const SoundClass& other)
{
}

SoundClass::~SoundClass()
{
}

WaveFile::WaveFile()
{

}


WaveFile::WaveFile(const WaveFile& other)
{

}

WaveFile::~WaveFile()
{
	if (soundBuff)
	{
		soundBuff->Release();
		delete soundBuff;
	}
}

bool SoundClass::Initialize(HWND hwnd)
{
	bool result;


	m_DirectSound = NULL;
	m_primaryBuffer = NULL;

	HRESULT hr;

	hr = DirectSoundCreate8(NULL, &m_DirectSound, NULL);
	if (FAILED(hr))
	{
		return false;
	}

	hr = m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if (FAILED(hr))
	{
		return false;
	}

	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(dsbd));

	dsbd.dwSize = sizeof(dsbd);
	dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	dsbd.dwBufferBytes = 0;
	dsbd.dwReserved = 0;
	dsbd.lpwfxFormat = NULL;
	dsbd.guid3DAlgorithm = GUID_NULL;


	hr = m_DirectSound->CreateSoundBuffer(&dsbd, &m_primaryBuffer, NULL);
	if (FAILED(hr))
	{
		return false;
	}

	WAVEFORMATEX wfx;
	ZeroMemory(&wfx, sizeof(WAVEFORMATEX));
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = 2;
	wfx.nSamplesPerSec = 44100;
	wfx.wBitsPerSample = 16;
	wfx.nBlockAlign = wfx.wBitsPerSample / 8 * wfx.nChannels;
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

	if (FAILED(hr = m_primaryBuffer->SetFormat(&wfx)))
	{
		return false;
	}
	return true;
}

void SoundClass::Shutdown()
{
	waveList.clear();
	if (m_primaryBuffer)
	{
		m_primaryBuffer->Release();
		m_primaryBuffer = 0;
	}

	if (m_DirectSound)
	{
		m_DirectSound->Release();
		m_DirectSound = 0;
	}
}

void SoundClass::AddSound(TCHAR* filename)
{
	WaveFile* tempWave = new WaveFile();

	if (!tempWave->LoadWaveFile(filename, m_DirectSound))
		return;

	waveList.push_back(tempWave);
}

bool WaveFile::LoadWaveFile(TCHAR* filename, IDirectSound8* directSound)
{
	HRESULT hr;
	HMMIO   hmmioIn = NULL;
	MMCKINFO        ckIn;
	PCMWAVEFORMAT   pcmWaveFormat;

	m_pwfx = NULL;

	if (NULL == (hmmioIn = mmioOpen(filename, NULL, MMIO_ALLOCBUF | MMIO_READ)))
		return false;

	if ((0 != mmioDescend(hmmioIn, &m_ckInRiff, NULL, 0)))
		return false;

	if (((&m_ckInRiff)->ckid != FOURCC_RIFF) ||
		((&m_ckInRiff)->fccType != mmioFOURCC('W', 'A', 'V', 'E')))
		return false;

	ckIn.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if (0 != mmioDescend(hmmioIn, &ckIn, &m_ckInRiff, MMIO_FINDCHUNK))
		return false;

	if (ckIn.cksize < (LONG) sizeof(PCMWAVEFORMAT))
		return false;

	if (mmioRead(hmmioIn, (HPSTR)&pcmWaveFormat,
		sizeof(pcmWaveFormat)) != sizeof(pcmWaveFormat))
		return false;

	if (pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM)
	{
		if (NULL == (m_pwfx = new WAVEFORMATEX))
			return false;

		memcpy(m_pwfx, &pcmWaveFormat, sizeof(pcmWaveFormat));
		(m_pwfx)->cbSize = 0;
	}
	else
	{

		WORD cbExtraBytes = 0L;
		if (mmioRead(hmmioIn, (CHAR*)&cbExtraBytes, sizeof(WORD)) != sizeof(WORD))
			return false;

		m_pwfx = (WAVEFORMATEX*)new CHAR[sizeof(WAVEFORMATEX)+cbExtraBytes];
		if (NULL == m_pwfx)
			return false;


		memcpy(m_pwfx, &pcmWaveFormat, sizeof(pcmWaveFormat));
		(m_pwfx)->cbSize = cbExtraBytes;


		if (mmioRead(hmmioIn, (CHAR*)(((BYTE*)&((m_pwfx)->cbSize)) + sizeof(WORD)),
			cbExtraBytes) != cbExtraBytes)
		{
			delete m_pwfx;
			m_pwfx = NULL;
			return false;
		}
	}

	if (0 != mmioAscend(hmmioIn, &ckIn, 0))
	{
		delete m_pwfx;
		m_pwfx = NULL;
		return false;
	}

	m_hmmioIn = hmmioIn;

	StartRead();
	WAVEFORMATEX waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(dsbd));

	dsbd.dwSize = sizeof(dsbd);
	dsbd.dwFlags = DSBCAPS_CTRLVOLUME;
	dsbd.dwBufferBytes = m_ckIn.cksize;
	dsbd.dwReserved = 0;
	dsbd.lpwfxFormat = &waveFormat;
	dsbd.guid3DAlgorithm = GUID_NULL;

	LPDIRECTSOUNDBUFFER pTempBuffer = 0;

	hr = directSound->CreateSoundBuffer(&dsbd, &pTempBuffer, NULL);
	if (FAILED(hr))
	{
		return false;
	}

	pTempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&soundBuff);
	if (FAILED(hr))
	{

		return false;
	}

	pTempBuffer->Release();

	m_bufferSize = dsbd.dwBufferBytes;

	BYTE* pbWavData;
	UINT cbWavSize;
	void *pbData = NULL;
	void *pbData2 = NULL;
	ULONG dwLength;
	ULONG dwLength2;

	UINT nWaveFileSize = m_ckIn.cksize;

	pbWavData = new BYTE[nWaveFileSize];
	if (NULL == pbWavData)
	{

		delete[] pbWavData;
		return false;
	}

	hr = ReadFile(nWaveFileSize, pbWavData, &m_ckIn, &cbWavSize);
	if (FAILED(hr))
	{

		delete[] pbWavData;
		return false;
	}

	StartRead();

	hr = soundBuff->Lock(0, m_bufferSize, &pbData, &dwLength, &pbData2, &dwLength2, 0L);
	if (FAILED(hr))
	{

		delete[] pbWavData;
		return false;
	}

	memcpy(pbData, pbWavData, m_bufferSize);
	soundBuff->Unlock(pbData, m_bufferSize, NULL, 0);

	delete[] pbWavData;

	return true;
}

bool SoundClass::IsPlaying(int index)
{
	if (waveList.size() < index)
		return false;

	return waveList[index]->IsPlaying();
}

bool WaveFile::IsPlaying()
{
	DWORD dwStatus = 0;

	soundBuff->GetStatus(&dwStatus);

	if (dwStatus & DSBSTATUS_PLAYING)
		return true;
	else
		return false;
}
bool WaveFile::Fill()
{
	HRESULT hr;
	BYTE* pbWavData;
	UINT cbWavSize;
	void *pbData = NULL;
	void *pbData2 = NULL;
	ULONG dwLength;
	ULONG dwLength2;


	UINT nWaveFileSize = m_ckIn.cksize;



	pbWavData = new BYTE[nWaveFileSize];
	if (NULL == pbWavData)
	{

		delete[] pbWavData;
		return false;
	}

	hr = ReadFile(nWaveFileSize, pbWavData, &m_ckIn, &cbWavSize);
	if (FAILED(hr))
	{

		delete[] pbWavData;
		return false;
	}

	StartRead();

	hr = soundBuff->Lock(0, m_bufferSize, &pbData, &dwLength, &pbData2, &dwLength2, 0L);
	if (FAILED(hr))
	{

		delete[] pbWavData;
		return false;
	}

	memcpy(pbData, pbWavData, m_bufferSize);
	soundBuff->Unlock(pbData, m_bufferSize, NULL, 0);

	delete[] pbWavData;
}
bool WaveFile::ReadFile(UINT cbRead, BYTE* pbDest,
	MMCKINFO* pckIn, UINT* cbActualRead)
{
	MMIOINFO mmioinfoIn;

	*cbActualRead = 0;

	if (0 != mmioGetInfo(m_hmmioIn, &mmioinfoIn, 0))
		return false;

	UINT cbDataIn = cbRead;
	if (cbDataIn > pckIn->cksize)
		cbDataIn = pckIn->cksize;

	pckIn->cksize -= cbDataIn;

	for (DWORD cT = 0; cT < cbDataIn; cT++)
	{

		if (mmioinfoIn.pchNext == mmioinfoIn.pchEndRead)
		{
			if (0 != mmioAdvance(m_hmmioIn, &mmioinfoIn, MMIO_READ))
				return false;

			if (mmioinfoIn.pchNext == mmioinfoIn.pchEndRead)
				return false;
		}

		*((BYTE*)pbDest + cT) = *((BYTE*)mmioinfoIn.pchNext);
		mmioinfoIn.pchNext++;
	}

	if (0 != mmioSetInfo(m_hmmioIn, &mmioinfoIn, 0))
		return false;

	*cbActualRead = cbDataIn;
	return true;
}

bool WaveFile::StartRead()
{
	if (-1 == mmioSeek(m_hmmioIn, (&m_ckInRiff)->dwDataOffset + sizeof(FOURCC),
		SEEK_SET))
		return false;

	(&m_ckIn)->ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (0 != mmioDescend(m_hmmioIn, &m_ckIn, &m_ckInRiff, MMIO_FINDCHUNK))
		return false;

	return false;
}

bool SoundClass::PlayWaveFile(bool loop, int index)
{
	HRESULT result;

	HRESULT hr;
	if (index > waveList.size())
		return false;
	if (NULL == waveList[index]->GetBuffer())
		return false;

	Restore(index);

	DWORD dwLooped = loop ? DSBPLAY_LOOPING : 0L;
	if (FAILED(hr = waveList[index]->GetBuffer()->Play(0, 0, dwLooped)))
	{
		return false;
	}
	return true;
}

void SoundClass::Stop(int index)
{
	if (index >= waveList.size())
		return;

	waveList[index]->GetBuffer()->Stop();
	waveList[index]->GetBuffer()->SetCurrentPosition(0);
}

bool SoundClass::Restore(int index)
{
	HRESULT hr;

	if (NULL == waveList[index]->GetBuffer())
	{
		return false;
	}

	DWORD dwStatus;
	if (FAILED(hr = waveList[index]->GetBuffer()->GetStatus(&dwStatus)))
	{
		return false;
	}

	if (dwStatus & DSBSTATUS_BUFFERLOST)
	{
		do
		{

			hr = waveList[index]->GetBuffer()->Restore();
			if (hr == DSERR_BUFFERLOST)
				Sleep(10);
		} while (hr = waveList[index]->GetBuffer()->Restore());

		waveList[index]->Fill();
	}

	return true;
}