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

enum PHASESTATE {
	PHASESTATE_START = 0,
	PHASESTATE_FINISH,
	PHASESTATE_RUN,
};

/****************************************************
* フェーズクラス
*****************************************************/
class Phase {
public:
	Phase() = delete;
	Phase(const int phaseNum, const float gravityX, const float gravityY);
	~Phase();

	virtual void Update();
	virtual void Draw();

	virtual void Start();
	virtual void Finish();
	virtual void Run();

	static void ChangeState(PHASESTATE state);

	const bool GetIsFinished() const { return m_isFinished; }
protected:
	//世界のルール
	Physics* m_physics = nullptr;

	//フェーズのステート
	static PHASESTATE m_state;

	//マネージャー群
	//GameObject* m_player = nullptr;
	PlayerManager* m_playerManager;
	FieldManager* m_fieldManager = nullptr;
	ThrowObjectManager* m_throwObjectManager = nullptr;
private:
	bool m_isFinished = false;

	int m_stateCount = 0;
	int m_targetCount = 30;
};

