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
#include "Game/Field.h"

/****************************************************
* ゲームシーン初期化
*****************************************************/
GameScene::GameScene() {
	m_physics = new Physics(0.0f, 9.8);
	m_player = new Player();
	m_field = new Field();
}

/****************************************************
* ゲームシーン更新
*****************************************************/
void GameScene::Update() {
	m_physics->UpdatePhysics((1.0f / 60.0f), 8, 3);
	m_player->Update();
	m_field->Update();
}

/****************************************************
* ゲームシーン描画
*****************************************************/
void GameScene::Draw() {
	m_player->Draw();
	m_field->Draw();
}

/****************************************************
* ゲームシーン終了
*****************************************************/
GameScene::~GameScene() {
	if (m_physics) delete m_physics;
	if (m_player) delete m_player;
	if (m_field) delete m_field;
}
