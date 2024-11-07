/******************************************************
* GameScene.h		ゲームシーン管理
* 制作者：ミヤタジョウジ
* 作成日：2024/10/22
* 最終更新日：2024/10/22
*******************************************************/
#pragma once
#include "Scene.h"
#include "Physics.h"
#include "Game/GameObject.h"
#include "Field.h"

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
	//世界のルール
	Physics* m_physics = nullptr;

	//オブジェクト
	GameObject* m_player = nullptr;
	Field* m_field = nullptr;

};

