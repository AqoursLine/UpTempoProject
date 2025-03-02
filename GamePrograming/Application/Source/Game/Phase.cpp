/******************************************************
* Phaseh		フェーズ管理
* 制作者：ミヤタジョウジ
* 作成日：2024/10/22
* 最終更新日：2024/10/22
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "Game/Phase.h"
#include "Game/Player.h"

/****************************************************
* スタティック変数初期化
*****************************************************/
PHASESTATE Phase::m_state = PHASESTATE_RUN;
int Phase:: m_finishSound;

/****************************************************
* フェーズ初期化
*****************************************************/
Phase::Phase(const int phaseNum, const float gravityX, const float gravityY)
	: m_physics(new Physics(gravityX, gravityY)),
	m_OUT_transition(
	L"Data/Texture/Transition/OUT/Square_OUT.png",
	XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2),
	XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT),
	0.0f,
	5,
	6,
	30,
	0.5f,
	false
),
m_IN_transition(
	L"Data/Texture/Transition/IN/CircleLine_IN.png",
	XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2),
	XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT),
	0.0f,
	5,
	6,
	30,
	0.5f,
	false
),

m_FinishTransition(
	L"Data/Texture/Transition/finishAnim.png",
	XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2),
	XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT),
	0.0f,
	5,
	12,
	60,
	0.5f,
	false
),

m_StartAnim(L"Data/Movie/スタート演出.avi")
{
	m_state = PHASESTATE_OUTTRANSITION;

	m_fieldManager = new FieldManager();
	m_stageObjectManager = new StageObjectManager();
	m_throwObjectManager = new ThrowObjectManager();
	m_playerManager = new PlayerManager(phaseNum);

	
	m_startSound= AUDIO.LoadWaveFile("Data/Sound/SE/スタート演出音.wav");
	m_finishSound = AUDIO.LoadWaveFile("Data/Sound/SE/笛.wav");

	
	//サウンド音量
	AUDIO.SetVolume(m_startSound, 2.0f);
	AUDIO.SetVolume(m_finishSound, 1.0f);

	m_StartAnim.SetIsAutoLoop(false);				//　スタート演出ループ設定
	
}

/****************************************************
* フェーズ更新
*****************************************************/
void Phase::Update() {
	m_physics->UpdatePhysics((1.0f / 60.0f), 8, 3);
	m_stageObjectManager->PhysicsUpdate();
	m_throwObjectManager->PhysicsUpdate();
	m_playerManager->PhysicsUpdate();

	switch (m_state) {

		case PHASESTATE_OUTTRANSITION:
			OutTransition();
		break;

		case PHASESTATE_START:
			Start();
			break;
		
		case PHASESTATE_RUN:
			Run();
			break;

		case PHASESTATE_INTRANSITION:
			InTransition();
			break;

		case PHASESTATE_FINISH:
			Finish();
			break;

		default:
			break;
	}
}

/****************************************************
* フェーズ描画
*****************************************************/
void Phase::Draw() {
	D3D.Draw2D(m_texture, m_bgPos, m_bgSize);

	m_fieldManager->Draw();
	m_stageObjectManager->Draw();
	m_throwObjectManager->Draw();
	m_playerManager->Draw();


	// 最初のトランジション描画
	if (m_state == PHASESTATE_OUTTRANSITION) {
		m_OUT_transition.Draw();
	}

	//スタート演出の描画
	if (m_state == PHASESTATE_START)
	{
		D3D.Draw2D(m_StartAnim.GetSRV(),
			XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2),
			XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT), PIXELMODE_DEFAULT);
	}

	
	//終了演出描画
	if (m_state == PHASESTATE_FINISH) {
		m_FinishTransition.Draw();
	}

	// 次のシーンに移るまでのトランジション描画
	if (m_state == PHASESTATE_INTRANSITION) {
		m_IN_transition.Draw();
	}
}

/****************************************************
* フェーズ終了
*****************************************************/
Phase::~Phase() {
	if (m_fieldManager) delete m_fieldManager;
	if (m_stageObjectManager) delete m_stageObjectManager;
	if (m_throwObjectManager) delete m_throwObjectManager;
	if (m_playerManager) delete m_playerManager;
	if (m_physics) delete m_physics;
}

void Phase::OutTransition()
{
	m_OUT_transition.Update();

	if (m_OUT_transition.IsAnimFinished()) {

		//スタートの音
		AUDIO.PlayAudio(m_startSound, 0);

		m_state = PHASESTATE_START;
	}
}



/****************************************************
* フェーズ起動から遊べるようになるまで
*****************************************************/
void Phase::Start() {
	m_playerManager->CharacterUpdate();

	//動画更新
	m_StartAnim.Update(GAMESYS.GetDletaTime());
	
	//フェーズ起動処理終了
	if (m_StartAnim.GetIsFinished()) {
		m_state = PHASESTATE_RUN;
	}

}



/****************************************************
* フェーズ終了まで
*****************************************************/
void Phase::Finish() {

	m_FinishTransition.Update();

	
	if (m_FinishTransition.IsAnimFinished()) {
		m_state = PHASESTATE_INTRANSITION;
	}
}


void Phase::InTransition()
{
	m_IN_transition.Update();

	if (m_IN_transition.IsAnimFinished()) {
		m_isFinished = true;
	}
}

/****************************************************
* フェーズ実行
*****************************************************/
void Phase::Run() {
	m_fieldManager->Update();
	m_stageObjectManager->Update();
	m_throwObjectManager->Update();
	m_playerManager->Update();
}

/****************************************************
* フェーズ遷移
*****************************************************/
void Phase::ChangeState(PHASESTATE state) {
	m_state = state;

	if (state == PHASESTATE_FINISH)
	{
		AUDIO.PlayAudio(m_finishSound, 0);

	}
}


