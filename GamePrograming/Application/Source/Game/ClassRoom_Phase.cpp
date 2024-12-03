#include "framework.h"
#include "Game/ClassRoom_Phase.h"
#include "Kokeshi.h"
#include "Bear.h"
#include "WoodenBox.h"

ClassRoom::ClassRoom():Phase(0.0f,9.8f)
{
	// ƒ‚ƒm‚Ì“o˜^
	m_throwObjectManager->PushLotteryObject(new WoodenBox(SCREEN_WIDTH * 0.5f - 250.0f, SCREEN_HEIGHT * 0.5f + 200, 0.0f));
	m_throwObjectManager->PushLotteryObject(new Kokeshi(SCREEN_WIDTH * 0.5f + 250.0f, SCREEN_HEIGHT * 0.5f + 200, 0.0f));
	m_throwObjectManager->PushLotteryObject(new Bear(SCREEN_WIDTH * 0.5f + 100.0f, SCREEN_HEIGHT * 0.5f + 200, 0.0f));
}
