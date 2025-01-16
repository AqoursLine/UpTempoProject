#include "framework.h"
#include "DirectX/DirectX.h"
#include "TitleScene.h"
#include "Game/Controller.h"

TitleScene::TitleScene() {
	//テクスチャ読込
	m_logoTex.Load(L"Data/Texture/Logo.png");
	m_startTex.Load(L"Data/Texture/start.png");
	m_quitTex.Load(L"Data/Texture/quit.png");
	m_titleChoose.Load(L"Data/Texture/title_choose.png");

	//選択肢用座標
	m_pos.x = SCREEN_WIDTH * 0.5f;
	m_pos.y = SCREEN_HEIGHT * 0.5f + 100.0f;
	float scale = 0.6f;
	m_size.x = 540.0f * scale;
	m_size.y = 220.0f * scale;

	//カメラ
	m_camera = new Camera();

	//ステート
	m_state = TITLE_START;
}

TitleScene::~TitleScene() {
	if (m_camera) {
		delete m_camera;
	}
}

void TitleScene::Update() {
	switch (m_state) {
		case TITLE_RUN:
			Run();
			break;
		case TITLE_START:
			Start();
			break;
		case TITLE_TRANSITION:
			Transition();
			break;
	}
}

void TitleScene::Draw() {
	m_camera->Draw();

	//タイトル画面の描画
	//if文は使わずそのまま描画
	//ロゴ
	D3D.Draw2D(m_logoTex, XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f - 200.0f), XMFLOAT2(650.0f, 350.0f));
	
	//選択肢背景
	D3D.Draw2D(m_titleChoose, XMFLOAT2(m_pos.x, m_pos.y + m_distance * m_choose), XMFLOAT2(m_size.x * 1.2f, m_size.y * 1.2f), 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	//スタート
	D3D.Draw2D(m_startTex, m_pos, m_size);
	//クイット
	D3D.Draw2D(m_quitTex, XMFLOAT2(m_pos.x, m_pos.y + m_distance), m_size);

	//開始アニメーションの描画
	//タイトル画面の上に被せるように描画することでスムーズにタイトル画面に遷移出来るのではという試み
	if (m_state == TITLE_START) {
	}

	//トランジション描画
	if (m_state == TITLE_TRANSITION) {
	}
}

/******************************************************
* 選択画面の操作
*******************************************************/
void TitleScene::Run() {
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
	if (CTRL.GetGamepadMax() > 0) {
		if (CTRL.GetLeftStickVertical(0) > 0) {
			isUpTrigger = true;
		}
		if (CTRL.GetLeftStickVertical(0) < 0) {
			isDownTrigger = true;
		}
	}

	//移動
	if (isUpTrigger || isDownTrigger) {
		m_choose = 1 - m_choose;
	}


	//決定
	if (CTRL.GetKeyboardTrigger(DIK_RETURN) || CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_CIRCLE, 0)) {
		m_state = TITLE_TRANSITION;
	}
}

/******************************************************
* 起動アニメーション等の操作
*******************************************************/
void TitleScene::Start() {
	//アニメーション再生処理


	//描画が終わった
	if (true) {
		m_state = TITLE_RUN;
	}

}

/******************************************************
* トランジション時の操作
*******************************************************/
void TitleScene::Transition() {
	//トランジション再生処理


	//トランジションが終わった
	if (true) {
		//シーン遷移
		if (m_choose == 0) {
			m_isFinished = true;
		} else if (m_choose == 1) {
			m_isEnd = true;
		}
	}
}

