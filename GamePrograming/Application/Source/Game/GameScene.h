/******************************************************
* GameScene.h		ゲームシーン管理
* 制作者：ミヤタジョウジ
* 作成日：2024/10/22
* 最終更新日：2024/10/22
*******************************************************/
#pragma once
#include "Game/Scene.h"
#include "Game/Physics.h"
#include "Game/GameObject.h"
#include "Game/Fish.h"

/****************************************************
* ゲームシーンクラス
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
	GameObject* m_net = nullptr;
	GameObject* m_fish[FISH_MAX] = { nullptr };
};

