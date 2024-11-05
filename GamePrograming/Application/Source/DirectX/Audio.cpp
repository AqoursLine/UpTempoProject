/******************************************************
* Audio.cpp		xaudio2�ݒ�
* ����ҁF�~���^�W���E�W
* �쐬���F2024/10/21
* �ŏI�X�V���F2024/10/22
*******************************************************/
#include "framework.h"
#include <locale.h>
#include "Audio.h"

#pragma comment(lib, "xaudio2.lib")

/****************************************************
* �I�[�f�B�I������
*****************************************************/
bool Audio::Initialize() {
	//COM������
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) {
		return false;
	}

	//XAudio2�̏�����
	if (FAILED(XAudio2Create(&m_xAudio2, 0))) {
		return false;
	}

	//�}�X�^�[�{�C�X�쐬
	if (FAILED(m_xAudio2->CreateMasteringVoice(&m_masteringVoice))) {
		return false;
	}

	//�f�[�^�Ǘ�������
	for (int i = 0; i < SOUND_DATA_MAX; i++) {
		m_waveData[i] = nullptr;
	}

	return true;
}

/****************************************************
* �I�[�f�B�I�I��
*****************************************************/
void Audio::Finalize() {
	for (int i = 0; i < m_soundIndex; i++) {
		if (m_waveData[i]) {
			delete m_waveData[i];
			m_waveData[i] = nullptr;
		}
	}
	if (m_masteringVoice)	m_masteringVoice->DestroyVoice();
	if (m_xAudio2)			m_xAudio2->Release();

	CoUninitialize();
}

/****************************************************
* �T�E���h���[�h
*****************************************************/
int Audio::LoadWaveFile(const std::string& filePath) {
	//�����̃t�@�C����Ǎ��ς݂̏ꍇ�����Y������Ԃ�
	for (int i = 0; i < SOUND_DATA_MAX; i++) {
		if (m_filePaths[i].compare(filePath) == 0) {
			return i;
		}
	}

	//�T�E���h�����ő�
	if (m_soundIndex == SOUND_DATA_MAX) {
		MessageBox(nullptr, L"�T�E���h�����ő�ł��I", L"�x��", MB_ICONWARNING);
		return -1;
	}

	WAVEDATA* wavedata = new WAVEDATA;

	if (wavedata->soundBuffer) {
		delete[] wavedata->soundBuffer;
		wavedata->soundBuffer = nullptr;
	}

	//mmio�n���h��
	HMMIO mmioHandle = nullptr;
	//�`�����N���
	MMCKINFO chunkInfo = {};
	//RIFF�`�����N
	MMCKINFO riffChunkInfo = {};

	//�}���`�o�C�g�����񂩂烏�C�h������֕ϊ�
	setlocale(LC_CTYPE, "jpn");
	wchar_t wFilePath[256];
	size_t ret;
	mbstowcs_s(&ret, wFilePath, filePath.c_str(), 256);

	//wav�t�@�C�����J��
	mmioHandle = mmioOpen(wFilePath, nullptr, MMIO_READ);
	if (!mmioHandle) {
		return -1;
	}

	//RIFF�`�����N�ɐN�����邽�߂�fccType�ݒ�
	riffChunkInfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	//RIFF�`�����N�ɐN������
	if (mmioDescend(mmioHandle, &riffChunkInfo, nullptr, MMIO_FINDRIFF) != MMSYSERR_NOERROR) {
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return -1;
	}

	//�N�����fmt�ɐݒ�
	chunkInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if (mmioDescend(mmioHandle,&chunkInfo, &riffChunkInfo, MMIO_FINDCHUNK) != MMSYSERR_NOERROR) {
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return -1;
	}

	//fmt�f�[�^�Ǎ�
	DWORD readSize = mmioRead(mmioHandle, (HPSTR)&wavedata->waveFormat, chunkInfo.cksize);
	if (readSize != chunkInfo.cksize) {
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return -1;
	}

	//�t�H�[�}�b�g�`�F�b�N
	if (wavedata->waveFormat.wFormatTag != WAVE_FORMAT_PCM) {
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return -1;
	}

	//fmt�`�����N��ޏo
	if (mmioAscend(mmioHandle, &chunkInfo, 0) != MMSYSERR_NOERROR) {
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return -1;
	}

	//data�`�����N�ɐN��
	chunkInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (mmioDescend(mmioHandle, &chunkInfo, &riffChunkInfo, MMIO_FINDCHUNK) != MMSYSERR_NOERROR) {
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return -1;
	}

	//�T�C�Y�ۑ�
	wavedata->size = chunkInfo.cksize;

	//data�`�����N�Ǎ�
	wavedata->soundBuffer = new char[chunkInfo.cksize];
	readSize = mmioRead(mmioHandle, (HPSTR)wavedata->soundBuffer, chunkInfo.cksize);
	if (readSize != chunkInfo.cksize) {
		mmioClose(mmioHandle, MMIO_FHOPEN);
		delete[] wavedata->soundBuffer;
		wavedata->soundBuffer = nullptr;
		return -1;
	}

	//wav�t�H�[�}�b�g�̐ݒ�
	WAVEFORMATEX format = {};
	memcpy(&format, &wavedata->waveFormat, sizeof(wavedata->waveFormat));
	format.wBitsPerSample = wavedata->waveFormat.nBlockAlign * 8 / wavedata->waveFormat.nChannels;	//1�T���v��������̃r�b�g��

	//�\�[�X�{�C�X�̍쐬
	if (FAILED(m_xAudio2->CreateSourceVoice(&wavedata->sourceVoice, &format))) {
		return -1;
	}

	mmioClose(mmioHandle, MMIO_FHOPEN);

	//�f�[�^�o�^
	m_waveData[m_soundIndex] = wavedata;
	m_filePaths[m_soundIndex] = filePath;

	//�Ԃ��l�ۊ�
	int tmpIndex = m_soundIndex;

	//�C���f�b�N�X����
	m_soundIndex++;

	return tmpIndex;
}

/****************************************************
* �T�E���h�Đ�
*****************************************************/
bool Audio::PlayAudio(int& dataIndex, int loopCount) {
	WAVEDATA* wavedata = m_waveData[dataIndex];
	XAUDIO2_VOICE_STATE state;

	//��Ԏ擾
	wavedata->sourceVoice->GetState(&state);
	if (state.BuffersQueued != 0){
		wavedata->sourceVoice->Stop();
		wavedata->sourceVoice->FlushSourceBuffers();
	}


	//�f�[�^���M
	XAUDIO2_BUFFER buffer = {};
	buffer.AudioBytes = wavedata->size;			//�o�b�t�@�̃o�C�g��
	buffer.pAudioData = (BYTE*)wavedata->soundBuffer;	//�o�b�t�@�̐擪�A�h���X
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	if (loopCount < 0) loopCount = XAUDIO2_LOOP_INFINITE;
	buffer.LoopCount = loopCount;
	wavedata->sourceVoice->SubmitSourceBuffer(&buffer);

	//�Đ�
	wavedata->sourceVoice->Start();

	return true;
}
