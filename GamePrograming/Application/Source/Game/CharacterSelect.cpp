#include "framework.h"
#include "DirectX/DirectX.h"
#include "CharacterSelect.h"
#include "Game/Controller.h"

CharacterSelect::CharacterSelect() {
	m_backGroundTex.Load(L"Data/Texture/CharacterSelectBg.png");
}

CharacterSelect::~CharacterSelect() {
}

void CharacterSelect::Update() {
	//とりあえずエンターキーを押したら終了
	if (CTRL.GetKeyboardTrigger(DIK_RETURN)) {
		m_isFinished = true;
	}

}

void CharacterSelect::Draw() {
	//背景描画
	D3D.Draw2D(m_backGroundTex, XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f), XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT));
}


