#include "framework.h"
#include "DirectX/DirectX.h"
#include "DirectX/video_texture.h"
#include "TitleScene.h"
#include "Game/Controller.h"
#include "DirectX/Audio.h"
#include "Game/GameSystem.h"

TitleScene::TitleScene()
	:m_IN_transition(
		L"Data/Texture/Transition/IN/Aperture_IN.png",
		XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2),
		XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT),
		0.0f,
		5,
		8,
		37,
		0.5f,
		false
	),
	m_OUT_transition(
		L"Data/Texture/Transition/OUT/CircleShape_OUT.png",
		XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2),
		XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT),
		0.0f,
		5,
		7,
		32,
		0.5f,
		false
	),
	m_op(L"Data/Movie/OP.avi")
{
	//テクスチャ読込
	m_logoTex.Load(L"Data/Texture/TitleLogo.png");
	m_startTex.Load(L"Data/Texture/StartButton.png");
	m_quitTex.Load(L"Data/Texture/QuitButton.png");
	m_titleChoose.Load(L"Data/Texture/title_choose.png");

	//BGM読み込み
	m_soundNum = AUDIO.LoadWaveFile("Data/Sound/BGM/Merrily_POP_1.wav");
	m_decisionSound = AUDIO.LoadWaveFile("Data/Sound/SE/決定10.wav");
	m_opSE = AUDIO.LoadWaveFile("Data/Sound/SE/オープニングSE.wav");

	//選択肢用座標
	m_pos.x = SCREEN_WIDTH * 0.5f + 525.0f;
	m_pos.y = SCREEN_HEIGHT * 0.5f + 100.0f;
	float scale = 0.6f;
	m_size.x = 540.0f * scale;
	m_size.y = 220.0f * scale;

	//カメラ
	m_camera = new Camera();

	// 背景動画のnew
	m_backMovie = std::make_unique<BackGroundMovie>();

	m_logo.create("Data/Movie/ロゴアニ.mp4");
	m_logo.setLooping(false);
	m_logoSound = AUDIO.LoadWaveFile("Data/Sound/SE/ロゴアニ.wav");

	m_op.SetIsAutoLoop(false);

	//サウンド音量
	AUDIO.SetVolume(m_soundNum, 1.0f);
	AUDIO.SetVolume(m_decisionSound, 0.5f);
	AUDIO.SetVolume(m_logoSound, 1.0f);
	AUDIO.SetVolume(m_opSE, 1.0f);

	//ステート
	// 前回のシーンがリザルトならトランジションを再生した後にオープニングアニメーションを再生
	if (GAMESYS.GetOldSceneNum() == SCENE_RESULT) {
		m_state = TITLE_START;
	}
	// そうでない場合はオープニングアニメーションを再生
	else {
		m_state = TITLE_LOGO;
		//BGM再生
		AUDIO.PlayAudio(m_logoSound, 0);
	}
}

TitleScene::~TitleScene() {
	if (m_camera) {
		delete m_camera;
	}

	m_logo.destroy();

	//サウンドの停止
	AUDIO.StopAudio(m_soundNum);

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
		case TITLE_LOGO:
			Logo();
			break;
		case TITLE_OP:
			Opening();
			break;
	}
}

void TitleScene::Draw() {
	m_camera->Draw();

	m_backMovie->Draw();

	//開始アニメーションの描画
	//タイトル画面の上に被せるように描画することでスムーズにタイトル画面に遷移出来るのではという試み
	//タイトル画面の描画
	//if文は使わずそのまま描画
	//ロゴ
	D3D.Draw2D(m_logoTex, XMFLOAT2(SCREEN_WIDTH * 0.5f + 615.0f, SCREEN_HEIGHT * 0.5f - 250.0f), XMFLOAT2(1920 * 0.4f, 1080 * 0.4f));

	//選択肢背景
	D3D.Draw2D(m_titleChoose, XMFLOAT2(m_pos.x + (m_choose * -50.0f), m_pos.y + m_distance * m_choose), XMFLOAT2(m_size.x * 1.2f, m_size.y * 1.2f), 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.1f, 1.0f));
	//スタート
	D3D.Draw2D(m_startTex, m_pos, m_size);
	//クイット
	D3D.Draw2D(m_quitTex, XMFLOAT2(m_pos.x - 50.0f, m_pos.y + m_distance), m_size);


	//トランジション描画
	if (m_state == TITLE_TRANSITION) {
		m_IN_transition.Draw();
	}

	//ロゴ描画
	if (m_state == TITLE_LOGO || m_state == TITLE_START) {
		D3D.Draw2D(m_logo.getTexture()->shader_resource_view, XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f), XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT), PIXELMODE_MOVIE);
	}

	//OP描画
	if (m_state == TITLE_OP) {
		D3D.Draw2D(m_op.GetSRV(), XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f), XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT),PIXELMODE_DEFAULT);
	}

	if (m_state == TITLE_START) {

		if (!m_OUT_transition.IsAnimFinished()) {
			m_OUT_transition.Draw();
		}
	}

}

/******************************************************
* 選択画面の操作
*******************************************************/
void TitleScene::Run() {

	//キーボード
	if (CTRL.GetKeyboardTrigger(DIK_W)) {
		m_choose = 0;
	}
	if (CTRL.GetKeyboardTrigger(DIK_S)) {
		m_choose = 1;
	}
	//ゲームパッド
	if (CTRL.GetGamepadMax() > 0) {
		if (CTRL.GetLeftStickVertical(0) > 0) {
			m_choose = 1;
		}
		if (CTRL.GetLeftStickVertical(0) < 0) {
			m_choose = 0;
		}
	
	}

	//決定
	if (CTRL.GetKeyboardTrigger(DIK_RETURN) || CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_CIRCLE, 0)) {
		AUDIO.PlayAudio(m_decisionSound,0);
		m_state = TITLE_TRANSITION;
	}

	// 動画のアップデート
	m_backMovie->Update();
}

/******************************************************
* 起動アニメーション等の操作
*******************************************************/
void TitleScene::Start() {
	//アニメーション再生処理
	m_OUT_transition.Update();

	// トランジションが終わったらオープニングアニメーションを再生
	if (m_OUT_transition.IsAnimFinished()) {
		m_state = TITLE_LOGO;
		//BGM再生
		AUDIO.PlayAudio(m_logoSound, 0);
	}
}

/******************************************************
* トランジション時の操作
*******************************************************/
void TitleScene::Transition() {
	//トランジション再生処理
	m_IN_transition.Update();

	//トランジションが終わった
	if (m_IN_transition.IsAnimFinished()) {
		//シーン遷移
		if (m_choose == 0) {
			m_isFinished = true;
		} else if (m_choose == 1) {
			m_isEnd = true;
		}
	}
}

/******************************************************
* ロゴアニメーション
*******************************************************/
void TitleScene::Logo() {
	m_logo.update(GAMESYS.GetDletaTime());
	
	if (m_logo.hasFinished() || CTRL.GetKeyboardTrigger(DIK_RETURN) || CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_CIRCLE, 0)) {
		AUDIO.StopAudio(m_logoSound);

		// ステート変更
		m_state = TITLE_OP;

		// オープニングの音を鳴らす
		AUDIO.PlayAudio(m_opSE, 0);

		// Update3回読まないと最初のフレームが描画されないからUpdateを三回呼び出してる。AVIだからちょっと重いのかも。こんな直し方ですまんね。これがプロ・プログラマーだよ。
		for (int i = 0; i < 3; i++) {
			m_op.Update(GAMESYS.GetDletaTime());
		}
		D3D.Draw2D(m_op.GetSRV(), XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f), XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT), PIXELMODE_DEFAULT);
	}
}

/******************************************************
* オープニングアニメーション
*******************************************************/
void TitleScene::Opening() {
	m_op.Update(GAMESYS.GetDletaTime());

	if (m_op.GetIsFinished() || CTRL.GetKeyboardTrigger(DIK_RETURN) || CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_CIRCLE, 0)) {
		AUDIO.StopAudio(m_opSE);

		m_state = TITLE_RUN;
		AUDIO.PlayAudio(m_soundNum, -1);
	}
}

