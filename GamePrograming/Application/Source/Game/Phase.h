/******************************************************
* Phaseh		フェーズ管理
* 制作者：ミヤタジョウジ
* 作成日：2024/10/22
* 最終更新日：2024/10/22
*******************************************************/
#pragma once

#include "Physics.h"
#include "Game/GameObject.h"
#include "Game/FieldManager.h"
#include "Game/ThrowObjectManager.h"
#include "Game/PlayerManager.h"

enum GAMESCENESTATE {
	GAMESCENESTATE_START = 0,
	GAMESCENESTATE_FINISH,
	GAMESCENESTATE_RUN,
};

/****************************************************
* フェーズクラス
*****************************************************/
class Phase {
public:
	Phase() = delete;
	Phase(const float gravityX, const float gravityY);
	~Phase();

	virtual void Update();
	virtual void Draw();

	virtual void Start();
	virtual void Finish();
	virtual void Run();
protected:
	//世界のルール
	Physics* m_physics = nullptr;

	//フェーズのステート
	GAMESCENESTATE m_state;

	//マネージャー群
	//GameObject* m_player = nullptr;
	PlayerManager* m_playerManager;
	FieldManager* m_fieldManager = nullptr;
	ThrowObjectManager* m_throwObjectManager = nullptr;
private:
};

