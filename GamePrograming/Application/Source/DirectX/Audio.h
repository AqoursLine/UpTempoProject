/******************************************************
* Audio.h		xaudio2�ݒ�
* ����ҁF�~���^�W���E�W
* �쐬���F2024/10/21
* �ŏI�X�V���F2024/10/22
*******************************************************/
#pragma once
#include "DirectX.h"

/****************************************************
* �萔�錾
*****************************************************/
constexpr int SOUND_DATA_MAX = 100;

/****************************************************
* �\���̐錾
*****************************************************/
//�T�E���h�f�[�^
struct WAVEDATA {
	WAVEFORMATEX waveFormat;			//�t�H�[�}�b�g���
	IXAudio2SourceVoice* sourceVoice = nullptr;	//�\�[�X�{�C�X
	char* soundBuffer = nullptr;		//�T�E���h�o�b�t�@
	DWORD size;							//�T�E���h�T�C�Y

	//�f�X�g���N�^
	~WAVEDATA() { 
		if (sourceVoice) {
			sourceVoice->Stop();
			sourceVoice->DestroyVoice();
		}
		if (soundBuffer) delete[] soundBuffer; 
	}
};

/****************************************************
* �I�[�f�B�I�N���X
*****************************************************/
class Audio {
public:
	//�I�[�f�B�I������
	bool Initialize();

	//�I�[�f�B�I�I��
	void Finalize();

	//�T�E���h���[�h
	int LoadWaveFile(const std::string& filePath);

	//�T�E���h�Đ�
	bool PlayAudio(int& dataIndex, int loopCount);

private:
	//xaudio�{��
	IXAudio2* m_xAudio2 = nullptr;
	//�}�X�^�[�{�C�X
	IXAudio2MasteringVoice* m_masteringVoice = nullptr;

	//�T�E���h�f�[�^�Ǘ�
	WAVEDATA* m_waveData[SOUND_DATA_MAX] = {};
	std::string m_filePaths[SOUND_DATA_MAX] ={};
	DWORD m_soundIndex = 0;

	//���̃N���X���A�ǂ�����ł��A�N�Z�X�ł���悤�ɃV���O���g���p�^�[����
	//�B��̃C���X�^���X�p�̃|�C���^
	static inline Audio* s_instance;
	Audio() {}
public:
	//�C���X�^���X����
	static void CreateInstance() {
		DeleteInstance();

		s_instance = new Audio();
	}
	//�C���X�^���X�폜
	static void DeleteInstance() {
		if (s_instance != nullptr) {
			delete s_instance;
			s_instance = nullptr;
		}
	}
	//�B��̃C���X�^���X���擾
	static Audio& GetInstance() {
		return *s_instance;
	}
};

#define AUDIO Audio::GetInstance()