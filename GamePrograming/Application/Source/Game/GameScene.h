/******************************************************
* GameScene.h		�Q�[���V�[���Ǘ�
* ����ҁF�~���^�W���E�W
* �쐬���F2024/10/22
* �ŏI�X�V���F2024/10/22
*******************************************************/
#pragma once
#include "Game/Scene.h"
#include "Game/Physics.h"
#include "Game/GameObject.h"

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
	Physics* m_physics = nullptr;
	GameObject* m_player = nullptr;
	GameObject* m_field = nullptr;

};

