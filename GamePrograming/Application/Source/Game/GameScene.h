/******************************************************
* GameScene.h		�Q�[���V�[���Ǘ�
* ����ҁF�~���^�W���E�W
* �쐬���F2024/10/22
* �ŏI�X�V���F2024/10/22
*******************************************************/
#pragma once
#include "Scene.h"
#include "Physics.h"

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
	Physics* m_physics;
};

