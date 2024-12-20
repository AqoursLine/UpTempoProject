#include "framework.h"
#include "Game/PhaseClassRoom.h"

PhaseClassRoom::PhaseClassRoom(const int phaseNum) : Phase(phaseNum, 0.0f, 9.8f)
{
	m_texture.Load("Data/Texture/school_bg.png");

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
}