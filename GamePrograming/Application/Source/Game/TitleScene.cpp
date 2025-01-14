#include "framework.h"
#include "DirectX/DirectX.h"
#include "TitleScene.h"
#include "Game/Controller.h"

TitleScene::TitleScene() {
	m_startTex.Load("Data/Texture/start.png");
	m_quitTex.Load("Data/Texture/quit.png");
	m_titleChoose.Load("Data/Texture/title_choose.png");

	m_pos.x = SCREEN_WIDTH * 0.5f;
	m_pos.y = SCREEN_HEIGHT * 0.5f + 100.0f;

	m_size.x = 300;
	m_size.y = 100;

	m_gamepadMax = CTRL.GetGamepadMax();
}

TitleScene::~TitleScene() {
}

void TitleScene::Update() {
	//選択用
	bool isUpTrigger = false, isDownTrigger = false;

	//キーボード
	if (CTRL.GetKeyboardTrigger(DIK_W)) {
		isUpTrigger = true;
	}
	if (CTRL.GetKeyboardTrigger(DIK_S)) {
		isDownTrigger = true;
	}

	//ゲームパッド
	for (int i = 0; i < m_gamepadMax; i++) {
		if (CTRL.GetLeftStickVertical(i) > 0) {
			isUpTrigger = true;
		}
		if (CTRL.GetLeftStickVertical(i) < 0) {
			isDownTrigger = true;
		}
	}

	//移動
	if (isUpTrigger || isDownTrigger) {
		m_choose = 1 - m_choose;
	}


	//決定
	bool isEnter = false;
	//キーボード
	if (CTRL.GetKeyboardTrigger(DIK_RETURN)) {
		isEnter = true;
	}
	//ゲームパッド
	for (int i = 0; i < m_gamepadMax; i++) {
		if (CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_CIRCLE, i)) {
			isEnter = true;
		}
	}

	//実行
	if (isEnter) {
		//シーン遷移
		if (m_choose == 0) {
			m_isFinished = true;
		} else if (m_choose == 1) {
			m_isEnd = true;
		}
	}

}

void TitleScene::Draw() {
	//行列設定
	XMMATRIX projection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	D3D.SetProjectionMatrix(projection);
	XMMATRIX view = XMMatrixIdentity();
	D3D.SetViewMatrix(view);

	D3D.Draw2D(m_titleChoose, XMFLOAT2(m_pos.x, m_pos.y + m_distance * m_choose), m_size);

	D3D.Draw2D(m_startTex, m_pos, m_size);

	D3D.Draw2D(m_quitTex, XMFLOAT2(m_pos.x, m_pos.y + m_distance), m_size);
}

