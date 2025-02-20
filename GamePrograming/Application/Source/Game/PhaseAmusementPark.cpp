#include "framework.h"
#include "PhaseAmusementPark.h"
#include "Game/StageObjectManager.h"
#include "DirectX/Audio.h"

PhaseAmusementPark::PhaseAmusementPark(const int phaseNum) : Phase(phaseNum, 0.0f, 9.8f)
{
	m_texture.Load(L"Data/Texture/park_bg.png");

	//BGM読み込み
	soundNum = AUDIO.LoadWaveFile("Data/Sound/BGM/ドキドキあそび広場.wav");

	//BGM再生
	AUDIO.PlayAudio(soundNum, 0);

	AUDIO.SetVolume(soundNum, 0.5f);


	/****************************************************
	* このフェーズに登場するモノの登録
	*****************************************************/

	// 恒常オブジェクト
	m_throwObjectManager->PushLotteryObject(WOODENBOX);
	m_throwObjectManager->PushLotteryObject(BEAR);
	m_throwObjectManager->PushLotteryObject(KOKESHI);

	// フェーズ限定オブジェクト
	m_throwObjectManager->PushLotteryObject(ABCBLOCK);
	m_throwObjectManager->PushLotteryObject(BALLOON);
	m_throwObjectManager->PushLotteryObject(CLOWN);
	m_throwObjectManager->PushLotteryObject(COFFEECUP);

	//ステージオブジェクト？
	StageObjectManager::AddStageObject(S_FERRISWHEEL, 1532, 365, 0.0f, 0);
}


