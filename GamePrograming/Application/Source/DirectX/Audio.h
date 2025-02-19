/******************************************************
* Audio.h		xaudio2設定
* 制作者：ミヤタジョウジ
* 作成日：2024/10/21
* 最終更新日：2024/10/22
*******************************************************/
#pragma once
#include "DirectX.h"

/****************************************************
* 定数宣言
*****************************************************/
constexpr int SOUND_DATA_MAX = 100;

/****************************************************
* 構造体宣言
*****************************************************/
//サウンドデータ
struct WAVEDATA {
	WAVEFORMATEX waveFormat;			//フォーマット情報
	IXAudio2SourceVoice* sourceVoice = nullptr;	//ソースボイス
	char* soundBuffer = nullptr;		//サウンドバッファ
	DWORD size;							//サウンドサイズ

	//デストラクタ
	~WAVEDATA() { 
		if (sourceVoice) {
			sourceVoice->Stop();
			sourceVoice->DestroyVoice();
		}
		if (soundBuffer) delete[] soundBuffer; 
	}
};

/****************************************************
* オーディオクラス
*****************************************************/
class Audio {
public:
	//オーディオ初期化
	bool Initialize();

	//オーディオ終了
	void Finalize();

	//サウンドロード
	int LoadWaveFile(const std::string& filePath);

	//サウンド再生
	bool PlayAudio(int& dataIndex, int loopCount);

private:
	//xaudio本体
	IXAudio2* m_xAudio2 = nullptr;
	//マスターボイス
	IXAudio2MasteringVoice* m_masteringVoice = nullptr;

	//サウンドデータ管理
	WAVEDATA* m_waveData[SOUND_DATA_MAX] = {};
	std::string m_filePaths[SOUND_DATA_MAX] ={};
	DWORD m_soundIndex = 0;

	//このクラスも、どこからでもアクセスできるようにシングルトンパターン化
	//唯一のインスタンス用のポインタ
	static inline Audio* s_instance;
	Audio() {}
public:
	//インスタンス生成
	static void CreateInstance() {
		DeleteInstance();

		s_instance = new Audio();
	}
	//インスタンス削除
	static void DeleteInstance() {
		if (s_instance != nullptr) {
			delete s_instance;
			s_instance = nullptr;
		}
	}
	//唯一のインスタンスを取得
	static Audio& GetInstance() {
		return *s_instance;
	}
};

#define AUDIO Audio::GetInstance()