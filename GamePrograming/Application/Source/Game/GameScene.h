/******************************************************
* GameScene.h		�Q�[���V�[���Ǘ�
* ����ҁF�~���^�W���E�W
* �쐬���F2024/10/22
* �ŏI�X�V���F2024/10/22
*******************************************************/
#pragma once
#include "Scene.h"
#include "Physics.h"
#include "Game/GameObject.h"
#include "Field.h"

/****************************************************
* �Q�[���V�[���N���X
*****************************************************/
class GameScene : public Scene {
public:
	GameScene();
	~GameScene();

	void Update() override;
	void Draw() override;

private:
	//���E�̃��[��
	Physics* m_physics = nullptr;

	//�I�u�W�F�N�g
	GameObject* m_player = nullptr;
	Field* m_field = nullptr;

};

