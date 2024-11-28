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
#include "Game/FieldManager.h"
#include "Game/ThrowObjectManager.h"

enum GAMESCENESTATE {
	GAMESCENESTATE_AWAKE = 0,
	GAMESCENESTATE_ASREEP,
	GAMESCENESTATE_RUN,
};

/****************************************************
* ゲームシーンクラス
*****************************************************/
class GameScene : public Scene {
public:
	GameScene() = delete;
	GameScene(const float gravityX, const float gravityY);
	~GameScene();

	virtual void Update() override;
	virtual void Draw() override;

	virtual void Awake();
	virtual void Asreep();
	virtual void Run();
protected:
	//世界のルール
	Physics* m_physics = nullptr;

	//ゲームシーンのステート
	GAMESCENESTATE m_state;
private:
	//マネージャー群
	GameObject* m_player = nullptr;
	FieldManager* m_fieldManager = nullptr;
	ThrowObjectManager* m_throwObjectManager;
};

