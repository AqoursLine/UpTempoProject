/******************************************************
* GameScene.h		ゲームシーン管理
* 制作者：ミヤタジョウジ
* 作成日：2024/10/22
* 最終更新日：2024/10/22
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
	m_phaseNum = 0;

	ChangePhase();
}

/****************************************************
* ゲームシーン更新
*****************************************************/
void GameScene::Update() {
	m_phase->Update();
	m_camera->Update();

	if (m_phase->GetIsFinished()) {
		ChangePhase();
	}
}

/****************************************************
* ゲームシーン描画
*****************************************************/
void GameScene::Draw() {
	m_camera->Draw();
	m_phase->Draw();
}

/****************************************************
* ゲームシーン終了
*****************************************************/
GameScene::~GameScene() {
	if (m_phase) delete m_phase;
	if (m_camera) delete m_camera;
	if (m_mt) delete m_mt;

	PlayerManager().Finalize();

}

/****************************************************
* フェーズチェンジ
*****************************************************/
void GameScene::ChangePhase() {
	if (m_phase) {
		delete m_phase;
		m_phase = nullptr;
	}

	m_phaseNum++;

#ifdef _DEBUG
	if (m_phaseNum == 1) {
		m_phase = new PhaseClassRoom(m_phaseNum);
	} else if (m_phaseNum == 2) {
		m_phase = new PhaseGame(m_phaseNum);
	} else if (m_phaseNum == 3) {
		m_phase = new PhaseOcean(m_phaseNum);
	}
#else
	std::uniform_int_distribution<> rand3(0, 2);
	int stageNum = rand3(m_mt);

	switch (m_phaseNum) {
		case 1:
			switch (stageNum) {
				case 0:
					break;
				case 1:
					break;
				case 2:
					break;
			}
			break;
		case 2:
			switch (stageNum) {
				case 0:
					break;
				case 1:
					break;
				case 2:
					break;
			}
			break;
		case 3:
			switch (stageNum) {
				case 0:
					break;
				case 1:
					break;
				case 2:
					break;
			}
			break;
	}
#endif // _DEBUG

}
