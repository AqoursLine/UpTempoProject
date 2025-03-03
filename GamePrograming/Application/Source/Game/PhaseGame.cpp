#include "framework.h"
#include "PhaseGame.h"

PhaseGame::PhaseGame(const int phaseNum) : Phase(phaseNum, 0.0f, 9.8f)
{
	m_texture.Load(L"Data/Texture/game2_big.png");


/****************************************************
* このフェーズに登場するモノの登録
*****************************************************/

	// 恒常オブジェクト
	m_throwObjectManager->PushLotteryObject(WOODENBOX);
	m_throwObjectManager->PushLotteryObject(BEAR);
	m_throwObjectManager->PushLotteryObject(KOKESHI);

	// フェーズ限定オブジェクト
	m_throwObjectManager->PushLotteryObject(APPLE);
	m_throwObjectManager->PushLotteryObject(SWORD);
	m_throwObjectManager->PushLotteryObject(SHIELD);
	m_throwObjectManager->PushLotteryObject(COIN);
	m_throwObjectManager->PushLotteryObject(SLIME);
	m_throwObjectManager->PushLotteryObject(SCAFFOLD);

	m_stageObjectManager->AddStageObject(S_MOON, 1300.0f, 300.0f, 0.0f, 600);
	m_stageObjectManager->AddStageObject(S_BOARD, 1500.0f, 900.0f, 0.0f, 600);
	m_stageObjectManager->AddStageObject(S_CLOUD, 1650.0f, 400.0f, 0.0f, 600);
	m_stageObjectManager->AddStageObject(S_CLOUD, 300.0f, 400.0f, 0.0f, 600);
	m_stageObjectManager->AddStageObject(S_CLOUD, 700.0f, 300.0f, 0.0f, 600);
	m_stageObjectManager->AddStageObject(S_GRASS, 250.0f, 900.0f, 0.0f, 600);

	m_stageObjectManager->Initialize();
}


