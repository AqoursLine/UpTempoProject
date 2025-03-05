#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "Game/ResultToTitleBar.h"

constexpr float BAR_RATIO = 0.8f;
constexpr float BAR_WIDTH = 1250.0f * BAR_RATIO;
constexpr float BAR_HEIGHT = 500.0f * BAR_RATIO;

ResultToTitleBar::ResultToTitleBar()
	: m_barAnim(
		L"Data/Texture/ResultBarRed.png",
		XMFLOAT2(SCREEN_WIDTH - BAR_WIDTH * 0.5f, BAR_HEIGHT *0.5f),
		XMFLOAT2(BAR_WIDTH, BAR_HEIGHT),
		0.0f,
		3,
		5,
		15,
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
