/******************************************************
* Phaseh		フェーズ管理
* 制作者：ミヤタジョウジ
* 作成日：2024/10/22
* 最終更新日：2024/10/22
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/Phase.h"
#include "Game/Player.h"

/****************************************************
* スタティック変数初期化
*****************************************************/
PHASESTATE Phase::m_state = PHASESTATE_RUN;

/****************************************************
* フェーズ初期化
*****************************************************/
Phase::Phase(const int phaseNum, const float gravityX, const float gravityY) : m_physics(new Physics(gravityX, gravityY)) {
	m_state = PHASESTATE_RUN;

	m_fieldManager = new FieldManager();
	m_throwObjectManager = new ThrowObjectManager();
	m_playerManager = new PlayerManager(phaseNum);
}

/****************************************************
* フェーズ更新
*****************************************************/
void Phase::Update() {
	switch (m_state) {
		case PHASESTATE_START:
			Start();
			break;
		case PHASESTATE_FINISH:
			Finish();
			break;
		case PHASESTATE_RUN:
			Run();
			break;
		default:
			break;
	}
}

/****************************************************
* フェーズ描画
*****************************************************/
void Phase::Draw() {
	m_fieldManager->Draw();
	m_throwObjectManager->Draw();
	m_playerManager->Draw();
}

/****************************************************
* フェーズ終了
*****************************************************/
Phase::~Phase() {
	if (m_fieldManager) delete m_fieldManager;
	if (m_throwObjectManager) delete m_throwObjectManager;
	if (m_playerManager) delete m_playerManager;
	if (m_physics) delete m_physics;
}

/****************************************************
* フェーズ起動
*****************************************************/
void Phase::Start() {

}

/****************************************************
* フェーズ終了まで
*****************************************************/
void Phase::Finish() {
	m_stateCount++;
	if (m_stateCount >= m_targetCount) {
		m_isFinished = true;
	}
}

/****************************************************
* フェーズ実行
*****************************************************/
void Phase::Run() {
	m_physics->UpdatePhysics((1.0f / 60.0f), 8, 3);
	m_fieldManager->Update();
	m_throwObjectManager->Update();
	m_playerManager->Update();
}

/****************************************************
* フェーズ遷移
*****************************************************/
void Phase::ChangeState(PHASESTATE state) {
	m_state = state;
}
