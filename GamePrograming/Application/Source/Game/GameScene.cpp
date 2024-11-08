/******************************************************
* GameScene.cpp		ゲームシーン管理
* 制作者：ミヤタジョウジ
* 作成日：2024/10/22
* 最終更新日：2024/10/22
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameScene.h"
#include "Game/Player.h"
#include "Game/Field.h"
#include "Game/Net.h"
#include "time.h"

/****************************************************
* ゲームシーン初期化
*****************************************************/
GameScene::GameScene() {
	m_physics = new Physics(0.0f, 0.0f); // 重力ゼロ
	m_player = new Player();

	for (int i = 0; i < FISH_MAX; i++) {
		m_fish[i] = new Fish();
	}

	srand((unsigned int)time(NULL));
}

/****************************************************
* ゲームシーン更新
*****************************************************/
void GameScene::Update() {
	m_physics->UpdatePhysics((1.0f / 60.0f), 8, 3);
	m_player->Update();
	
	for (int i = 0; i < FISH_MAX; i++) {
		m_fish[i]->Update();
	}
}

/****************************************************
* ゲームシーン描画
*****************************************************/
void GameScene::Draw() {
	m_player->Draw();

	for (int i = 0; i < FISH_MAX; i++) {
		m_fish[i]->Draw();
	}
}

/****************************************************
* ゲームシーン終了
*****************************************************/
GameScene::~GameScene() {
	if (m_physics) delete m_physics;
	if (m_player) delete m_player;
	

	for (int i = 0; i < FISH_MAX; i++) {
		if (m_fish[i]) delete m_fish[i];
	}
}
