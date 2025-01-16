#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/StageSelect.h"
#include "Game/Controller.h"

StageSelect::StageSelect() {
	m_backGroundTex.Load(L"Data/Texture/StageSelectBg.png");
}

StageSelect::~StageSelect() {
}

void StageSelect::Update() {
	//とりあえずエンターキーを押したら終了
	if (CTRL.GetKeyboardTrigger(DIK_RETURN)) {
		m_isFinished = true;
	}
}

void StageSelect::Draw() {
	D3D.Draw2D(m_backGroundTex, XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f), XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT));
}
