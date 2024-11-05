/******************************************************
* Audio.cpp		xaudio2設定
* 制作者：ミヤタジョウジ
* 作成日：2024/10/21
* 最終更新日：2024/10/22
*******************************************************/
#include "framework.h"
#include <locale.h>
#include "Audio.h"

#pragma comment(lib, "xaudio2.lib")

/****************************************************
* オーディオ初期化
*****************************************************/
bool Audio::Initialize() {
	//COM初期化
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) {
		return false;
	}

	//XAudio2の初期化
	if (FAILED(XAudio2Create(&m_xAudio2, 0))) {
		return false;
	}

	//マスターボイス作成
	if (FAILED(m_xAudio2->CreateMasteringVoice(&m_masteringVoice))) {
		return false;
	}

	//データ管理初期化
	for (int i = 0; i < SOUND_DATA_MAX; i++) {
		m_waveData[i] = nullptr;
	}

	return true;
}

/****************************************************
* オーディオ終了
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
* サウンドロード
*****************************************************/
int Audio::LoadWaveFile(const std::string& filePath) {
	//同名のファイルを読込済みの場合同じ添え字を返す
	for (int i = 0; i < SOUND_DATA_MAX; i++) {
		if (m_filePaths[i].compare(filePath) == 0) {
			return i;
		}
	}

	//サウンド数が最大
	if (m_soundIndex == SOUND_DATA_MAX) {
		MessageBox(nullptr, L"サウンド数が最大です！", L"警告", MB_ICONWARNING);
		return -1;
	}

	WAVEDATA* wavedata = new WAVEDATA;

	if (wavedata->soundBuffer) {
		delete[] wavedata->soundBuffer;
		wavedata->soundBuffer = nullptr;
	}

	//mmioハンドル
	HMMIO mmioHandle = nullptr;
	//チャンク情報
	MMCKINFO chunkInfo = {};
	//RIFFチャンク
	MMCKINFO riffChunkInfo = {};

	//マルチバイト文字列からワイド文字列へ変換
	setlocale(LC_CTYPE, "jpn");
	wchar_t wFilePath[256];
	size_t ret;
	mbstowcs_s(&ret, wFilePath, filePath.c_str(), 256);

	//wavファイルを開く
	mmioHandle = mmioOpen(wFilePath, nullptr, MMIO_READ);
	if (!mmioHandle) {
		return -1;
	}

	//RIFFチャンクに侵入するためのfccType設定
	riffChunkInfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	//RIFFチャンクに侵入する
	if (mmioDescend(mmioHandle, &riffChunkInfo, nullptr, MMIO_FINDRIFF) != MMSYSERR_NOERROR) {
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return -1;
	}

	//侵入先をfmtに設定
	chunkInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if (mmioDescend(mmioHandle,&chunkInfo, &riffChunkInfo, MMIO_FINDCHUNK) != MMSYSERR_NOERROR) {
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return -1;
	}

	//fmtデータ読込
	DWORD readSize = mmioRead(mmioHandle, (HPSTR)&wavedata->waveFormat, chunkInfo.cksize);
	if (readSize != chunkInfo.cksize) {
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return -1;
	}

	//フォーマットチェック
	if (wavedata->waveFormat.wFormatTag != WAVE_FORMAT_PCM) {
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return -1;
	}

	//fmtチャンクを退出
	if (mmioAscend(mmioHandle, &chunkInfo, 0) != MMSYSERR_NOERROR) {
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return -1;
	}

	//dataチャンクに侵入
	chunkInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (mmioDescend(mmioHandle, &chunkInfo, &riffChunkInfo, MMIO_FINDCHUNK) != MMSYSERR_NOERROR) {
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return -1;
	}

	//サイズ保存
	wavedata->size = chunkInfo.cksize;

	//dataチャンク読込
	wavedata->soundBuffer = new char[chunkInfo.cksize];
	readSize = mmioRead(mmioHandle, (HPSTR)wavedata->soundBuffer, chunkInfo.cksize);
	if (readSize != chunkInfo.cksize) {
		mmioClose(mmioHandle, MMIO_FHOPEN);
		delete[] wavedata->soundBuffer;
		wavedata->soundBuffer = nullptr;
		return -1;
	}

	//wavフォーマットの設定
	WAVEFORMATEX format = {};
	memcpy(&format, &wavedata->waveFormat, sizeof(wavedata->waveFormat));
	format.wBitsPerSample = wavedata->waveFormat.nBlockAlign * 8 / wavedata->waveFormat.nChannels;	//1サンプルあたりのビット数

	//ソースボイスの作成
	if (FAILED(m_xAudio2->CreateSourceVoice(&wavedata->sourceVoice, &format))) {
		return -1;
	}

	mmioClose(mmioHandle, MMIO_FHOPEN);

	//データ登録
	m_waveData[m_soundIndex] = wavedata;
	m_filePaths[m_soundIndex] = filePath;

	//返す値保管
	int tmpIndex = m_soundIndex;

	//インデックス増加
	m_soundIndex++;

	return tmpIndex;
}

/****************************************************
* サウンド再生
*****************************************************/
bool Audio::PlayAudio(int& dataIndex, int loopCount) {
	WAVEDATA* wavedata = m_waveData[dataIndex];
	XAUDIO2_VOICE_STATE state;

	//状態取得
	wavedata->sourceVoice->GetState(&state);
	if (state.BuffersQueued != 0){
		wavedata->sourceVoice->Stop();
		wavedata->sourceVoice->FlushSourceBuffers();
	}


	//データ送信
	XAUDIO2_BUFFER buffer = {};
	buffer.AudioBytes = wavedata->size;			//バッファのバイト数
	buffer.pAudioData = (BYTE*)wavedata->soundBuffer;	//バッファの先頭アドレス
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	if (loopCount < 0) loopCount = XAUDIO2_LOOP_INFINITE;
	buffer.LoopCount = loopCount;
	wavedata->sourceVoice->SubmitSourceBuffer(&buffer);

	//再生
	wavedata->sourceVoice->Start();

	return true;
}
