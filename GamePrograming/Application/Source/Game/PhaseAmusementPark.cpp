#include "framework.h"
#include "PhaseAmusementPark.h"

PhaseAmusementPark::PhaseAmusementPark(const int phaseNum) : Phase(phaseNum, 0.0f, 9.8f)
{
	m_texture.Load(L"Data/Texture/park.png");

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
	//m_throwObjectManager->PushLotteryObject(FERRISWHEEL);
	//m_throwObjectManager->PushLotteryObject(MERRYGOROUNDBEAR);
}
