#include "framework.h"
#include "PhaseOcean.h"

PhaseOcean::PhaseOcean(const int phaseNum) : Phase(phaseNum, 0.0f, 9.8f)
{
	m_texture.Load(L"Data/Texture/ocean.png");

	/****************************************************
	* ???̃t?F?[?Y?ɓo?ꂷ?郂?m?̓o?^
	*****************************************************/

	// ?P???u?W?F?N?g
	m_throwObjectManager->PushLotteryObject(WOODENBOX);
	m_throwObjectManager->PushLotteryObject(BEAR);
	m_throwObjectManager->PushLotteryObject(KOKESHI);

	// ?t?F?[?Y?????u?W?F?N?g
	m_throwObjectManager->PushLotteryObject(SHELL);
	m_throwObjectManager->PushLotteryObject(BARREL);
	m_throwObjectManager->PushLotteryObject(CORAL);
	m_throwObjectManager->PushLotteryObject(ANCHOR);
	m_throwObjectManager->PushLotteryObject(WHALE);
}
