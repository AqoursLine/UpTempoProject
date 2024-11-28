/******************************************************
* GameScene.cpp		ゲームシーン管理
* 制作者：ミヤタジョウジ
* 作成日：2024/10/22
* 最終更新日：2024/10/22
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "GameScene.h"
#include "Game/Player.h"

/****************************************************
* ゲームシーン初期化
*****************************************************/
GameScene::GameScene() {
	m_physics = new Physics(0.0f, 9.8);
	m_camera = new Camera();
	m_fieldManager = new FieldManager();
	m_throwObjectManager = new ThrowObjectManager();
	m_player = new Player(1);
}

/****************************************************
* ゲームシーン更新
*****************************************************/
void GameScene::Update() {
	m_camera->Update();
	m_physics->UpdatePhysics((1.0f / 60.0f), 8, 3);
	m_fieldManager->Update();
	m_throwObjectManager->Update();
	m_player->Update();
}

/****************************************************
* ゲームシーン描画
*****************************************************/
void GameScene::Draw() {
	m_camera->Draw();
	m_fieldManager->Draw();
	m_throwObjectManager->Draw();
	m_player->Draw();
}

/****************************************************
* ゲームシーン終了
*****************************************************/
GameScene::~GameScene() {
	if (m_physics) delete m_physics;
	if (m_player) delete m_player;
	if (m_fieldManager) delete m_fieldManager;
	if (m_throwObjectManager) delete m_throwObjectManager;
	if (m_camera) delete m_camera;
}
