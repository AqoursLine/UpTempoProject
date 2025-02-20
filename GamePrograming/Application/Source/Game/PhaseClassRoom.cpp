#include "framework.h"
#include "Game/PhaseClassRoom.h"
#include "Game/StageObjectManager.h"
#include "DirectX/Audio.h"

PhaseClassRoom::PhaseClassRoom(const int phaseNum) : Phase(phaseNum, 0.0f, 9.8f)
{
	m_texture.Load(L"Data/Texture/bg_NewSchool.png");

	//BGM読み込み
	soundNum = AUDIO.LoadWaveFile("Data/Sound/BGM/KAMIKAZE_ATTACK.wav");

	//BGM再生
	AUDIO.PlayAudio(soundNum, 0);

	/****************************************************
	* このフェーズに登場するモノの登録
	*****************************************************/

	// 恒常オブジェクト
	m_throwObjectManager->PushLotteryObject(WOODENBOX);
	m_throwObjectManager->PushLotteryObject(BEAR);
	m_throwObjectManager->PushLotteryObject(KOKESHI);
	
	// フェーズ限定オブジェクト
	m_throwObjectManager->PushLotteryObject(HOUKI);
	m_throwObjectManager->PushLotteryObject(PC);
	m_throwObjectManager->PushLotteryObject(PLATFORM);
	m_throwObjectManager->PushLotteryObject(NOTE);

	//ここでステージオブジェクト配置
	StageObjectManager::AddStageObject(S_LAMP_LEFT, SCREEN_WIDTH * 0.5f - 400.0f, SCREEN_HEIGHT * 0.5f - 400, 0.0f, 600);
	StageObjectManager::AddStageObject(S_LAMP_RIGHT, SCREEN_WIDTH * 0.5f + 400.0f, SCREEN_HEIGHT * 0.5f - 400, 0.0f, 600);
}

