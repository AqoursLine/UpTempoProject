/******************************************************
* GameScene.h		ゲームシーン管理
* 制作者：センバソラ
* 作成日：2024/1/4
* 最終更新日：2024/1/4
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameScene.h"
#include "Game/PhaseClassRoom.h"
#include "Game/PhaseGame.h"
#include "Game/PhaseOcean.h"

/****************************************************
* ゲームシーン初期化
*****************************************************/
GameScene::GameScene() {
	std::random_device rd;
	m_mt = new std::mt19937(rd());

	m_camera = new Camera();

	m_ef = new EffectManager();

	m_phaseNum = 0;

	//1/4　センバ追加
	STAGE stage = SaveData::GetStageNum();
	switch (stage) {
	case STAGE_CLASSROOM:
		m_phase = new PhaseClassRoom(m_phaseNum);
		break;
	case STAGE_OCEAN:
		m_phase = new PhaseOcean(m_phaseNum);
		break;
	case STAGE_GAME:
		m_phase = new PhaseGame(m_phaseNum);
		break;
	default:
		m_phase = nullptr;
		break;
	}
	
}

/****************************************************
* ゲームシーン更新
*****************************************************/
void GameScene::Update() {
	m_phase->Update();
	m_camera->Update();
	m_ef->Update();

	if (m_phase->GetIsFinished()) {
		m_isFinished = true;
	}
}

/****************************************************
* ゲームシーン描画
*****************************************************/
void GameScene::Draw() {
	m_camera->Draw();
	m_phase->Draw();
	m_ef->Draw();
}

/****************************************************
* ゲームシーン終了
*****************************************************/
GameScene::~GameScene() {
	if (m_phase) delete m_phase;
	if (m_camera) delete m_camera;
	if (m_mt) delete m_mt;
	if (m_ef) delete m_ef;
}
