#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "Game/SaveData.h"
#include "ResultRankVideos.h"

ResultRankAnims::ResultRankAnims()
	:m_1stAnim(
		L"Data/Texture/1st_UV.png",
		XMFLOAT2(810, 720),
		XMFLOAT2(400, 400),
		0.0f,
		5,
		10,
		50,
		0.5f,
		true
	),

	m_2ndAnim(
		L"Data/Texture/2nd_UV.png",
		XMFLOAT2(1100,750),
		XMFLOAT2(400,400),
		0.0f,
		5,
		6,
		30,
		0.5f,
		true
		),

	m_3rdAnim(
		L"Data/Texture/3rd_UV.png",
		XMFLOAT2(440,820),
		XMFLOAT2(350,350),
		0.0f,
		5,
		9,
		42,
		0.5f,
		true
	),

	m_4thAnim(L"Data/Texture/4th_UV.png",
		XMFLOAT2(1550,870),
		XMFLOAT2(300,300),
		0.0f,
		5,
		11,
		54,
		0.5f,
		true
	)

{
	//m_1stPos = XMFLOAT2(790, 700);
	//m_2ndPos = XMFLOAT2(1100, 750);
	//m_3rdPos = XMFLOAT2(440, 780);
	//m_4thPos = XMFLOAT2(1550, 900);


	m_totalPlayer = SaveData::GetTotalPlayer();

	// 人数によって順位のposを変える
	switch (m_totalPlayer)
	{
	case 3:
		m_2ndAnim.SetPos(XMFLOAT2(440, 820));
		m_2ndAnim.SetSize(XMFLOAT2(350, 350));

		m_3rdAnim.SetPos(XMFLOAT2(1550, 870));
		m_3rdAnim.SetSize(XMFLOAT2(300, 300));
		break;

	case 2:
		m_2ndAnim.SetPos(XMFLOAT2(1550, 870));
		m_2ndAnim.SetSize(XMFLOAT2(300, 300));
		break;
		
	default:
		break;
	}
}

ResultRankAnims::~ResultRankAnims()
{
}

void ResultRankAnims::Update()
{
	//m_1stVideo.Update(GAMESYS.GetDletaTime());
	//m_2ndVideo.Update(GAMESYS.GetDletaTime());
	//m_3rdVideo.Update(GAMESYS.GetDletaTime());
	//m_4thVideo.Update(GAMESYS.GetDletaTime());

	switch (m_totalPlayer)
	{
	case 4:
		m_4thAnim.Update();

	case 3:
		m_3rdAnim.Update();

	case 2:
		m_1stAnim.Update();
		m_2ndAnim.Update();
	default:
		break;
	}

}

void ResultRankAnims::Draw()
{
	switch (m_totalPlayer)
	{
	case 4:
		m_4thAnim.Draw();

	case 3:
		m_3rdAnim.Draw();

	case 2:
		m_1stAnim.Draw();
		m_2ndAnim.Draw();

	default:
		break;
	}
}
