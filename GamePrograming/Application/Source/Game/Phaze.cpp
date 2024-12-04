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
* フェーズ初期化
*****************************************************/
Phase::Phase(const float gravityX, const float gravityY) : m_physics(new Physics(gravityX, gravityY)) {
	m_state = GAMESCENESTATE_RUN;

	m_fieldManager = new FieldManager();
	m_throwObjectManager = new ThrowObjectManager();
	m_playerManager = new PlayerManager(1);
	//m_player = new Player();
}

/****************************************************
* フェーズ更新
*****************************************************/
void Phase::Update() {
	switch (m_state) {
		case GAMESCENESTATE_AWAKE:
			Awake();
			break;
		case GAMESCENESTATE_ASREEP:
			Asreep();
			break;
		case GAMESCENESTATE_RUN:
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
	///m_player->Draw();
	m_playerManager->Draw();
}

/****************************************************
* フェーズ終了
*****************************************************/
Phase::~Phase() {
	if (m_physics) delete m_physics;
	//if (m_player) delete m_player;
	if (m_fieldManager) delete m_fieldManager;
	if (m_throwObjectManager) delete m_throwObjectManager;

	m_playerManager->Finalize();
	if (m_playerManager) delete m_playerManager;

}

/****************************************************
* フェーズ起動
*****************************************************/
void Phase::Awake() {

}

/****************************************************
* フェーズ終了まで
*****************************************************/
void Phase::Asreep() {

}

/****************************************************
* フェーズ実行
*****************************************************/
void Phase::Run() {
	m_physics->UpdatePhysics((1.0f / 60.0f), 8, 3);
	m_fieldManager->Update();
	m_throwObjectManager->Update();
	//m_player->Update();
	m_playerManager->Update();
}
