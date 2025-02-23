#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "ResultRankVideos.h"

ResultRankVideos::ResultRankVideos()
	:m_1stVideo(L"Data/Movie/1st.avi"),
	m_2ndVideo(L"Data/Movie/2nd.avi"),
	m_3rdVideo(L"Data/Movie/3rd.avi"),
	m_4thVideo(L"Data/Movie/4th.avi")

{
	m_1stPos = XMFLOAT2(790, 700);
	m_2ndPos = XMFLOAT2(1100, 750);
	m_3rdPos = XMFLOAT2(440, 780);
	m_4thPos = XMFLOAT2(1550, 900);


	// 動画をループするようにする
	m_1stVideo.SetIsAutoLoop(true);
	m_2ndVideo.SetIsAutoLoop(true);
	m_3rdVideo.SetIsAutoLoop(true);
	m_4thVideo.SetIsAutoLoop(true);
}

ResultRankVideos::~ResultRankVideos()
{
}

void ResultRankVideos::Update()
{
	m_1stVideo.Update(GAMESYS.GetDletaTime());
	m_2ndVideo.Update(GAMESYS.GetDletaTime());
	m_3rdVideo.Update(GAMESYS.GetDletaTime());
	m_4thVideo.Update(GAMESYS.GetDletaTime());
}

void ResultRankVideos::Draw()
{
	D3D.Draw2D(m_1stVideo.GetSRV(), m_1stPos, XMFLOAT2(400, 400), PIXELMODE_DEFAULT);
	D3D.Draw2D(m_2ndVideo.GetSRV(), m_2ndPos, XMFLOAT2(400, 400), PIXELMODE_DEFAULT);
	D3D.Draw2D(m_3rdVideo.GetSRV(), m_3rdPos, XMFLOAT2(350, 350), PIXELMODE_DEFAULT);
	D3D.Draw2D(m_4thVideo.GetSRV(), m_4thPos, XMFLOAT2(300, 300), PIXELMODE_DEFAULT);
}
