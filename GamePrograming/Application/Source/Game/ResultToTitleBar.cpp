#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "Game/ResultToTitleBar.h"

ResultToTitleBar::ResultToTitleBar()
	: m_barAnim(
		L"Data/Texture/ResultBarCyan.png",
		XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2),
		XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT),
		0.0f,
		5,
		6,
		28,
		0.5f,
		false
	)
{

}

ResultToTitleBar::~ResultToTitleBar()
{
}

void ResultToTitleBar::Update()
{
	m_barAnim.Update();
}

void ResultToTitleBar::Draw()
{
	m_barAnim.Draw();
}
