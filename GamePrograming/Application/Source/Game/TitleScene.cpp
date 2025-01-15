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

	m_camera = new Camera();

	bool result =  m_test.Load(L"Data\\Movie\\test.mp4");

	if (!result) {
		MessageBox(NULL, L"動画読み込みエラー", L"エラー", MB_OK);
	}
}

TitleScene::~TitleScene() {
	if (m_camera) {
		delete m_camera;
	}
}

void TitleScene::Update() {
	m_test.Update();

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

		m_test.SeekToStart();
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
	m_camera->Draw();

	D3D.Draw2D(m_titleChoose, XMFLOAT2(m_pos.x, m_pos.y + m_distance * m_choose), m_size);

	D3D.Draw2D(m_startTex, m_pos, m_size);

	D3D.Draw2D(m_quitTex, XMFLOAT2(m_pos.x, m_pos.y + m_distance), m_size);

	m_test.Draw(XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f), XMFLOAT2(200, 200));
}

