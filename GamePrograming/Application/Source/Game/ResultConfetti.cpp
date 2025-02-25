#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "ResultConfetti.h"

ResultConfetti::ResultConfetti()
	:m_confettiVideo(L"Data/Movie/Confetti_Result.avi")
{
	m_Pos = XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	m_confettiVideo.SetIsAutoLoop(false);
}

ResultConfetti::~ResultConfetti()
{
}

void ResultConfetti::Update()
{
	m_confettiVideo.Update(GAMESYS.GetDletaTime());
}

void ResultConfetti::Draw()
{
	D3D.Draw2D(m_confettiVideo.GetSRV(), m_Pos, XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT), PIXELMODE_DEFAULT);
}
