#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "ResultCharacter.h"
#include "Game/SaveData.h"
#include "Game/easing.h"

ResultCharacter::ResultCharacter()
{
	m_totalPlayer = SaveData::GetTotalPlayer();

	//青ボディ
	m_presentBody.emplace_back();
	m_presentBody.back().Load(L"Data/Texture/BluePresentBody.png");
	//青蓋
	m_presentHead.emplace_back();
	m_presentHead.back().Load(L"Data/Texture/BluePresentHead.png");

	switch (m_totalPlayer) {
		case 4:
			//赤ボディ
			m_presentBody.emplace_back();
			m_presentBody.back().Load(L"Data/Texture/RedPresentBody.png");
			//赤蓋
			m_presentHead.emplace_back();
			m_presentHead.back().Load(L"Data/Texture/RedPresentHead.png");
		case 3:
			//黄色ボディ
			m_presentBody.emplace_back();
			m_presentBody.back().Load(L"Data/Texture/YellowPresentBody.png");
			//黄色蓋
			m_presentHead.emplace_back();
			m_presentHead.back().Load(L"Data/Texture/YellowPresentHead.png");
	}
	//紫ボディ
	m_presentBody.emplace_back();
	m_presentBody.back().Load(L"Data/Texture/PurplePresentBody.png");
	//紫蓋
	m_presentHead.emplace_back();
	m_presentHead.back().Load(L"Data/Texture/PurplePresentHead.png");


	//キャラクターロード
	for (int i = 0; i < m_totalPlayer; i++) {
		int rank = SaveData::GetPlayerRank();
		CHARACTOR chara = SaveData::GetPlayerData(rank).charactorNum;
		m_characterTex.emplace_back();
		Texture& tex = m_characterTex.back();
		switch (chara) {
			case CHARACTOR_IKEMEN:
				tex.Load(L"Data/Texture/ikemen_test.png");
				break;
			case CHARACTOR_NEKKETSU:
				tex.Load(L"Data/Texture/Nekketsu_test.png");
				break;
			case CHARACTOR_FOX:
				tex.Load(L"Data/Texture/fox_test.png");
				break;
			case CHARACTOR_WOMAN:
				tex.Load(L"Data/Texture/Woman_test.png");
				break;
			case CHARACTOR_BISYOUZYO:
				tex.Load(L"Data/Texture/bisyoujo_test.png");
				break;
			case CHARACTOR_RABBIT:
				tex.Load(L"Data/Texture/rabbit_test.png");
				break;
			default:
				break;
		}
	}

	//キャラクターのポジション設定
	m_bluePresentPosition	= m_blueTargetPos	= m_blueStartPos	= m_bluepos		= XMFLOAT2(770, 520);
	m_purplePresentPosition	= m_purpleTargetPos	= m_purpleStartPos	= m_purplepos	= XMFLOAT2(1580, 710);
	m_redPresentPosition	= m_redTargetPos	= m_redStartPos		= m_redpos		= XMFLOAT2(1170, 600);
	m_yellowPresentPosition	= m_yellowTargetPos	= m_yellowStartPos	= m_yellowpos	= XMFLOAT2(440, 700);

	m_blueTargetPos.y = 610;
	m_redTargetPos.y = 680;
	m_yellowTargetPos.y = 750;
	m_purpleTargetPos.y = 890;

	m_blueStartPos.y	= m_bluepos.y	= m_blueTargetPos.y + 120;
	m_purpleStartPos.y	= m_purplepos.y	= m_purpleTargetPos.y + 120;
	m_redStartPos.y		= m_redpos.y	= m_redTargetPos.y + 120;
	m_yellowStartPos.y	= m_yellowpos.y	= m_yellowTargetPos.y + 120;
}

ResultCharacter::~ResultCharacter() {
}

void ResultCharacter::Update() {
	m_frameCount++;

	float time[4];
	
	time[0] = m_frameCount / 30.f;
	time[1] = m_frameCount / 60.0f;
	time[2] = m_frameCount / 40.0f;
	time[3] = m_frameCount / 50.0f;

	for (int i = 0; i < 4; i++) {
		if (time[i] > 1.0f) {
			time[i] = 1.0f;
		}
	}

	m_blueTime = Easing::OutQuart(time[0]);
	m_purpleTime = Easing::OutQuart(time[1]);
	m_redTime = Easing::OutQuart(time[2]);
	m_yellowTime = Easing::OutQuart(time[3]);

	m_bluepos.y		= m_blueStartPos.y + (m_blueTargetPos.y - m_blueStartPos.y) * m_blueTime;
	m_purplepos.y	= m_purpleStartPos.y + (m_purpleTargetPos.y - m_purpleStartPos.y) * m_purpleTime;
	m_redpos.y		= m_redStartPos.y + (m_redTargetPos.y - m_redStartPos.y) * m_redTime;
	m_yellowpos.y	= m_yellowStartPos.y + (m_yellowTargetPos.y - m_yellowStartPos.y) * m_yellowTime;


	//時間が経ったら
	if (m_frameCount >= 60) {
		m_isFinished = true;
	}
}

void ResultCharacter::Draw() {
	float presentSize = 1200.f;
	float characterSize = 700.f;
	float scale = 0.5f + (m_frameCount / 60.0f) * 0.5f;
	float charaHeight = characterSize * scale;

	//青蓋
	D3D.Draw2D(m_presentHead[0], m_bluePresentPosition, XMFLOAT2(presentSize, presentSize), XMConvertToRadians(-4.5f));
	//キャラ本体
	D3D.Draw2D(m_characterTex[0], XMFLOAT2(m_bluepos.x, m_bluepos.y - (characterSize - charaHeight) * 0.5f), XMFLOAT2(characterSize, charaHeight), 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, scale));
	//青ボディ
	D3D.Draw2D(m_presentBody[0], m_bluePresentPosition, XMFLOAT2(presentSize, presentSize), XMConvertToRadians(-4.5f));

	//D3D.Draw2D(m_1stVideo.GetSRV(), m_bluePresentPosition, XMFLOAT2(500, 500), PIXELMODE_DEFAULT);

	switch (m_totalPlayer) {
		case 4:
			presentSize = 1100.f;
			characterSize = 550.f;
			charaHeight = characterSize * scale;
			//赤蓋
			D3D.Draw2D(m_presentHead[1], m_redPresentPosition, XMFLOAT2(presentSize * -1, presentSize), XMConvertToRadians(5.5f));
			//キャラ本体
			D3D.Draw2D(m_characterTex[1], XMFLOAT2(m_redpos.x, m_redpos.y - (characterSize - charaHeight) * 0.5f), XMFLOAT2(characterSize, charaHeight), 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, scale));
			//赤ボディ
			D3D.Draw2D(m_presentBody[1], m_redPresentPosition, XMFLOAT2(presentSize * -1, presentSize), XMConvertToRadians(5.5f));

			//D3D.Draw2D(m_1stVideo.GetSRV(), m_redPresentPosition, XMFLOAT2(500, 500), PIXELMODE_DEFAULT);
		case 3:
			presentSize = 950.f;
			characterSize = 500.f;
			charaHeight = characterSize * scale;

			//黄色蓋
			D3D.Draw2D(m_presentHead[m_totalPlayer - 2], m_yellowPresentPosition, XMFLOAT2(presentSize, presentSize));
			//キャラ本体
			D3D.Draw2D(m_characterTex[m_totalPlayer - 2], XMFLOAT2(m_yellowpos.x, m_yellowpos.y - (characterSize - charaHeight) * 0.5f), XMFLOAT2(characterSize, charaHeight), 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, scale));
			//黄色ボディ
			D3D.Draw2D(m_presentBody[m_totalPlayer - 2], m_yellowPresentPosition, XMFLOAT2(presentSize, presentSize));
	}

	presentSize = 900.f;
	characterSize = 400.f;
	charaHeight = characterSize * 0.5f;

	//紫蓋
	D3D.Draw2D(m_presentHead[m_totalPlayer - 1], m_purplePresentPosition, XMFLOAT2(presentSize * -1, presentSize));
	//キャラ本体
	D3D.Draw2D(m_characterTex[m_totalPlayer - 1], XMFLOAT2(m_purplepos.x, m_purplepos.y - charaHeight * 0.5f), XMFLOAT2(characterSize, charaHeight), 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 0.5f));
	//紫ボディ
	D3D.Draw2D(m_presentBody[m_totalPlayer - 1], m_purplePresentPosition, XMFLOAT2(presentSize * -1, presentSize));
}


