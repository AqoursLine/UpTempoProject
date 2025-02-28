#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/Physics.h"
#include "Game/PlayerNum.h"
#include "Game/Player.h"

PlayerNum::PlayerNum() {
}

PlayerNum::~PlayerNum() {
}

void PlayerNum::Update() {
	if (!m_existsThis) {
		return;
	}

	m_frameCnt++;

	constexpr int MAX_FRAME = 30;

	if (m_frameCnt >= MAX_FRAME) {
		m_existsThis = false;
	}

	m_alpha = static_cast<float>(m_frameCnt) / MAX_FRAME;

	m_alpha = 1.0f - m_alpha;
}

void PlayerNum::Draw() {
	if (!m_existsThis) {
		return;
	}
	XMFLOAT2 pos = m_player->GetPos();
	pos.y -= 200.0f;

	float ratio = 0.2f;
	XMFLOAT2 size = XMFLOAT2(500.0f * ratio, 281.0f * ratio);
	D3D.Draw2D(m_tex, pos, size, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, m_alpha));
}

void PlayerNum::SetTexture(Player* pPlayer, bool isCPU, int pNum) {
	if (isCPU) {
		pNum = 5;
	}

	std::wstring filePath = L"Data/Texture/button" + std::to_wstring(pNum) + L".png";

	m_tex.Load(filePath);

	m_player = pPlayer;
}
