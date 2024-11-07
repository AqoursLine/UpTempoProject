/******************************************************
* GameScene.cpp		�Q�[���V�[���Ǘ�
* ����ҁF�~���^�W���E�W
* �쐬���F2024/10/22
* �ŏI�X�V���F2024/10/22
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "GameScene.h"
#include "Game/Player.h"
#include "Game/Field.h"

/****************************************************
* �Q�[���V�[��������
*****************************************************/
GameScene::GameScene() {
	m_physics = new Physics(0.0f, 9.8);
	m_player = new Player();
	m_field = new Field();
}

/****************************************************
* �Q�[���V�[���X�V
*****************************************************/
void GameScene::Update() {
	m_physics->UpdatePhysics((1.0f / 60.0f), 8, 3);
	m_player->Update();
	m_field->Update();
}

/****************************************************
* �Q�[���V�[���`��
*****************************************************/
void GameScene::Draw() {
	m_player->Draw();
	m_field->Draw();
}

/****************************************************
* �Q�[���V�[���I��
*****************************************************/
GameScene::~GameScene() {
	if (m_physics) delete m_physics;
	if (m_player) delete m_player;
	if (m_field) delete m_field;
}
