#include "framework.h"
#include "DirectX/DirectX.h"
#include "ResultScene.h"
#include "Game/Controller.h"
#include "DirectX/Audio.h"


ResultScene::ResultScene()
	: m_OUT_transition(
		L"Data/Texture/Transition/OUT/CircleLine_OUT.png",
		XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2),
		XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT),
		0.0f,
		5,
		5,
		25,
		0.5f,
		false
	),
	m_IN_transition(
		L"Data/Texture/Transition/IN/CircleShape_IN.png",
		XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2),
		XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT),
		0.0f,
		5,
		7,
		33,
		0.5f,
		false
	)
{
	m_resultTex.Load(L"Data/Texture/result.png");
	m_goTitleTex.Load(L"Data/Texture/GoTitle.png");
	m_resultBgTex.Load(L"Data/Texture/ResultBg.png");

	//BGM読み込み
	m_soundNum = AUDIO.LoadWaveFile("Data/Sound/BGM/これより開幕.wav");

	m_cheersSound = AUDIO.LoadWaveFile("Data/Sound/SE/歓声.wav");

	m_resultCharacter = new ResultCharacter();

	m_resultRankAnims = new ResultRankAnims();

	m_confettiVideo = new ResultConfetti();

	m_toTitleBar = new ResultToTitleBar();

	m_state = RESULT_START;

	m_camera = new Camera();

	//BGM再生
	AUDIO.PlayAudio(m_soundNum, -1);

	AUDIO.SetVolume(m_soundNum, 1.0f);

	AUDIO.SetVolume(m_cheersSound, 1.0f);
}

ResultScene::~ResultScene() {

	if (m_camera) delete m_camera;
	if (m_resultCharacter) delete m_resultCharacter;

	if (m_resultRankAnims) delete m_resultRankAnims;
	if (m_confettiVideo) delete m_confettiVideo;
	if (m_toTitleBar) delete m_toTitleBar;


	AUDIO.StopAudio(m_soundNum);

}

void ResultScene::Update() {
	switch (m_state) {
		case RESULT_START:
			Start();
			break;
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

	//背景描画
	D3D.Draw2D(m_resultBgTex, XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f), XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT));

	//結果発表画面描画
	D3D.Draw2D(m_resultTex, XMFLOAT2(SCREEN_WIDTH * 0.5f, 150.0f), XMFLOAT2(450.0f, 160.0f));


	m_resultCharacter->Draw();

	// ランキングアニメーション描画
	m_resultRankAnims->Draw();

	// 紙吹雪描画
	m_confettiVideo->Draw();

	// 最初のトランジション描画
	if (!m_OUT_transition.IsAnimFinished()) {
		m_OUT_transition.Draw();
	}

	//コンティニュー選択描画(上から被せる)
	//ネズミ返し式で描画しない
	if (m_state < RESULT_WAIT) {
		return;
	}


	//ステートがWAIT以上なら描画
	m_toTitleBar->Draw();

//	D3D.Draw2D(m_goTitleTex, XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f), XMFLOAT2(830.0f, 160.0f));


	//トランジション描画
	//ステートがTRANSITION以上なら描画
	if (m_state < RESULT_TRANSITION) {
		return;
	}


	m_IN_transition.Draw();
}

/******************************************************
* 数フレーム待つ
*******************************************************/
void ResultScene::Start() {
	//m_stateCount++;

	m_OUT_transition.Update();

	if (m_OUT_transition.IsAnimFinished()) {
		AUDIO.PlayAudio(m_cheersSound, 0);
		m_state = RESULT_RESULT;
	}
}

/******************************************************
* 結果発表
*******************************************************/
void ResultScene::Result() {

	m_resultRankAnims->Update();

	//結果発表が終わるまで待つ
	m_resultCharacter->Update();

	m_confettiVideo->Update();

	//終わったらステート変更
	if (m_resultCharacter->GetIsFinished()) {
		m_state = RESULT_WAIT;
	}
}

/******************************************************
* 入力待ち
*******************************************************/
void ResultScene::Wait() {
	//入力待ち中の背景についての動作処理
	m_resultRankAnims->Update();

	m_confettiVideo->Update();

	m_toTitleBar->Update();


	//プレイヤーがボタンを入力するのを待つ
	if (CTRL.GetKeyboardTrigger(DIK_RETURN) || CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_TRIANGLE, 0)) {
		m_state = RESULT_TRANSITION;
	}
}

/******************************************************
* トランジション
*******************************************************/
void ResultScene::Transition() {
	//トランジション再生処理
	m_IN_transition.Update();

	//トランジション再生終了
	if (m_IN_transition.IsAnimFinished()) {
		m_isFinished = true;
	}
}


