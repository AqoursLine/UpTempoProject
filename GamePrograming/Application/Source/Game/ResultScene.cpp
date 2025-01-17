#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/Movie.h"
#include "ResultScene.h"
#include "Game/Controller.h"

ResultScene::ResultScene() {
	m_resultTex.Load(L"Data/Texture/result.png");
	m_goTitleTex.Load(L"Data/Texture/GoTitle.png");

	m_movie.Load(L"Data/Movie/Logo.avi");

	m_state = RESULT_RESULT;

	m_camera = new Camera();
}

ResultScene::~ResultScene() {

	if (m_camera) delete m_camera;

}

void ResultScene::Update() {
	switch (m_state) {
		case RESULT_RESULT:
			Result();
			break;
		case RESULT_WAIT:
			Wait();
			break;
		case RESULT_TRANSITION:
			Transition();
			break;
	}
}

void ResultScene::Draw() {
	m_camera->Draw();

	//結果発表画面描画
	D3D.Draw2D(m_resultTex, XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f - 300.0f), XMFLOAT2(450.0f, 160.0f));
	D3D.Draw2D(m_resultTex.GetHandle().Get(), XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f - 200.0f), XMFLOAT2(450.0f, 160.0f));

	m_movie.Draw(XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f), XMFLOAT2(1000, 1000));

	//コンティニュー選択描画(上から被せる)
	//ネズミ返し式で描画しない
	if (m_state < RESULT_WAIT) {
		return;
	}
	//ステートがWAIT以上なら描画
	D3D.Draw2D(m_goTitleTex, XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f), XMFLOAT2(830.0f, 160.0f));


	//トランジション描画
	//ステートがTRANSITION以上なら描画
	if (m_state < RESULT_TRANSITION) {
		return;
	}

}

/******************************************************
* 結果発表
*******************************************************/
void ResultScene::Result() {
	//結果発表が終わるまで待つ

	//終わったらステート変更
	if (true) {
		m_state = RESULT_WAIT;
	}
}

/******************************************************
* 入力待ち
*******************************************************/
void ResultScene::Wait() {
	//入力待ち中の背景についての動作処理



	//プレイヤーがボタンを入力するのを待つ
	if (CTRL.GetKeyboardTrigger(DIK_RETURN) || CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_CIRCLE, 0)) {
		m_state = RESULT_TRANSITION;
	}
}

/******************************************************
* トランジション
*******************************************************/
void ResultScene::Transition() {
	//トランジション再生処理

	//トランジション再生終了
	if (true) {
		m_isFinished = true;
	}
}
