#include "framework.h"
#include "PhaseGame.h"

PhaseGame::PhaseGame(const int phaseNum) : Phase(phaseNum, 0.0f, 9.8f)
{
	m_texture.Load(L"Data/Texture/game2.png");

/****************************************************
* ???̃t?F?[?Y?ɓo?ꂷ?郂?m?̓o?^
*****************************************************/

	// ?P???u?W?F?N?g
	m_throwObjectManager->PushLotteryObject(WOODENBOX);
	m_throwObjectManager->PushLotteryObject(BEAR);
	m_throwObjectManager->PushLotteryObject(KOKESHI);

	// ?t?F?[?Y?????u?W?F?N?g
	m_throwObjectManager->PushLotteryObject(APPLE);
	m_throwObjectManager->PushLotteryObject(SWORD);
	m_throwObjectManager->PushLotteryObject(SHIELD);
	m_throwObjectManager->PushLotteryObject(COIN);
	m_throwObjectManager->PushLotteryObject(SLIME);
	m_throwObjectManager->PushLotteryObject(MOON);
	m_throwObjectManager->PushLotteryObject(BOARD);
	m_throwObjectManager->PushLotteryObject(SCAFFOLD);
	m_throwObjectManager->PushLotteryObject(CLOUD);
	m_throwObjectManager->PushLotteryObject(GRASS);
}
