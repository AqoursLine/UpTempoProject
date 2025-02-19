#include "framework.h"
#include "PhaseAmusementPark.h"

PhaseAmusementPark::PhaseAmusementPark(const int phaseNum) : Phase(phaseNum, 0.0f, 9.8f)
{
	m_texture.Load(L"Data/Texture/park.png");

	/****************************************************
	* ???̃t?F?[?Y?ɓo?ꂷ?郂?m?̓o?^
	*****************************************************/

	// ?P???u?W?F?N?g
	m_throwObjectManager->PushLotteryObject(WOODENBOX);
	m_throwObjectManager->PushLotteryObject(BEAR);
	m_throwObjectManager->PushLotteryObject(KOKESHI);

	// ?t?F?[?Y?????u?W?F?N?g
	m_throwObjectManager->PushLotteryObject(ABCBLOCK);
	m_throwObjectManager->PushLotteryObject(BALLOON);
	m_throwObjectManager->PushLotteryObject(CLOWN);
	m_throwObjectManager->PushLotteryObject(COFFEECUP);

	//?X?e?[?W?I?u?W?F?N?g?H
	//m_throwObjectManager->PushLotteryObject(FERRISWHEEL);
	//m_throwObjectManager->PushLotteryObject(MERRYGOROUNDBEAR);
}
