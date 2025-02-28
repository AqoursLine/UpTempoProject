#include "framework.h"
#include "PhaseAmusementPark.h"
#include "Game/StageObjectManager.h"

PhaseAmusementPark::PhaseAmusementPark(const int phaseNum) : Phase(phaseNum, 0.0f, 9.8f)
{
	m_texture.Load(L"Data/Texture/ParkPorl_bg.png");

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
	m_stageObjectManager->AddStageObject(S_FERRISWHEEL, 1520.0f, 510.0f, 0.0f, 0);
	for (int i = 0; i < 4; i++) {
		m_stageObjectManager->AddStageObject(S_GONDOLA, 1520.0f, 510.0f, 0.0f, 600, i);
	}

	m_stageObjectManager->AddStageObject(S_HORSE_FRONT, 220.0f, 800.0f, 0.0f, 300, 0);
	m_stageObjectManager->AddStageObject(S_HORSE_BACK, 700.0f, 800.0f, 0.0f, 300, 0);

	m_stageObjectManager->Initialize();
}


