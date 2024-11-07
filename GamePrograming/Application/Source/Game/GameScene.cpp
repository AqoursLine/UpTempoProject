/******************************************************
* GameScene.cpp		�Q�[���V�[���Ǘ�
* ����ҁF�~���^�W���E�W
* �쐬���F2024/10/22
* �ŏI�X�V���F2024/10/22
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameScene.h"
#include "Game/Player.h"
#include "Game/Field.h"
#include "Game/Fish.h"

/****************************************************
* �Q�[���V�[��������
*****************************************************/
GameScene::GameScene() {
	m_physics = new Physics(0.0f, 9.8);
	m_player = new Player();
	m_field = new Field();
	m_fish = new Fish();

}

/****************************************************
* �Q�[���V�[���X�V
*****************************************************/
void GameScene::Update() {
	m_physics->UpdatePhysics((1.0f / 60.0f), 8, 3);
	m_player->Update();
	m_field->Update();
	m_fish->Update();
}

/****************************************************
* �Q�[���V�[���`��
*****************************************************/
void GameScene::Draw() {
	m_player->Draw();
	m_field->Draw();
	m_fish->Draw();
}

/****************************************************
* �Q�[���V�[���I��
*****************************************************/
GameScene::~GameScene() {
	if (m_physics) delete m_physics;
	if (m_player) delete m_player;
	if (m_field) delete m_field;
	if (m_fish)delete m_fish;
}
